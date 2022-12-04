#include "ModuleEditor.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleEditorCamera.h"
#include "ModuleRender.h"
#include "ModuleRenderExercise.h"
#include "ModuleTexture.h"
#include <GL/glew.h>

ModuleEditor::ModuleEditor()
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{}

// Called before render is available
bool ModuleEditor::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    return true;
}

update_status ModuleEditor::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    // Console window
    //ImGui::TextUnformatted("Enable keyboard controls.");

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
    ShowWindow();
    ShowAssimpLogsWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //SDL_GL_SwapWindow(App->window->window);

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
    ImGui::EndFrame();
    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_Quit();

    return true;
}

void ModuleEditor::ShowWindow()
{
    static float drag_f = App->editorCamera->GetCameraSpeed();
    float3 cameraPosition = App->editorCamera->GetCameraPos();
    static int xPosition = cameraPosition.x;
    static int yPosition = cameraPosition.y;
    static int zPosition = cameraPosition.z;
    static bool wireframeMode = false;

    //ImGui::ShowDemoWindow();
    ImGui::Begin("Editor");
    if (ImGui::DragFloat("Camera Speed", &drag_f, 0.0005f, 0.001f, 0.01f, "%.3f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraSpeed(drag_f);
    }
    ImGui::Text("");
    ImGui::Text("Camera position");
    if (ImGui::DragInt("x", &xPosition, 0.05f, -100, 100, "%d", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraPos(float3(xPosition, yPosition, zPosition));
    }
    if (ImGui::DragInt("y", &yPosition, 0.05f, -100, 100, "%d", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraPos(float3(xPosition, yPosition, zPosition));
    }
    if (ImGui::DragInt("z", &zPosition, 0.05f, -100, 100, "%d", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraPos(float3(xPosition, yPosition, zPosition));
    }
    if (ImGui::Button("Reset camera"))
    {
        App->editorCamera->ResetCameraPosition();
        App->editorCamera->ResetCameraRotation();
    }
    if (ImGui::Checkbox("Wireframe mode", &wireframeMode))
    {
        App->texture->SetWireframeMode(wireframeMode);
    }
    if (ImGui::Button("Reload model"))
    {
        App->rendererExercise->SetNewModel("");
    }
    ImGui::End();
}

void ModuleEditor::ShowAssimpLogsWindow()
{
    vector<const char*> logs = App->rendererExercise->getAssimpLogs();

    ImGui::Begin("Assimp logs");
    for (unsigned i = 0; i < logs.size(); ++i)
    {
        ImGui::Text(logs[i]);
    }
    ImGui::End();
}