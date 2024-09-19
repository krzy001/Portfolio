#include "Common.h"
#include "House.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


CHouse::CHouse()
{}

CHouse::~CHouse()
{}


// Create the plane, including its geometry, texture mapping, normal, and colour
void CHouse::Create(string directory, string filename, float width, float height, float textureRepeat)
{

	m_width = width;
	m_height = height;

	// Load the texture
	m_texture.Load(directory + filename, true);

	m_directory = directory;
	m_filename = filename;

	// Set parameters for texturing using sampler object
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create a VBO
	m_vbo.Create();
	m_vbo.Bind();

	float halfWidth = m_width / 2.0f;
	float halfHeight = m_height / 2.0f;

	// Vertex positions
	glm::vec3 HouseVertices[34] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, halfHeight, 0.0f),
		glm::vec3(m_width, 0.0f, 0.0f),
		glm::vec3(m_width, halfHeight, 0.0f),

		glm::vec3(m_width, 0.0f, 0.0f),
		glm::vec3(m_width, halfHeight, 0.0f),
		glm::vec3(m_width, 0.0f, m_width),
		glm::vec3(m_width, halfHeight, m_width),

		glm::vec3(m_width, 0.0f, m_width),
		glm::vec3(m_width, halfHeight, m_width),
		glm::vec3(0.0f, 0.0f, m_width),
		glm::vec3(0.0f, halfHeight, m_width),

		glm::vec3(0.0f, 0.0f, m_width),
		glm::vec3(0.0f, halfHeight, m_width),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, halfHeight, 0.0f),

		glm::vec3(0.0f, halfHeight, 0.0f),
		glm::vec3(halfWidth, m_height, 0.0f),
		glm::vec3(m_width, halfHeight, 0.0f),

		glm::vec3(m_width, halfHeight, 0.0f),
		glm::vec3(halfWidth, m_height, 0.0f),
		glm::vec3(m_width, halfHeight, m_width),
		glm::vec3(halfWidth, m_height, m_width),

		glm::vec3(m_width, halfHeight, m_width),
		glm::vec3(halfWidth, m_height, m_width),
		glm::vec3(0.0f, halfHeight, m_width),

		glm::vec3(0.0f, halfHeight, m_width),
		glm::vec3(halfWidth, m_height, m_width),
		glm::vec3(0.0f, halfHeight, 0.0f),
		glm::vec3(halfWidth, m_height, 0.0f),

		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, m_width),
		glm::vec3(m_width, 0.0f, 0.0f),
		glm::vec3(m_width, 0.0f, m_width)
	};

	// Texture coordinates
	glm::vec2 HouseTexCoords[34] =
	{

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, textureRepeat),
		glm::vec2(textureRepeat, 0.0f),
		glm::vec2(textureRepeat, textureRepeat),

		glm::vec2(textureRepeat, 0.0f),
		glm::vec2(textureRepeat, textureRepeat),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, textureRepeat),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, textureRepeat),
		glm::vec2(textureRepeat, 0.0f),
		glm::vec2(textureRepeat, textureRepeat),

		glm::vec2(textureRepeat, 0.0f),
		glm::vec2(textureRepeat, textureRepeat),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, textureRepeat),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(textureRepeat / 2, textureRepeat),
		glm::vec2(textureRepeat, 0.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, textureRepeat),
		glm::vec2(textureRepeat, 0.0f),
		glm::vec2(textureRepeat, textureRepeat),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(textureRepeat / 2, textureRepeat),
		glm::vec2(textureRepeat, 0.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, textureRepeat),
		glm::vec2(textureRepeat, 0.0f),
		glm::vec2(textureRepeat, textureRepeat),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, textureRepeat),
		glm::vec2(textureRepeat, 0.0f),
		glm::vec2(textureRepeat, textureRepeat)
	};

	// Plane normal
	glm::vec3 HouseNormal[34] = {
		
		glm::normalize(HouseVertices[0]),
		glm::normalize(HouseVertices[1]),
		glm::normalize(HouseVertices[2]),
		glm::normalize(HouseVertices[3]),

		glm::normalize(HouseVertices[4]),
		glm::normalize(HouseVertices[5]),
		glm::normalize(HouseVertices[6]),
		glm::normalize(HouseVertices[7]),

		glm::normalize(HouseVertices[8]),
		glm::normalize(HouseVertices[9]),
		glm::normalize(HouseVertices[10]),
		glm::normalize(HouseVertices[11]),

		glm::normalize(HouseVertices[12]),
		glm::normalize(HouseVertices[13]),
		glm::normalize(HouseVertices[14]),
		glm::normalize(HouseVertices[15]),

		glm::normalize(HouseVertices[16]),
		glm::normalize(HouseVertices[17]),
		glm::normalize(HouseVertices[18]),

		glm::normalize(HouseVertices[19]),
		glm::normalize(HouseVertices[20]),
		glm::normalize(HouseVertices[21]),
		glm::normalize(HouseVertices[22]),

		glm::normalize(HouseVertices[23]),
		glm::normalize(HouseVertices[24]),
		glm::normalize(HouseVertices[25]),

		glm::normalize(HouseVertices[26]),
		glm::normalize(HouseVertices[27]),
		glm::normalize(HouseVertices[28]),
		glm::normalize(HouseVertices[29]),

		glm::normalize(HouseVertices[30]),
		glm::normalize(HouseVertices[31]),
		glm::normalize(HouseVertices[32]),
		glm::normalize(HouseVertices[33])

	};

	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < 34; i++) {
		m_vbo.AddData(&HouseVertices[i], sizeof(glm::vec3));
		m_vbo.AddData(&HouseTexCoords[i], sizeof(glm::vec2));
		m_vbo.AddData(&HouseNormal[i], sizeof(glm::vec3));
	}


	// Upload the VBO to the GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei istride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

}

// Render the plane as a triangle strip
void CHouse::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 16, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 19, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 23, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 26, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 30, 4);
}

// Release resources
void CHouse::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}