#pragma once

namespace engine
{
	class mesh;

	class prism
	{
	public:
		prism(std::vector<glm::vec3> vertices);
		~prism();
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<prism> create(std::vector<glm::vec3> vertices);

	private:
		std::vector<glm::vec3> m_vertices;
		ref<engine::mesh> m_mesh;
	};
}
