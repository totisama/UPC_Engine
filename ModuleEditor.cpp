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

    return continueValue;
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
    static float cameraSpeed = App->editorCamera->GetCameraSpeed();
    static float fov = App->editorCamera->GetFOV();
    static float nearPlane = App->editorCamera->GetNearPlane();
    static float farPlane = App->editorCamera->GetFarPlane();
    float3 cameraPosition = App->editorCamera->GetCameraPos();
    static bool wireframeMode = false;

    if (fps.size() >= 1500) {
        fps.erase(fps.begin(), fps.begin() + 1000);
    }

    fps.emplace_back(ImGui::GetIO().Framerate);
    
    //ImGui::ShowDemoWindow();
    ImGui::Begin("Editor");
    if (ImGui::DragFloat("Camera Speed", &cameraSpeed, 0.0005f, 0.001f, 0.01f, "%.3f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetCameraSpeed(cameraSpeed);
    }
    if (ImGui::DragFloat("FOV", &fov, 0.05f, 0.5f, 2.0f, "%.2f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetFOV(fov);
    }
    if (ImGui::DragFloat("Near Plane", &nearPlane, 1.0f, 0.1f, 200.0f, "%.1f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetNearPlane(nearPlane);
    }
    if (ImGui::DragFloat("Far Plane", &farPlane, 1.0f, 0.1f, 400.0f, "%.1f", ImGuiSliderFlags_None))
    {
        App->editorCamera->SetFarPlane(farPlane);
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
    if (ImGui::CollapsingHeader("About"))
    {
        if (ImGui::Button("Exit"))
        {
            continueValue = UPDATE_STOP;
        }
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Visit the GitHub: https://github.com/totisama/UPC_Engine");
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "With this engine you can load FBXs files and\n"
            "move the camera around with the controllers"
        );
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Controllers");
        ImGui::Text("Escape: Quit Engine");
        ImGui::Text("W: Forward");
        ImGui::Text("S: Backward");
        ImGui::Text("A: Left");
        ImGui::Text("D: Right");
        ImGui::Text("Q: Up");
        ImGui::Text("E: Down");
        ImGui::Text("Shift: Double the moment speed");
        ImGui::Text("Left Click + Move Mouse: Rotate Camera");
        ImGui::Text("Up Arrow: Rotate Up");
        ImGui::Text("Down Arrow: Rotate Down");
        ImGui::Text("Left Arrow: Rotate Left");
        ImGui::Text("Right Arrow: Rotate Right");
        ImGui::Text("Left Alt + Left Click + Move Mouse: Orbit Model");
        ImGui::Text("Mouse Wheel: Zoom In & Out");
        ImGui::Text("F: Focus Model");
        ImGui::Text("Drag & Drop FBX: Load New Model");
    }
    if (ImGui::CollapsingHeader("Properties"))
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
    if (ImGui::CollapsingHeader("Add new model"))
    {
        ImGui::TextUnformatted("By selecting one of this buttons you can load a new Model.\n"
            "If you want, you can drag a FBX file to the window to load it too."
        );

        if (ImGui::Button("Load Baker House Model"))
        {
            App->rendererExercise->resetAssimpLog();
            App->rendererExercise->pushAssimpLog("Grass Block Model selected");
            App->rendererExercise->SetNewModel("./../Assets/Models/BakerHouse.fbx");
        }
        if (ImGui::Button("Load Grass Block Model"))
        {
            App->rendererExercise->resetAssimpLog();
            App->rendererExercise->pushAssimpLog("Grass Block Model selected");
            App->rendererExercise->SetNewModel("./../Assets/Models/Grass_block.fbx");
        }
        if (ImGui::Button("Load Watch Tower Model"))
        {
            App->rendererExercise->resetAssimpLog();
            App->rendererExercise->pushAssimpLog("Nations Watch Tower selected");
            App->rendererExercise->SetNewModel("./../Assets/Models/WatchTower.fbx");
        }
    }
    if (ImGui::CollapsingHeader("Configuration"))
    {
        SDL_version compiled;
        SDL_VERSION(&compiled);

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Platform: %s", SDL_GetPlatform());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "CPUs: %u", SDL_GetCPUCount());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "RAM: %u GBs", SDL_GetSystemRAM() / 1000);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "OpenGL version: %s", glGetString(GL_VERSION));
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SDL version: %u.%u.%u", compiled.major, compiled.minor, compiled.patch);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Vendor: %s", glGetString(GL_VENDOR));
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Renderer: %s", glGetString(GL_RENDERER));
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Frames Per Second");
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::PlotHistogram("", &fps[0], 1500, 0, "", 0.0f, 4000.0f, ImVec2(350.0f, 100.0f), 0);
    }
    if (ImGui::CollapsingHeader("Assimp logs"))
    {
        vector<const char*> logs = App->rendererExercise->getAssimpLogs();
        for (unsigned i = 0; i < logs.size(); ++i)
        {
            ImGui::Text(logs[i]);
        }
    }
    ImGui::End();
}