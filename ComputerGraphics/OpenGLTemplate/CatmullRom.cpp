#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

	return a + b * t + c * t2 + d * t3;

}


void CCatmullRom::SetControlPoints() {

	/*
	m_controlPoints.push_back(glm::vec3(100, 1, 0));
	m_controlPoints.push_back(glm::vec3(71, 1, 71));
	m_controlPoints.push_back(glm::vec3(0, 1, 100));
	m_controlPoints.push_back(glm::vec3(-71, 1, 71));
	m_controlPoints.push_back(glm::vec3(-100, 1, 0));
	m_controlPoints.push_back(glm::vec3(-71, 1, -71));
	m_controlPoints.push_back(glm::vec3(0, 1, -100));
	m_controlPoints.push_back(glm::vec3(71, 1, -71));
	*/

	m_controlPoints.push_back(glm::vec3(100, 10.5, 0));
	m_controlPoints.push_back(glm::vec3(-42, 10.5, 142));
	m_controlPoints.push_back(glm::vec3(100, 10.5, 184));
	m_controlPoints.push_back(glm::vec3(242, 10.0, 126));
	m_controlPoints.push_back(glm::vec3(300, 55.0, -16));
	m_controlPoints.push_back(glm::vec3(-242, 25.0, -158));
	m_controlPoints.push_back(glm::vec3(100, 15.0, -216));
	m_controlPoints.push_back(glm::vec3(42, 1.5, -126));

}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3& p, glm::vec3& up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size() - 1; i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));

	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}


void CCatmullRom::CreateCentreline()
{
	SetControlPoints();

	UniformlySampleControlPoints(500);

	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < m_centrelinePoints.size() - 1; i++) {
		vbo.AddData(&m_centrelinePoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	vbo.AddData(&m_centrelinePoints[0], sizeof(glm::vec3));
	vbo.AddData(&texCoord, sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves()
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.

	
	glm::vec3 n(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < m_centrelinePoints.size() -1; i++) {
		glm::vec3 p = m_centrelinePoints.at(i);
		glm::vec3 pNext = m_centrelinePoints.at(i + 1);

		glm::vec3 T = glm::normalize(pNext - p);
		glm::vec3 N = glm::normalize(glm::cross(T,glm::vec3(0.0f,1.0f,0.0f)));
		glm::vec3 B = glm::normalize(glm::cross(N,T));


		m_leftOffsetPoints.push_back(p- 10.0f*N);

		m_rightOffsetPoints.push_back(p+10.0f*N);

	}

	
	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
	glBindVertexArray(m_vaoLeftOffsetCurve);

	CVertexBufferObject Lvbo;
	Lvbo.Create();
	Lvbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < m_leftOffsetPoints.size() - 1; i++) {
		Lvbo.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
		Lvbo.AddData(&texCoord, sizeof(glm::vec2));
		Lvbo.AddData(&normal, sizeof(glm::vec3));
	}
	Lvbo.AddData(&m_leftOffsetPoints[0], sizeof(glm::vec3));
	Lvbo.AddData(&texCoord, sizeof(glm::vec2));
	Lvbo.AddData(&normal, sizeof(glm::vec3));

	// Upload the VBO to the GPU
	Lvbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


	glGenVertexArrays(1, &m_vaoRightOffsetCurve);
	glBindVertexArray(m_vaoRightOffsetCurve);

	CVertexBufferObject Rvbo;
	Rvbo.Create();
	Rvbo.Bind();

	for (unsigned int i = 0; i < m_rightOffsetPoints.size() - 1; i++) {
		Rvbo.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
		Rvbo.AddData(&texCoord, sizeof(glm::vec2));
		Rvbo.AddData(&normal, sizeof(glm::vec3));
	}
	Rvbo.AddData(&m_rightOffsetPoints[0], sizeof(glm::vec3));
	Rvbo.AddData(&texCoord, sizeof(glm::vec2));
	Rvbo.AddData(&normal, sizeof(glm::vec3));

	// Upload the VBO to the GPU
	Rvbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	
}


void CCatmullRom::CreateTrack() {

	std::string m_directory = "resources\\textures\\road.jpg";

	m_texture.Load(m_directory);

	// Set parameters for texturing using sampler object
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec2 texCoord1(0.0f, 0.0f);
	glm::vec2 texCoord2(1.0f, 0.0f);
	glm::vec2 texCoord3(0.0f, 0.1f);
	glm::vec2 texCoord4(1.0f, 0.1f);


	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < m_centrelinePoints.size() - 2; i++) {
		vbo.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord1, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		vbo.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord2, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		vbo.AddData(&m_leftOffsetPoints[i+1], sizeof(glm::vec3));
		vbo.AddData(&texCoord3, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		vbo.AddData(&m_rightOffsetPoints[i+1], sizeof(glm::vec3));
		vbo.AddData(&texCoord4, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		m_vertexCount += 4;

		texCoord1 += glm::vec2(0.0f, 0.01f);
		texCoord2 += glm::vec2(0.0f, 0.01f);
		texCoord3 += glm::vec2(0.0f, 0.01f);
		texCoord4 += glm::vec2(0.0f, 0.01f);
	}

	vbo.AddData(&m_leftOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&texCoord1, sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

	vbo.AddData(&m_rightOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&texCoord2, sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

	m_vertexCount += 2;

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

}


void CCatmullRom::RenderCentreline()
{
	glLineWidth(10.0f);
	glPointSize(5.0);
	glBindVertexArray(m_vaoCentreline);
	glDrawArrays(GL_POINTS, 0, int(m_centrelinePoints.size()));
	glDrawArrays(GL_LINE_LOOP, 0, int(m_centrelinePoints.size()));

}

void CCatmullRom::RenderOffsetCurves()
{
	// Bind the VAO m_vaoLeftOffsetCurve and render it

	glLineWidth(10.0f);
	glPointSize(5.0);
	glBindVertexArray(m_vaoLeftOffsetCurve);
	glDrawArrays(GL_POINTS, 0, int(m_leftOffsetPoints.size()));
	glDrawArrays(GL_LINE_LOOP, 0, int(m_leftOffsetPoints.size()));

	// Bind the VAO m_vaoRightOffsetCurve and render it

	glLineWidth(10.0f);
	glPointSize(5.0);
	glBindVertexArray(m_vaoRightOffsetCurve);
	glDrawArrays(GL_POINTS, 0, int(m_rightOffsetPoints.size()));
	glDrawArrays(GL_LINE_LOOP, 0, int(m_rightOffsetPoints.size()));
}


void CCatmullRom::RenderTrack()
{
	// Bind the VAO m_vaoTrack and render it
	//glEnable(GLU_CULLING);

	glBindVertexArray(m_vaoTrack);
	m_texture.Bind();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);

}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}

glm::vec3 CCatmullRom::_dummy_vector(0.0f, 0.0f, 0.0f);