#pragma once

namespace glar
{
	const double EPSILON = 0.00000001;
	const bool CULLING = false;
	const double PI = 3.14159265358979323846;
	const double invPI = 0.31830988618379067154;
	const double inv2PI = 0.15915494309189533577;
	const double inv4Pi = 0.07957747154594766788;
	const double piOver2 = 1.57079632679489661923;
	const double piOver4 = 0.78539816339744830961;
	const int LIGHT_SAMPLES = 8;
	const double LIGHT_INTENSITY = 300;
	
	enum Axis { x, y, z };

	enum BxDFType {
		REFLECTION = 1 << 0,
		TRANSMISSION = 1 << 1,
		DIFFUSE = 1 << 2,
		GLOSSY = 1 << 3,
		SPECULAR = 1 << 4,
		ALL = DIFFUSE | GLOSSY | SPECULAR | REFLECTION | TRANSMISSION,
	};

	enum RenderType
	{
		BASIC = 1 << 0,
		MULTIPLE_LIGHT_SAMPLING = 1 << 1,
		ALL_LIGHTS_SAMPLIG = 1 << 2,
		INDIRECT = 1 << 3,
		BEST = MULTIPLE_LIGHT_SAMPLING | ALL_LIGHTS_SAMPLIG | INDIRECT,
	};
}