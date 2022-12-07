#include "Mesh.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleRenderExercise.h"
#include "ModuleEditorCamera.h"
#include "GL/glew.h"
#include "Game/MathGeoLib_Source/Math/float2.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vao);

}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	
	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);
	
	unsigned uv_offset = position_size;
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));
	
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	num_vertices = mesh->mNumVertices;
	App->rendererExercise->pushAssimpLog("Vertex Buffer Object loaded");
}

void Mesh::LoadEBO(const aiMesh* mesh)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	num_indices = mesh->mNumFaces * 3;
	App->rendererExercise->pushAssimpLog("Element Buffer Object loaded");
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * num_vertices));
	App->rendererExercise->pushAssimpLog("Vertex Array Object created");
}

void Mesh::DrawMesh(const std::vector<unsigned>& model_textures)
{
	float4x4 view, proj;

	proj = App->editorCamera->GetProjectionMatrix();
	view = App->editorCamera->GetViewMatrix();
	unsigned program = App->renderer->defaultProgram;

	glUseProgram(program);

	glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &App->editorCamera->GetModelMatrix()[0][0]);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
}