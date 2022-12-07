#include "ModuleTexture.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "GL/glew.h"
#include "DirectXTex/DirectXTex.h"
#include <string>

using namespace std;

ModuleTexture::ModuleTexture()
{
}

// Destructor
ModuleTexture::~ModuleTexture()
{
}

// Called before render is available
bool ModuleTexture::Init()
{
	return true;
}

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleTexture::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleTexture::CleanUp()
{
	return true;
}

void ModuleTexture::SetWireframeMode(bool setMode)
{
	if (setMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

GLuint ModuleTexture::LoadTexture(const char* fileName, string modelPath)
{
	DirectX::ScratchImage image = DirectX::ScratchImage();

	App->rendererExercise->pushAssimpLog(("Searching texture %s on:", fileName));
	App->rendererExercise->pushAssimpLog("FBX path...");

	HRESULT hResult = TestTexturePath(string(fileName), image);

	if (FAILED(hResult))
	{
		string texturePath;
		texturePath = modelPath + string(fileName);
		App->rendererExercise->pushAssimpLog("FBX folder...");
		hResult = TestTexturePath(texturePath, image);

		if (FAILED(hResult))
		{
			texturePath = "./../Assets/Textures/" + string(fileName);
			App->rendererExercise->pushAssimpLog("Texture folder...");
			hResult = TestTexturePath(texturePath, image);

			if (FAILED(hResult))
			{
				ENGINE_LOG("Error loading the texture %s ", fileName);
				App->rendererExercise->pushAssimpLog(("Error loading the texture %s ", fileName));

				return false;
			}
		}
	}

	App->rendererExercise->pushAssimpLog("Texture finded!");

	DirectX::ScratchImage* resultImage = new DirectX::ScratchImage();

	hResult = DirectX::FlipRotate(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, *resultImage);

	if (FAILED(hResult))
	{
		ENGINE_LOG("Error fliping the texture %s", fileName);
		App->rendererExercise->pushAssimpLog(("Error fliping the texture %s", fileName));

		return false;
	}
	
	GLuint texture;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, resultImage->GetMetadata().width, resultImage->GetMetadata().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, resultImage->GetPixels());
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

HRESULT ModuleTexture::TestTexturePath(string path, DirectX::ScratchImage& image)
{
	wstring wideString = wstring(path.begin(), path.end());
	const wchar_t* testPath = wideString.c_str();

	HRESULT hResult;
	hResult = DirectX::LoadFromDDSFile(testPath, DirectX::DDS_FLAGS_NONE, nullptr, image);

	if (FAILED(hResult))
	{
		hResult = DirectX::LoadFromTGAFile(testPath, DirectX::TGA_FLAGS_NONE, nullptr, image);

		if (FAILED(hResult))
		{
			hResult = DirectX::LoadFromWICFile(testPath, DirectX::WIC_FLAGS_NONE, nullptr, image);
		}
	}
	
	return hResult;
}