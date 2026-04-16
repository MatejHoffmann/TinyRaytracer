#include "Renderer.h"


glar::Renderer::Renderer(const Camera & camera, const std::string& scenePath, const std::string& savePath)
:	m_scenePath(scenePath),
	m_savePath(savePath),
	m_image({camera.getWidth(),camera.getHeight()}),
	m_camera(camera),
	m_completedThreads(0)
{
	m_background = Color(0.1, 0.1, 0.2);
	m_scene.loadScene(scenePath);
}

void glar::Renderer::render(int samples, int bounces, int threadsCount, RenderType flags)
{
	auto start = std::chrono::system_clock::now();
	
	multiThreadRender(samples, bounces, threadsCount, flags);
	
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	
	std::cout << elapsed.count() << '\n';
	
	m_image.saveImage(m_savePath);
}

void glar::Renderer::threadJob(int start, int end, int samples, int bounces, RenderType flags)
{
	Block block;
	block.startIndex = start;
	block.endIndex = end;
	bool indirect = flags & RenderType::INDIRECT;

	for (int i = start; i < end; i++)
	{
		Color L(0.0);
		for (int s = 0; s < samples; s++)
		{
			Ray ray = m_camera.generateRay(i);
			Intersection it;
			if (!m_scene.intersection(ray, it))
			{
				L = m_background*samples;
			}
			else
			{
					L += directIllumination(it, flags);
				if (indirect)
					L += indirectIllumination(it, bounces, flags);
			}
		}
		block.colors.push_back(L / samples);
	}

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_blocks.push_back(std::move(block));
		m_completedThreads++;
		m_cv.notify_one();
	}
}

void glar::Renderer::multiThreadRender(int samples, int bounces, int threadsCount ,RenderType flags)
{
	int availableThreads = std::thread::hardware_concurrency();
	int nThreads = threadsCount > availableThreads ? availableThreads : threadsCount;
	int pixelsPerThread = m_image.numberOfPixels() / nThreads;
	int leftOver = m_image.numberOfPixels() % nThreads;



	for (int i = 0; i < nThreads; ++i)
	{
		int start = i * pixelsPerThread;
		int end = pixelsPerThread + i * pixelsPerThread;

		if(i == nThreads -1)
		{
			end += leftOver;
		}
		
		std::thread t([this, start, end, samples, bounces, &flags]()
			{
			threadJob(start,end,samples,bounces,flags);
			});
		m_threads.push_back(std::move(t));
	}

	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this, &nThreads] {
			return m_completedThreads == nThreads;
			});
	}

	for (std::thread& t : m_threads)
	{
		t.join();
	}

	for (Block block : m_blocks)
	{
		int start = block.startIndex;
		int end = block.endIndex;
		int colorIndex = 0;
		for (int i = start; i < end; i++, colorIndex++)
		{
			m_image.setPixel(i, block.colors[colorIndex]);
		}
	}
}

double glar::Renderer::powerHeuristic(int nf, double fPdf, int ng, double gPdf) const
{
	double f = nf * fPdf, g = ng * gPdf;
	return (f * f) / (f * f + g * g);
}

glar::Color glar::Renderer::directShading(const Intersection& intersection, const std::shared_ptr<const Light>& light) const
{
	auto material = intersection.primitive->getMaterial();
	if (material->isEmissive())
		return material->emission();

	Color Ld(0.0);

	Vector wi, wo;
	Color Li = 0.0, f = 0.0;
	double lightPdf, scatteredPdf;

	if (light->sample(intersection.point, wi, Li, lightPdf, m_scene))
	{
		f = material->f(intersection, wi, intersection.wo);
		scatteredPdf = material->pdf(intersection, wi, intersection.wo);
		if (f != Color(0.0) && scatteredPdf != 0.0)
		{
			double weight = powerHeuristic(1, lightPdf, 1, scatteredPdf);
			Ld += f * Li * weight * std::max(0.0, dot(intersection.normal, -wi)) / lightPdf;
		}
	}

	if (material->sample(intersection, -intersection.wo, wo, f, scatteredPdf))
	{
		lightPdf = light->pdf(intersection.point, -wo);

		if (lightPdf > 0.0 && light->Le(intersection.point, -wo, Li, m_scene))
		{
			double weight = powerHeuristic(1, scatteredPdf, 1, lightPdf);
			Ld += f * Li * weight * std::max(0.0, dot(intersection.normal, wo)) / scatteredPdf;
		}
	}
	return Ld;
}

glar::Color glar::Renderer::indirectShading(const Intersection& intersection, int bounces, RenderType flags) const
{
	Color L = directIllumination(intersection, flags);

	if (bounces == 0)
		return L;

	double pdf;
	Vector wo;
	auto material = intersection.primitive->getMaterial();

	Color f;
	if (!material->sample(intersection, -intersection.wo, wo, f, pdf))
		return L;
	//f *= std::max(0.0, dot(wo, intersection.normal));
	f *= std::abs(dot(wo, intersection.normal));
	Intersection it;
	if (!m_scene.intersection(Ray(intersection.point, wo), it))
		return L;
	return L + f * indirectShading(it, bounces - 1, flags) / pdf;
}


glar::Color glar::Renderer::indirectIllumination(const Intersection& intersection, int bounces, RenderType flags) const
{
	double pdf;
	Vector wo;
	auto material = intersection.primitive->getMaterial();
	Color f, L = Color(0.0f);

		if (material->sample(intersection, -intersection.wo, wo, f, pdf))
		{
			Intersection it;
			if (m_scene.intersection(Ray(intersection.point, wo), it))
			{
				//L += f * indirectShading(it, bounces, flags) * std::max(0.0, dot(wo, intersection.normal)) / pdf;
				L += f * indirectShading(it, bounces, flags) * std::abs(dot(wo, intersection.normal)) / pdf;
			}
		}
	return L;
}

glar::Color glar::Renderer::directIllumination(const Intersection& intersection, RenderType flags) const
{
	if (intersection.primitive->getMaterial()->isEmissive())
		return intersection.primitive->getMaterial()->emission();

	int nLights = m_scene.lights.size();
	
	if (nLights == 0)
		return Color(0.0);

	//if all lights should be sampled
	
	if(flags & RenderType::ALL_LIGHTS_SAMPLIG)
	{
		Color L(0.0);

		for (int i = 0; i < nLights; i++)
		{
			const std::shared_ptr<Light>& light = m_scene.lights[i];
			int nSamples = flags & RenderType::MULTIPLE_LIGHT_SAMPLING ? light->numberOfSamples() : 1;
			Color Ld(0.0);
			for (int j = 0; j < nSamples; j++)
			{
				Ld += directShading(intersection, light);
			}
			L += Ld / nSamples;
		}
		return L;
	}

	//otherwise randomly chose one to be sampled

	Sampler sampler;
	double p = sampler.uniformSampleOne();

	int lightNum = std::min((int)p * nLights, nLights - 1);
	double lightPdf = 1.0 / nLights;
	const std::shared_ptr<Light>& light = m_scene.lights[lightNum];
	int nSamples = flags & RenderType::MULTIPLE_LIGHT_SAMPLING ? light->numberOfSamples() : 1;
	Color Ld(0.0);

	for (int i = 0; i < nSamples; i++)
	{
		Ld += directShading(intersection, light);
	}
	Ld /= nSamples;

	return Ld / lightPdf;
}
