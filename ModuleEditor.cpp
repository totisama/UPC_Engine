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
    /*ImGui::Text("Camera position");
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
    }*/
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
        vector<Mesh*> meshes = App->rendererExercise->GetCurrentModel()->GetMeshes();
        vector<GLuint> textures = App->rendererExercise->GetCurrentModel()->GetMaterials();
        vector<std::string> texturesNames = App->rendererExercise->GetCurrentModel()->GetMaterialsNames();

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Name: %s", App->rendererExercise->GetCurrentModel()->GetModelName().c_str());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Number of Meshes: %u", meshes.size());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Number of Textures: %u", textures.size());
        ImGui::Separator();

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Meshes:");
        for (int i = 0; i < meshes.size(); i++)
        {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Mesh %u", i+1);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Indices count: %u", meshes[i]->num_indices);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Vertices count: %u", meshes[i]->num_vertices);
            ImGui::Text("");
        }

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Textures:");
        for (int j = 0; j < textures.size(); j++)
        {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Texture ID-Name: %u - %s", textures[j], texturesNames[j].c_str());
            ImGui::Image((void*)(intptr_t)textures[j], ImVec2(75, 75));
            ImGui::Text("");
        }
    }
    if (ImGui::CollapsingHeader("About"))
    {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Platform: %s", SDL_GetPlatform());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "CPUs: %u", SDL_GetCPUCount());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "RAM: %u GB", SDL_GetSystemRAM()/1000);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "OpenGL version %s", glGetString(GL_VERSION));
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SDL version %u.%u.%u", compiled.major, compiled.minor, compiled.patch);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Frames Per Second");
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::PlotHistogram("", &fps[0], 1500, 0, "", 0.0f, 4000.0f, ImVec2(350.0f, 100.0f), 0);
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