#pragma once
#include <assimp/scene.h>
#include <vector>


class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void DrawMesh(const std::vector<unsigned>& model_textures);

	int num_vertices, num_indices, material_index;

private:
	unsigned int vbo, ebo, vao;
};

