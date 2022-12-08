#include "Model.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleRenderExercise.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

Model::Model()
{
}

Model::~Model()
{
}

void Model::LoadModel(const char* file_name)
{
	App->rendererExercise->pushAssimpLog("Loading Model...");
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		modelName = file_name;
		std::size_t lastIndex = modelName.find_last_of("/");

		if (lastIndex != std::string::npos)
		{
			currentPath = modelName.substr(0, lastIndex + 1);
			modelName = modelName.substr(lastIndex + 1);

			lastIndex = modelName.find_last_of(".");
			modelName = modelName.substr(0, lastIndex);
		} else {
			lastIndex = modelName.find_last_of("\\/");

			if (lastIndex != std::string::npos)
			{
				currentPath = modelName.substr(0, lastIndex + 1);
				modelName = modelName.substr(lastIndex + 1);

				lastIndex = modelName.find_last_of(".");
				modelName = modelName.substr(0, lastIndex);
			}
		}

		LoadMaterials(scene);
		LoadMeshes(scene->mMeshes, scene->mNumMeshes);
		App->rendererExercise->pushAssimpLog("Model loded!");
		App->rendererExercise->pushAssimpLog("");
	}
	else
	{
		App->rendererExercise->pushAssimpLog(("Error loading %s: %s", file_name, aiGetErrorString()));
		ENGINE_LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

void Model::LoadMaterials(const aiScene* scene)
{
	App->rendererExercise->pushAssimpLog("Loading Materials and Texture...");
	aiString file;

	materials.reserve(scene->mNumMaterials);
	
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			materials.push_back(App->texture->LoadTexture(file.data, currentPath));
			materialsNames.push_back((std::string)file.data);
		} else {
			ENGINE_LOG("Error loading the texture");
			App->rendererExercise->pushAssimpLog("Error loading the texture");
		}
	}
}

void Model::LoadMeshes(aiMesh** arrayMeshes, int totalMeshes)
{
	App->rendererExercise->pushAssimpLog("Loading Meshes...");
	for (int i = 0; i < totalMeshes; ++i) {
		const aiMesh* ai_mesh = arrayMeshes[i];
		Mesh* mesh = new Mesh();

		mesh->material_index = ai_mesh->mMaterialIndex;
		mesh->num_indices = ai_mesh->mNumFaces;
		mesh->LoadVBO(ai_mesh);
		mesh->LoadEBO(ai_mesh);
		mesh->CreateVAO();

		meshes.push_back(mesh);
	}
}

void Model::DrawModel()
{
	for (vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->DrawMesh(materials);
	}
}

std::string Model::GetModelName()
{
	return modelName;
}

vector<Mesh*> Model::GetMeshes()
{
	return meshes;
}

vector<GLuint> Model::GetMaterials()
{
	return materials;
}

vector<std::string> Model::GetMaterialsNames()
{
	return materialsNames;
}


float3 Model::GetMaxVertex()
{
	return maxVertex;
}

void Model::SetMaxVertex(float3 vertex)
{
	maxVertex = vertex;
}

float3 Model::GetMinVertex()
{
	return minVertex;
}

void Model::SetMinVertex(float3 vertex)
{
	minVertex = vertex;
}

float3 Model::GetCenter()
{
	return centerVertex;
}

void Model::SetCenter(float3 center)
{
	centerVertex = center;
}

void Model::CalculateVertices()
{
	vector<Mesh*> meshes = GetMeshes();

	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		float3 maxCurrentMeshVertex = meshes[i]->GetAABBMax();
		float3 minCurrentMeshVertex = meshes[i]->GetAABBMin();
		float3 currentMeshCenter = meshes[i]->GetAABBCenter();

		if (maxCurrentMeshVertex.x > maxVertex.x)
		{
			maxVertex.x = maxCurrentMeshVertex.x;
		}
		if (maxCurrentMeshVertex.y > maxVertex.y)
		{
			maxVertex.y = maxCurrentMeshVertex.y;
		}
		if (maxCurrentMeshVertex.z > maxVertex.z)
		{
			maxVertex.z = maxCurrentMeshVertex.z;
		}

		if (minCurrentMeshVertex.x < minVertex.x)
		{
			minVertex.x = minCurrentMeshVertex.x;
		}
		if (minCurrentMeshVertex.y > minVertex.y)
		{
			minVertex.y = minCurrentMeshVertex.y;
		}
		if (minCurrentMeshVertex.z > minVertex.z)
		{
			minVertex.z = minCurrentMeshVertex.z;
		}

		centerVertex += currentMeshCenter;
	}

	centerVertex /= meshes.size();
}