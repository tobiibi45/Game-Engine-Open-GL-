#pragma onc
#include <string>

class ParticleSystemRenderer
{
public:

	ParticleSystemRenderer(unsigned int maxParticles);

	void setTexture(std::string textureFilepath);
	void initialise();
	//void update(Particle* particleData, int numberOfLiveParticles);
	void render(unsigned int numberOfParticles, const unsigned int shaderProgram);

	static unsigned int TextureFromFile(const char* filepath, const std::string& directory, bool gamma = false);
	unsigned int VAO;

private:
	unsigned int m_textureIndex;
	unsigned int VBO_billboard; // stays the same for each particle
	unsigned int VBO_positionsAndColours; // changes every frame
	int m_maxParticles;
};

