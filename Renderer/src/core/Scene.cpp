#include "Scene.h"
#define TINYOBJLOADER_IMPLEMENTATION 
#include <iostream>

#include <tiny_obj_loader.h>
#include "../shapes/Triangle.h"
#include "../lights/AreaLight.h"
#include "../BxDF/Glossy.h"
#include "../BxDF/Lambertian.h"
#include "../BxDF/SpecularReflection.h"
#include "../BxDF/SpecularTransmission.h"

bool glar::Scene::intersection(const Ray& ray, Intersection& it) const
{
	return bvhTree.intersect(ray, it);
}

bool glar::Scene::naiveIntersection(const Ray& ray, Intersection& it) const
{
	bool found = false;
	for(int i = 0; i< primitives.size(); i++)
	{
		Intersection tmp;
		if(primitives[i]->intersect(ray, tmp))
		{
			found = true;
			if(tmp < it)
			{
				it.normal = tmp.normal;
				it.point = tmp.point;
				it.wo = tmp.wo;
				it.t = tmp.t;
				it.primitive = tmp.primitive;
			}
		}
	}
	return found;
}

bool glar::Scene::isVisible(const Point& p0, const Point& p1) const
{
	double t = (p0 - p1).length();
	Intersection it;
	if (intersection(Ray(p0, p1), it))
	{
		if (std::abs(it.t - t) < 0.01)
			return true;
	}
	return false;
}

void glar::Scene::loadScene(const std::string& path)
{
	lights.clear();
	primitives.clear();
	materials.clear();

	tinyobj::ObjReaderConfig reader_config;
	reader_config.triangulate = true;

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(path, reader_config))
	{
		if (!reader.Error().empty())
		{
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty())
	{
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& mat = reader.GetMaterials();

	Material defaultMat;
	defaultMat.addBxDF(std::make_shared<Lambertian>(Color(0.8, 0.8, 0.8)));
	
	materials.push_back(std::make_shared<Material>(defaultMat));

	for (auto& material : mat)
	{
		Color diffuse;
		Color specular;
		double shininess;
		Color emmission;
		Material m;

		for (int i = 0; i < 3; i++)
		{
			diffuse[i] = material.diffuse[i];
			specular[i] = material.specular[i];
			emmission[i] = material.emission[i];
		}
		shininess = material.shininess;

		if (emmission != Color(0.0))
			materials.push_back(std::make_shared<Material>(Material(emmission)));
		else
		{
			if (diffuse != Color(0.0))
				m.addBxDF(std::make_shared<Lambertian>(diffuse));
			if (specular != Color(0.0))
			{
				//m.addBxDF(std::make_shared<Glossy>(specular, shininess));
				//m.addBxDF(std::make_shared<SpecularReflection>(specular));
				m.addBxDF(std::make_shared<SpecularTransmission>(specular,2));
			}
			if (m.NumOfBxdfs() == 0)
			{
				m.addBxDF(std::make_shared<Lambertian>(Color(0.8,0.8,0.8)));
			}
			materials.push_back(std::make_shared<Material>(m));
		}
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = shapes[s].mesh.num_face_vertices[f];
			Vertex vertexes[3];

			bool recalculate = true;
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

				if (idx.normal_index >= 0)
				{
					recalculate = false;
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					vertexes[v] = Vertex(Point(vx, vy, vz), Vector(nx, ny, nz));
				}
				else
				{
					vertexes[v] = Vertex(Point(vx, vy, vz));
				}
			}

			index_offset += fv;

			// per-face material
			int materialId = shapes[s].mesh.material_ids[f]+1;

			auto primitive = std::make_shared<Triangle>(Triangle(vertexes[0], vertexes[1], vertexes[2], materials[materialId],false,recalculate));

			primitives.push_back(primitive);
			if (materials[materialId]->isEmissive())
			{
				auto light = std::make_shared<AreaLight>(AreaLight(materials[materialId]->emission(), LIGHT_INTENSITY, LIGHT_SAMPLES, primitive));
				lights.push_back(light);
			}
		}
	}
	bvhTree.build(primitives);
}
