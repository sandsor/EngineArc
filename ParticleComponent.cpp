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
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		//	// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
			glUniformMatrix4fv(mParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));

	
	}
}

void ParticleComponent::init(GLint matrixUniform)
{
	mMatrixUniform = matrixUniform;
	initializeOpenGLFunctions();
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER,      //what buffer type
		mVertices.size() * sizeof(Vertex),   //how big buffer do we need
		mVertices.data(),             //the actual vertices
		GL_STATIC_DRAW        //should the buffer be updated on the GPU
	);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
		3,                  // size / number of elements of data type
		GL_FLOAT,           // data type
		GL_FALSE,           // normalize data
		sizeof(Vertex),  // stride
		reinterpret_cast<GLvoid*>(0));          // array buffer offset
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &mEAB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ParticleComponent::draw()
{
	glBindVertexArray(mVAO);
	//glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
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
