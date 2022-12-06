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
	std::string GetModelName();
	vector<Mesh*> GetMeshes();
	vector<GLuint> GetMaterials();
	vector<std::string> GetMaterialsNames();

private:
	vector<GLuint> materials;
	vector<std::string> materialsNames;
	vector<Mesh*> meshes;

	std::string modelName = "";
};

