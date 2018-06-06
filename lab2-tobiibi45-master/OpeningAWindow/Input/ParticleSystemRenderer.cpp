#include "ParticleSystemRenderer.h"
#include <glad/glad.h>


ParticleSystemRenderer::ParticleSystemRenderer(unsigned int maxParticles)
{
	m_maxParticles = maxParticles;
}


void ParticleSystemRenderer::initialise()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};
	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// VBO containing the positions of the particles
	glGenBuffers(1, &VBO_positionsAndColours);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positionsAndColours);
	// Initialise with empty (NULL) buffer
	// subsequently updated each frame (STREAM_DRAW).
	//glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), NULL, GL_STREAM_DRAW);
}

//void ParticleSystemRenderer::update(Particle* particleData, int numberOfLiveParticles)
//{
//	//glBindBuffer(GL_ARRAY_BUFFER, VBO_positionsAndColours);
//	//// Set Buffer to NULL before overwrite with SubData
//	//glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), NULL, GL_STREAM_DRAW);
//	//glBufferSubData(GL_ARRAY_BUFFER, 0, numberOfLiveParticles * sizeof(Particle), particleData);
//	//// set the vertex attribute pointers
//	//// billboard vertices
//	//glEnableVertexAttribArray(0);
//	//glBindBuffer(GL_ARRAY_BUFFER, VBO_billboard);
//	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//	//// position and size
//	//glEnableVertexAttribArray(1);
//	//glBindBuffer(GL_ARRAY_BUFFER, VBO_positionsAndColours);
//	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
//	//// colours
//	//glEnableVertexAttribArray(2);
//	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, colour));
//}

void ParticleSystemRenderer::render(unsigned int numberOfParticles, const unsigned int shaderProgram)
{
	// The first parameter is the buffer number
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
		// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
		// this is set to 0 if we always use the same data
		glVertexAttribDivisor(0, 0); // billboard : always reuse 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // pos : one per particle -> 1
	glVertexAttribDivisor(2, 1); // colour : one per particle -> 1
								 // draw the instanced particles
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numberOfParticles);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
