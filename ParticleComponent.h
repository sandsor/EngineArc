#pragma once
#include<visualobject.h>
#include<vector2d.h>
#include<vector3d.h>
#include<glm/vec4.hpp>
#include<glm/gtx/compatibility.hpp>
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <matrix4x4.h>
#include <vector>
#include "vertex.h"
#include "cube.h"


struct ParticleProperties 
{
	glm::vec3 Position;
	glm::vec3 Velocity, VelocityVariation;
	glm::vec3 ColorBegin;
	glm::vec3 ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime=1.0f;
};
class ParticleComponent : public VisualObject
{
public:
	ParticleComponent();
	~ParticleComponent();
	void Update(float deltaTime);
	void Render();
	void init(GLint matrixUniform)override;
	void draw()override;
	glm::vec3 calculateColor(float mLife, glm::vec3 mColorBegin,glm::vec3 mColorEnd );

	void Emit(const ParticleProperties& particleProperties);
private:
	struct Particle
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 ColorBegin;
		glm::vec3 ColorEnd;
		float Rotation;
		float SizeBegin, SizeEnd;
		Cube CubeParticle;
		float LifeTime = 1.0f;
		float LifeRemaining= 0.0f;
		bool Active = false;

	};
	std::vector<Particle> mParticlePool;
	uint32_t mPoolIndex = 999;
	//stuff to draw sqare
	GLuint mQuadVA = 0;
	//std::unique_ptr<GLCore::utils::Shader> mParticleShader;
	GLint mParticleShaderViewProj, mParticleShaderTransform, mParticleShaderColor;
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