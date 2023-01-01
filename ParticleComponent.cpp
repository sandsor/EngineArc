#include "ParticleComponent.h"


ParticleComponent::ParticleComponent()
{
	mParticlePool.resize(1000);
	mMatrix.setToIdentity();


}

ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::Update(float deltaTime)
{
	for (auto& particle : mParticlePool)
	{
		if (!particle.Active)
			continue;
		if(particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}
		particle.LifeRemaining -=deltaTime;
		particle.Position += particle.Velocity*(float)deltaTime;
		particle.Rotation += 0.01f * deltaTime;
	}

}

void ParticleComponent::Render()
{
	//rendering stuff

	for (auto& particle : mParticlePool)
	{
		if (!particle.Active)
			continue;

		//	// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec3 color = calculateColor(life, particle.ColorBegin, particle.ColorEnd);

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		//	// Render
		particle.CubeParticle.move(particle.Position[0], particle.Position[1], particle.Position[2]);
		particle.CubeParticle.ChangeColor(color[0],color[1],color[2]);
			//* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
			//* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
			//glUniformMatrix4fv(mParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));

	
	}
}

void ParticleComponent::init(GLint matrixUniform)
{
	for (auto& particle : mParticlePool)
	{
		if (!particle.Active)
			continue;
		particle.CubeParticle.init(matrixUniform);

	}
}

void ParticleComponent::draw()
{
	for (auto& particle : mParticlePool)
	{
		if (!particle.Active)
			continue;
		particle.CubeParticle.draw();

	}
}

glm::vec3 ParticleComponent::calculateColor(float mLife, glm::vec3 mColorBegin, glm::vec3 mColorEnd)
{
	glm::vec3 color;
	float RedDifference = mColorBegin[0] - mColorEnd[0];
	float GreenDifference = mColorBegin[1] - mColorEnd[1];
	float BlueDifference = mColorBegin[2] - mColorEnd[2];
	color[0] = mColorBegin[0] - (1 - mLife) * mColorEnd[0];
	color[1] = mColorBegin[1] - (1 - mLife) * mColorEnd[1];
	color[2] = mColorBegin[2] - (1 - mLife) * mColorEnd[2];

	return color;
}




	void ParticleComponent::Emit(const ParticleProperties & particleProperties)
{
	Particle& particle = mParticlePool[mPoolIndex];
	particle.Active = true;
	particle.Position = particleProperties.Position;
	//Need random num gen here
	//Particle.Rotation = Random::float() * 2.0f * glm::pi<float();

	//Velocity
	particle.Velocity = particleProperties.Velocity;
	//particle.Velocity.x = particleProperties.VelocityVariation.x*(Random::Float()-0.5f);
	//particle.Velocity.y = particleProperties.VelocityVariation.y*(Random::float()-0.5f);

	//Color
	particle.ColorBegin = particleProperties.ColorBegin;
	particle.ColorEnd = particleProperties.ColorEnd;

	particle.LifeTime = particleProperties.LifeTime;
	particle.LifeRemaining = particleProperties.LifeTime;
	particle.SizeBegin = particleProperties.SizeBegin + particleProperties.SizeEnd;

	mPoolIndex = --mPoolIndex % mParticlePool.size();


}
