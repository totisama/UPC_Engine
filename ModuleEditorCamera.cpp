#include "ModuleEditorCamera.h"
#include "Application.h"
#include "Game/MathGeoLib_Source/Geometry/Frustum.h"
#include "Game/MathGeoLib_Source/Math/Quat.h"

ModuleEditorCamera::ModuleEditorCamera()
{
}

// Destructor
ModuleEditorCamera::~ModuleEditorCamera()
{}

// Called before render is available
bool ModuleEditorCamera::Init()
{
    frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
    frustum.SetViewPlaneDistances(0.1f, 200.0f);
    frustum.SetHorizontalFovAndAspectRatio(DegToRad(90.0f), 1.3f);

    frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
    frustum.SetFront(-float3::unitZ);
    frustum.SetUp(float3::unitY);

    return true;
}

// Called before quitting
bool ModuleEditorCamera::CleanUp()
{
    return true;
}

void ModuleEditorCamera::SetFOV(float valor = 90.0f)
{
    frustum.SetHorizontalFovAndAspectRatio(DegToRad(valor), 1.3f);
}

void ModuleEditorCamera::SetAspectRatio()
{
}

void ModuleEditorCamera::SetPlaneDistances()
{
}

float4x4 ModuleEditorCamera::GetProjectionMatrix()
{
    return frustum.ProjectionMatrix();
}

float4x4 ModuleEditorCamera::GetViewMatrix()
{
    return frustum.ViewMatrix();
}

float4x4 ModuleEditorCamera::GetModelMatrix()
{
    return float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
        float4x4::RotateZ(math::pi / 4.0f),
        float3(2.0f, 1.0f, 0.0f));
}

void ModuleEditorCamera::Translate(float3 position)
{
    frustum.SetPos(frustum.Pos() + float4x4(frustum.WorldMatrix()).Float3x3Part().MulDir(position));
}

void ModuleEditorCamera::Rotate(float3 rotation)
{
    Quat yaw = Quat::RotateY(rotation.x);
    float3 right = yaw * frustum.WorldRight();

    Quat pitch = Quat::RotateAxisAngle(right, rotation.y);
    float3 up = pitch * yaw * frustum.Up();
    float3 front = pitch * yaw * frustum.Front();

    frustum.SetUp(up);
    frustum.SetFront(front);
}

void ModuleEditorCamera::ResetCameraPosition()
{
    frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
}

void ModuleEditorCamera::ResetCameraRotation()
{
    frustum.SetFront(-float3::unitZ);
    frustum.SetUp(float3::unitY);
}

float3 ModuleEditorCamera::GetCameraUp()
{
    return frustum.Up();
}

float ModuleEditorCamera::GetCameraRotationSpeed()
{
    return cameraRotationSpeed;
}

void ModuleEditorCamera::SetCameraRotationSpeed(float value)
{
    cameraRotationSpeed = value;
}

float ModuleEditorCamera::GetCameraSpeed()
{
    return cameraSpeed;
}

void ModuleEditorCamera::SetCameraSpeed(float value)
{
    cameraSpeed = value;
}

void ModuleEditorCamera::SetCameraPos(float3 position)
{
    return frustum.SetPos(position);
}

float3 ModuleEditorCamera::GetCameraPos()
{
    return frustum.Pos();
}