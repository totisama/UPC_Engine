#include "ModuleEditorCamera.h"
#include "Application.h"
#include "Game/MathGeoLib_Source/Geometry/Frustum.h"

ModuleEditorCamera::ModuleEditorCamera()
{
}

// Destructor
ModuleEditorCamera::~ModuleEditorCamera()
{}

// Called before render is available
bool ModuleEditorCamera::Init()
{
    return true;
}

// Called before quitting
bool ModuleEditorCamera::CleanUp()
{
    return true;
}

void ModuleEditorCamera::SetFOV()
{
}

void ModuleEditorCamera::SetAspectRatio()
{
}

void ModuleEditorCamera::SetPlaneDistances()
{
}

void ModuleEditorCamera::GetProjectionMatrix()
{
}

void ModuleEditorCamera::GetViewMatrix()
{
}