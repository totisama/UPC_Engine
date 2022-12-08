#pragma once
#include "GL/glew.h"
#include "Mesh.h"
#include "assimp/scene.h"
#include <list>
#include <vector>

using namespace std;

class Model
{
public:
	Model();
	~Model();

	void LoadModel(const char* file_name);
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(aiMesh** arrayMeshes, int totalMeshes);
	void DrawModel();
	void CalculateVertices();
	std::string GetModelName();
	vector<Mesh*> GetMeshes();
	vector<GLuint> GetMaterials();
	vector<std::string> GetMaterialsNames();

	float3 GetMaxVertex();
	void SetMaxVertex(float3 vertex);

	float3 GetMinVertex();
	void SetMinVertex(float3 vertex);
	
	float3 GetCenter();
	void SetCenter(float3 vertex);

private:
	vector<GLuint> materials;
	vector<std::string> materialsNames;
	vector<Mesh*> meshes;

	std::string modelName = "";
	std::string currentPath = "";

	float3 maxVertex = { 0, 0, 0 };
	float3 minVertex = { 0, 0, 0 };
	float3 centerVertex = { 0, 0, 0 };
};

