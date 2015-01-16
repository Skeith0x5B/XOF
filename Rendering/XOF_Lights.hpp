/*
===============================================================================

	XOF
	===
	File	:	XOF_Lights.hpp
	Desc	:	Describes light types.

===============================================================================
*/
#ifndef XOF_LIGHTS_HPP
#define XOF_LIGHTS_HPP


#include <glm/glm.hpp>


// Attributes common to both directional and point-lights
struct BaseLight {
	glm::vec3	color;
	float		ambientIntensity;
	float		diffuseIntensity;
};


struct DirectionalLight : public BaseLight {
				DirectionalLight() { 
					std::memset( this, 0x00, sizeof( DirectionalLight ) ); 
				}

	glm::vec3	direction;
};


struct PointLight : public BaseLight {
					PointLight() { 
						std::memset( this, 0x00, sizeof( PointLight ) ); 
					}

	glm::vec3		position;

	struct	Attentuation {
		float constant;
		float linear;
		float exp;
	};

	Attentuation	attenuation;
};


#endif // XOF_LIGHTS_HPP