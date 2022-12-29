#pragma once
#include<vector2d.h>
#include<vector3d.h>
#include<glm/vec4.hpp>
#include<glm/gtx/compatibility.hpp>
#include<random>

struct ParticleProperties
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime=1.0f;
};
class ParticleComponent
{
public:
	ParticleComponent();

	void Update(float deltaTime);
	void Render();

	void Emit(const ParticleProperties& particleProperties);
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation;
		float SizeBegin, SizeEnd;
		float LifeTime = 1.0f;
		float LifeRemaining= 0.0f;
		bool Active = false;

	};
	std::vector<Particle> mParticlePool;
	uint32_t mPoolIndex = 999;
	//stuff to draw sqare
	/*GLuint mQuadVA = 0;
	std::unique_ptr<GLCore::utils::Shader> mParticleShader;
	GLint mParticleShaderViewProj, mParticleShaderTransform, mParticleShaderColor;*/
};

//code in renderwindow for this code example:

//header
//private:
//	GLCore::Utils::OrthographicCameraController m_CameraController;
//	ParticleProps m_Particle;
//	ParticleSystem m_ParticleSystem;


//cpp
//m_Particle.Position = { x + pos.x, y + pos.y };
//for (int i = 0; i < 5; i++)
//	m_ParticleSystem.Emit(m_Particle);
//m_ParticleSystem.OnUpdate(ts);
//m_ParticleSystem.OnRender(m_CameraController.GetCamera());
//
//m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
//m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
//m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
//m_Particle.LifeTime = 1.0f;
//m_Particle.Velocity = { 0.0f, 0.0f };
//m_Particle.VelocityVariation = { 3.0f, 1.0f };
//m_Particle.Position = { 0.0f, 0.0f };