#include "Common.h"
#include "Pentagon.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


CPentagon::CPentagon()
{}

CPentagon::~CPentagon()
{}


// Create the plane, including its geometry, texture mapping, normal, and colour
void CPentagon::Create(string directory, string filename, float width, float height, float textureRepeat)
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
	glm::vec3 PentagonVertices[38] =
	{
		glm::vec3(0.0f, 0.0f, width*-1.0f),
		glm::vec3(width * -1, 0.0f, width*-0.3f),
		glm::vec3(0.0f, height, width*-1.0f),
		glm::vec3(width * -1, height, width*-0.3f),

		glm::vec3(width * -1.0f, 0.0f, width*-0.3f),
		glm::vec3(width * -0.6f, 0.0f, width * 0.8f),
		glm::vec3(width * -1.0f, height, width * -0.3f),
		glm::vec3(width * -0.6f, height, width * 0.8f),

		glm::vec3(width * -0.6f, 0.0f, width * 0.8f),
		glm::vec3(width * 0.6f, 0.0f, width * 0.8f),
		glm::vec3(width * -0.6f, height, width * 0.8f),
		glm::vec3(width * 0.6f, height, width * 0.8f),

		glm::vec3(width * 0.6f, 0.0f, width * 0.8f),
		glm::vec3(width * 1.0f, 0.0f, width * -0.3f),
		glm::vec3(width * 0.6f, height, width * 0.8f),
		glm::vec3(width * 1.0f, height, width * -0.3f),

		glm::vec3(width * 1.0f, 0.0f, width * -0.3f),
		glm::vec3(0.0f, 0.0f, width * -1.0f),
		glm::vec3(width * 1.0f, height, width * -0.3f),
		glm::vec3(0.0f, height, width * -1.0f),

		glm::vec3(width * -0.6f, 0.0f, width * 0.8f),
		glm::vec3(width * 0.6f, 0.0f, width * 0.8f),
		glm::vec3(width * 1.0f, 0.0f, width * -0.3f),

		glm::vec3(0.0f, 0.0f, width * -1.0f),
		glm::vec3(width * -1.0f, 0.0f, width * -0.3f),
		glm::vec3(width * -0.6f, 0.0f, width * 0.8f),

		glm::vec3(0.0f, 0.0f, width * -1.0f),
		glm::vec3(width * -0.6f, 0.0f, width * 0.8f),
		glm::vec3(width * 1.0f, 0.0f, width * -0.3f),

		glm::vec3(width * -0.6f, height, width * 0.8f),
		glm::vec3(width * 0.6f, height, width * 0.8f),
		glm::vec3(width * 1.0f, height, width * -0.3f),

		glm::vec3(0.0f, height, width * -1.0f),
		glm::vec3(width * -1.0f, height, width * -0.3f),
		glm::vec3(width * -0.6f, height, width * 0.8f),

		glm::vec3(0.0f, height, width * -1.0f),
		glm::vec3(width * -0.6f, height, width * 0.8f),
		glm::vec3(width * 1.0f, height, width * -0.3f)
	};

	// Texture coordinates
	glm::vec2 PentagonTexCoords[38] =
	{

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.2f, 0.9f),
		glm::vec2(0.8f, 0.9f),
		glm::vec2(1.0f, 0.35f),

		glm::vec2(0.5f, 0.0f),
		glm::vec2(0.0f, 0.35f),
		glm::vec2(0.2f, 0.9f),

		glm::vec2(0.5f, 0.0f),
		glm::vec2(0.2f, 0.9f),
		glm::vec2(1.0f, 0.35f),

		glm::vec2(0.2f, 0.9f),
		glm::vec2(0.8f, 0.9f),
		glm::vec2(1.0f, 0.35f),

		glm::vec2(0.5f, 0.0f),
		glm::vec2(0.0f, 0.35f),
		glm::vec2(0.2f, 0.9f),

		glm::vec2(0.5f, 0.0f),
		glm::vec2(0.2f, 0.9f),
		glm::vec2(1.0f, 0.35f)

	};

	// Plane normal
	glm::vec3 PentagonNormal[38] = {

		glm::normalize(PentagonVertices[0]),
		glm::normalize(PentagonVertices[1]),
		glm::normalize(PentagonVertices[2]),
		glm::normalize(PentagonVertices[3]),

		glm::normalize(PentagonVertices[4]),
		glm::normalize(PentagonVertices[5]),
		glm::normalize(PentagonVertices[6]),
		glm::normalize(PentagonVertices[7]),

		glm::normalize(PentagonVertices[8]),
		glm::normalize(PentagonVertices[9]),
		glm::normalize(PentagonVertices[10]),
		glm::normalize(PentagonVertices[11]),

		glm::normalize(PentagonVertices[12]),
		glm::normalize(PentagonVertices[13]),
		glm::normalize(PentagonVertices[14]),
		glm::normalize(PentagonVertices[15]),

		glm::normalize(PentagonVertices[16]),
		glm::normalize(PentagonVertices[17]),
		glm::normalize(PentagonVertices[18]),

		glm::normalize(PentagonVertices[19]),
		glm::normalize(PentagonVertices[20]),
		glm::normalize(PentagonVertices[21]),
		glm::normalize(PentagonVertices[22]),

		glm::normalize(PentagonVertices[23]),
		glm::normalize(PentagonVertices[24]),
		glm::normalize(PentagonVertices[25]),

		glm::normalize(PentagonVertices[26]),
		glm::normalize(PentagonVertices[27]),
		glm::normalize(PentagonVertices[28]),
		glm::normalize(PentagonVertices[29]),

		glm::normalize(PentagonVertices[30]),
		glm::normalize(PentagonVertices[31]),
		glm::normalize(PentagonVertices[32]),
		glm::normalize(PentagonVertices[33]),

		glm::normalize(PentagonVertices[34]),
		glm::normalize(PentagonVertices[35]),
		glm::normalize(PentagonVertices[36]),
		glm::normalize(PentagonVertices[37])

	};

	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < 38; i++) {
		m_vbo.AddData(&PentagonVertices[i], sizeof(glm::vec3));
		m_vbo.AddData(&PentagonTexCoords[i], sizeof(glm::vec2));
		m_vbo.AddData(&PentagonNormal[i], sizeof(glm::vec3));
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
void CPentagon::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 20, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 23, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 26, 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 29, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 32, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 35, 3);
}

// Release resources
void CPentagon::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}