#pragma once

namespace engine
{
	class mesh;

	class pentagon
	{
	public:
		pentagon(std::vector<glm::vec3> vertices);
		~pentagon();
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<pentagon> create(std::vector<glm::vec3> vertices);

	private:
		std::vector<glm::vec3> m_vertices;
		ref<engine::mesh> m_mesh;
	};
}
