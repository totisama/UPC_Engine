#pragma once
#include <assimp/scene.h>
#include <vector>
#include "Game/MathGeoLib_Source/Math/float3.h"
#include "Game/MathGeoLib_Source/Geometry/AABB.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void DrawMesh(const std::vector<unsigned>& model_textures);

	float3 GetAABBMin();
	float3 GetAABBMax();
	float3 GetAABBCenter();

	int num_vertices, num_indices, material_index;

private:
	unsigned int vbo, ebo, vao;
	AABB *aabb = new AABB;
};

