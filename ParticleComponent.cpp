#include "ParticleComponent.h"
ParticleComponent::ParticleComponent()
{
	//mParticlePool.resize(1000);
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
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		//	// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
		//	glUniformMatrix4fv(m_ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
		//	glUniform4fv(m_ParticleShaderColor, 1, glm::value_ptr(color));
		//	glBindVertexArray(m_QuadVA);
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		//}
	}
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
