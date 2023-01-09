#include "ppch.h"
#include "ParticleSystem.h"

#include <random>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#define GLM_ENABLE_EXPERIMENTAL

class Random {
public:
	inline static void Init() {
		s_RandomEngine.seed(std::random_device()());
	}

	inline static float Float() {
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}
private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution < std::mt19937::result_type> s_Distribution;
};

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution <std::mt19937::result_type> Random::s_Distribution;

ParticleSystem::ParticleSystem(uint32_t particlePoolSize) : poolIndex_(particlePoolSize - 1) {
	particlePool_.resize(particlePoolSize);
}

void ParticleSystem::OnUpdate(PriMech::Timestep timestep) {
	for (auto& particle : particlePool_) {
		if (!particle.active) continue;
		if (particle.lifeRemaining <= 0.0f) {
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= timestep;
		particle.position += particle.velocity * (float)timestep;
		particle.rotation += 0.01f * (float)timestep;
	}
}

void ParticleSystem::OnRender(PriMech::OrthographicCamera& camera) {
	PriMech::Renderer2D::BeginScene(camera);
	for (auto& particle : particlePool_) {
		if (!particle.active) continue;

		float life = particle.lifeRemaining / particle.lifeTime; //Fade the particle based on lifetime
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life); //color.a = color.a * life ...
		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

		PriMech::Renderer2D::DrawRotatedQuad(glm::vec3(particle.position, 0.0f), { size, size }, particle.rotation, color);

	}
	PriMech::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps) {
	Particle& particle = particlePool_[poolIndex_];
	particle.active = true;
	particle.position = particleProps.position;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	//Velocity
	particle.velocity = particleProps.velocity;
	particle.velocity.x += particleProps.velocityVariation.x * (Random::Float() - 0.5f);
	particle.velocity.y += particleProps.velocityVariation.y * (Random::Float() - 0.5f);

	//Color
	particle.colorBegin = particleProps.colorBegin;
	particle.colorEnd = particleProps.colorEnd;

	particle.lifeTime = particleProps.lifeTime;
	particle.lifeRemaining = particleProps.lifeTime;
	particle.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (Random::Float() - 0.5f);
	particle.sizeEnd = particleProps.sizeEnd;

	poolIndex_ = --poolIndex_ % particlePool_.size();
}
