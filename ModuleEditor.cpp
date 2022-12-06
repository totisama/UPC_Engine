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

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiWindowFlags_AlwaysAutoResize;

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

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
    ShowWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    SDL_GL_DeleteContext(App->renderer->context);
    SDL_DestroyWindow(App->window->window);
    SDL_Quit();

    return true;
}

void ModuleEditor::ShowWindow()
{
    static float drag_f = App->editorCamera->GetCameraSpeed();
    float3 cameraPosition = App->editorCamera->GetCameraPos();
    vector<const char*> logs = App->rendererExercise->getAssimpLogs();
    static float xPosition = cameraPosition.x;
    static float yPosition = cameraPosition.y;
    static float zPosition = cameraPosition.z;
    static bool wireframeMode = false;

    SDL_version compiled;
    SDL_VERSION(&compiled);

    if (fps.size() >= 1500) {
        fps.erase(fps.begin(), fps.begin() + 1000);
    }

    fps.emplace_back(ImGui::GetIO().Framerate);
    
    //ImGui::ShowDemoWindow();
    ImGui::Begin("Editor");
    if (ImGui::DragFloat("Camera Speed", &drag_f, 0.0005f, 0.001f, 0.01f, "%.3f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraSpeed(drag_f);
    }
    ImGui::Text("");
    ImGui::Text("Camera position");
    if (ImGui::DragFloat("x", &xPosition, 0.05f, -100, 100, "%.3f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraPosition(float3(xPosition, yPosition, zPosition));
    }
    if (ImGui::DragFloat("y", &yPosition, 0.05f, -100, 100, "%.3f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraPosition(float3(xPosition, yPosition, zPosition));
    }
    if (ImGui::DragFloat("z", &zPosition, 0.05f, -100, 100, "%.3f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraPosition(float3(xPosition, yPosition, zPosition));
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
    if (ImGui::CollapsingHeader("Current Model"))
    {
        //ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Triangle count %u", 10);
        //ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), " texture size %u", 10);
    }
    if (ImGui::CollapsingHeader("About"))
    {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "OpenGL version %s", glGetString(GL_VERSION));
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SDL version %u.%u.%u", compiled.major, compiled.minor, compiled.patch);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Frames Per Second");
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::PlotHistogram("", &fps[0], 1500, 0, "", 0.0f, 5000.0f, ImVec2(350.0f, 100.0f), 0);
    }
    if (ImGui::CollapsingHeader("Assimp logs"))
    {
        for (unsigned i = 0; i < logs.size(); ++i)
        {
            ImGui::Text(logs[i]);
        }
    }
    ImGui::End();
}