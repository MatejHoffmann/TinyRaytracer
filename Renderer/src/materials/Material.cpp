#include "Material.h"

glar::Material::Material(const Color& emission)
: m_emission(emission)
{
}

void glar::Material::addBxDF(std::shared_ptr<BxDF> bxdf)
{
	m_type = BxDFType(bxdf->getMaterialType() | m_type);
	m_BxDFs.push_back(std::move(bxdf));
}

glar::Color glar::Material::f(const Intersection& it, const Vector& wi, const Vector& wo, BxDFType type) const
{
	bool reflect = dot(-wi, it.normal) * dot(wo,it.normal) > 0;
	Color f(0);
	for (int i = 0; i < m_BxDFs.size(); i++)
	{
		if (m_BxDFs[i]->MatchesFlags(type) && ((reflect && (m_BxDFs[i]->type & REFLECTION)) || (!reflect && (m_BxDFs[i]->type & TRANSMISSION))))
			f += m_BxDFs[i]->f(it, wi, wo);
	}
	return f;
}

bool glar::Material::sample(const Intersection& it, const Vector& wi, Vector& wo, Color& f, double& pdf, BxDFType type) const
{
	pdf = 0.0;
	f = Color(0.0);
	
	int matchingComps = NumOfComponents(type);
	if(matchingComps == 0)
	{
		pdf = 0.0;
		f = Color(0.0);
		return false;
	}

	Sampler sampler;
	int comp = std::min((int)std::floor(sampler.uniformSampleOne() * matchingComps), matchingComps - 1);
	std::shared_ptr<BxDF> bxdf(nullptr);
	int count = comp;
	for(int i = 0; i < m_BxDFs.size(); i++)
	{
		if(m_BxDFs[i]->MatchesFlags(type) && count-- == 0)
		{
			bxdf = m_BxDFs[i];
			break;
		}
	}

	f = bxdf->sample(it, wi, wo , pdf);
	if(pdf == 0.0)
	{
		f = Color(0.0);
		return false;
	}

	if(!(bxdf->type & SPECULAR) && matchingComps > 1)
	{
		for (int i = 0; i < m_BxDFs.size(); i++)
		{
			if (m_BxDFs[i] != bxdf && m_BxDFs[i]->MatchesFlags(type))
				pdf += m_BxDFs[i]->pdf(it, wi, wo);
		}
	}
	
	if (matchingComps > 1)
		pdf /= matchingComps;


	if (!(bxdf->type & SPECULAR))
	{
		bool reflect = dot(-wi, it.normal) * dot(wo, it.normal) > 0;
		f = 0;
		
		for (int i = 0; i < m_BxDFs.size(); i++)
		{
			if (m_BxDFs[i]->MatchesFlags(type) && ((reflect && (m_BxDFs[i]->type & REFLECTION)) || (!reflect && (m_BxDFs[i]->type & TRANSMISSION))))
				f += m_BxDFs[i]->f(it,wi, wo);
		}
	}
	return true;	
}

double glar::Material::pdf(const Intersection& it, const Vector& wi, const Vector& wo, BxDFType type) const
{
	if (m_BxDFs.size() == 0) 
		return 0.0;

	double tmp = 0.0;
	int matchingComps = 0;
	for (int i = 0; i < m_BxDFs.size(); i++)
	{
		if (m_BxDFs[i]->MatchesFlags(type)) 
		{
			matchingComps++;
			tmp += m_BxDFs[i]->pdf(it, wi, wo);
		}
	}
	return matchingComps > 0 ? tmp / matchingComps : 0;
}

bool glar::Material::isEmissive() const
{
	return m_emission != Color(0.0);
}

int glar::Material::NumOfComponents(BxDFType flags) const
{
	int num = 0;
	for (int i = 0; i < m_BxDFs.size(); i++)
		if (m_BxDFs[i]->MatchesFlags(flags)) 
			num++;
	return num;
}

int glar::Material::NumOfBxdfs() const
{
	return m_BxDFs.size();
}
