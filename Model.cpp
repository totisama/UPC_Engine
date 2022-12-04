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
			materials.push_back(App->texture->LoadTexture(file.data));
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
	for (std::list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->DrawMesh(materials);
	}
}