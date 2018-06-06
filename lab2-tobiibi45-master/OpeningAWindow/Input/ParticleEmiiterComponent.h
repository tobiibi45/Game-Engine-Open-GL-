#pragma once
#include "Components.h"
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "ParticleSystemRenderer.h"

class ParticleEmiiterComponent : public Components
{

public:

	//struct Particle
	//{
	//	// initialised to dead particle (-1.0f life)
	//	Particle() : lifeRemaining(-1.0f), cameraSqDistance(-1.0f) {}
	//	// variables that need passing to the shader
	//	glm::vec3 position;
	//	float size;
	//	glm::vec4 colour;
	//	// other variables
	//	glm::vec3 velocity;
	//	float lifeRemaining;
	//	float cameraSqDistance;
	//	// sort to ensure the particles further away are drawn first
	//	bool operator<(const Particle& that) const
	//	{
	//		return this->cameraSqDistance > that.cameraSqDistance;
	//	}
	//};

	//Particle* m_particles;
	//int m_timeSinceLastParticleGen;
	//int LastUsedParticle;
	//const unsigned int maxParticles = 10000;

	//ParticleEmiiterComponent () 
	//{
	//	ParticleSystemRenderer* m_particleRenderer = new ParticleSystemRenderer(maxParticles);
	//	m_particles = new Particle[maxParticles];

	//	LastUsedParticle = 0;
	//	m_numLiveParticles = 0;
	//	m_timeSinceLastParticleGen = 0;
	//}
	////…
	////… keep track and only draw m_numLiveParticles

	void OnUpdate(float dt) override
	{
		//// generate new particles
		//m_timeSinceLastParticleGen += dt;
		//if (m_timeSinceLastParticleGen > 1)
		//{
		//	int particleIndex = FindUnusedParticle();
		//	m_particles[particleIndex].lifeRemaining = 500.0f;
		//	float randomXStart = ((rand() % RAND_MAX) / (float)RAND_MAX) * 50000.0f - 25000.0f;
		//	float randomZStart = ((rand() % RAND_MAX) / (float)RAND_MAX) * 50000.0f - 25000.0f;
		//	m_particles[particleIndex].position = glm::vec3(randomXStart, 5000, randomZStart);
		//	m_particles[particleIndex].velocity = glm::vec3(0, 0, 0);
		//	m_particles[particleIndex].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//	m_particles[particleIndex].size = 100;
		//	m_timeSinceLastParticleGen = 0;
		//}

		//std::sort(&m_particles[0], &m_particles[maxParticles]);
		//// Then call the following after updating the particles
		//// sort the particles based on their distance from the camera
	}
	void OnMessage(const std::string m) override {}
		
	int FindUnusedParticle() 
	{

	//	for (int i = LastUsedParticle; i < maxParticles; i++) {
	//		if (m_particles[i].lifeRemaining < 0) {
	//			LastUsedParticle = i;
	//			return i;
	//		}
	//	}

	//	for (int i = 0; i<LastUsedParticle; i++) {
	//		if (m_particles[i].lifeRemaining < 0) {
	//			LastUsedParticle = i;
	//			return i;
	//		}
	//	}

	//	return 0; // All particles are taken, override the first one
	}

};
#pragma once
