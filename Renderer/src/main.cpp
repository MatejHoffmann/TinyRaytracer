#include <chrono>

#include "core/Camera.h"
#include "core/Renderer.h"

int main()
{
	int width = 800;
	int height = 800;
	int samples = 512;
	int bounces = 3;
	int threads = 24;
	glar::RenderType renderTechnique(glar::RenderType::INDIRECT);
	glar::Camera camera(width, height, glar::Point(0.0, 1.0, 2.7), glar::Point(0.0, 1.0, 0.0), 60.0);
	std::string objPath = R"(scenes/visual_scenes/Cornell_box_spheres_mirror.obj)";
	std::string savePath = R"(scenes/result.ppm)";
	
	glar::Renderer renderer(camera, objPath, savePath);
	renderer.render(samples, bounces,threads,renderTechnique);
	return 0;
}


