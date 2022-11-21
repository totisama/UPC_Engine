#include "ModuleEditor.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleEditorCamera.h"
#include "ModuleRender.h"
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

    ShowWindow();

    // Console window
    //ImGui::TextUnformatted("Enable keyboard controls.");

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
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

    return true;
}

void ModuleEditor::ShowWindow()
{
    static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    static float drag_f = App->editorCamera->GetCameraSpeed();

    ImGui::ShowDemoWindow();
    ImGui::Begin("Window");
    bool resetCameraPosition = ImGui::Button("Reset position");
    ImGui::DragFloat("Camera Speed", &drag_f, 0.0005f, 0.001f, 0.1f, "%.3f", flags);
    ImGui::End();

    if (resetCameraPosition)
    {
        App->editorCamera->ResetCameraPosition();
    }
}