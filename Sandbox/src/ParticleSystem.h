#pragma once

#include <PriMech.h>

struct ParticleProps {
	glm::vec2 position;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem(uint32_t particlePoolSize = 100000);

	void OnUpdate(PriMech::Timestep timestep);
	void OnRender(PriMech::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle {
		glm::vec2 position;
		glm::vec2 velocity, velocityVariation;
		glm::vec4 colorBegin, colorEnd;

		float sizeBegin, sizeEnd, sizeVariation;
		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;
		float rotation = 0.0f;
		
		bool active = false;
	};
	std::vector<Particle> particlePool_;
	uint32_t poolIndex_;
};