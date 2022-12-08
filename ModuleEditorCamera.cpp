#include "ModuleEditorCamera.h"
#include "ModuleRenderExercise.h"
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

float ModuleEditorCamera::GetFOV()
{
    return frustum.HorizontalFov();
}

void ModuleEditorCamera::SetFOV(float value)
{
    frustum.SetPerspective(value, frustum.VerticalFov());
}

float ModuleEditorCamera::GetNearPlane()
{
    return frustum.NearPlaneDistance();
}

void ModuleEditorCamera::SetNearPlane(float value)
{
    frustum.SetViewPlaneDistances(value, frustum.FarPlaneDistance());
}

float ModuleEditorCamera::GetFarPlane()
{
    return frustum.FarPlaneDistance();
}

void ModuleEditorCamera::SetFarPlane(float value)
{
    frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), value);
}

void ModuleEditorCamera::SetAspectRatio(float aspect)
{
    frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspect);
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
        float4x4::identity,
        float3(1.0f));
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
    frustum.SetHorizontalFovAndAspectRatio(DegToRad(90.0f), 1.3f);
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

void ModuleEditorCamera::SetCameraPosition(float3 position)
{
    frustum.SetPos(position);
}

float3 ModuleEditorCamera::GetCameraPos()
{
    return frustum.Pos();
}

void ModuleEditorCamera::SetPositionAndRotationAccordingToModel()
{
    float3 position, modelCenter, minVertex, maxVertex = {0, 0, 0};
    vector<Mesh*> meshes = App->rendererExercise->GetCurrentModel()->GetMeshes();

    for (unsigned i = 0; i < meshes.size(); ++i)
    {
        float3 maxCurrentMeshVertex = meshes[i]->GetAABBMax();
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

        modelCenter += currentMeshCenter;
    }

    modelCenter /= meshes.size();
    position.Set(maxVertex.x + 5, maxVertex.y + 5, maxVertex.z + 5);

    SetCameraPosition(position);
    LookAt(modelCenter);
}

void ModuleEditorCamera::LookAt(float3 direction)
{
    direction.Normalize();

    float3 localForward = frustum.Front().Normalized();
    float3 localUp = frustum.Up().Normalized();
    float3x3 lookAt = float3x3::LookAt(localForward, direction, localUp, float3::unitY);

    frustum.SetFront(lookAt * localForward);
    frustum.SetUp(lookAt * localUp);
}