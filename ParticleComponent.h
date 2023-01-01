#pragma once
#include<visualobject.h>
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

};

