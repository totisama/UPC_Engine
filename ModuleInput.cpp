#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"
#include "imgui_impl_sdl.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	ENGINE_LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		ENGINE_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    //Quit
    if (keyboard[SDL_SCANCODE_ESCAPE])
    {
        return UPDATE_STOP;
    }

    float cameraSpeed = App->editorCamera->GetCameraSpeed();
    float3 position(0.0f, 0.0f, 0.0f);

    //move backward
    if (keyboard[SDL_SCANCODE_W])
    {   
        position.z -= cameraSpeed;
    }

    //move forward
    if (keyboard[SDL_SCANCODE_S])
    {
        position.z += cameraSpeed;
    }

    //Move right
    if (keyboard[SDL_SCANCODE_D])
    {
        position.x += cameraSpeed;
    }

    //Move left
    if (keyboard[SDL_SCANCODE_A])
    {
        position.x -= cameraSpeed;
    }

    //Move up
    if (keyboard[SDL_SCANCODE_Q])
    {
        position.y += cameraSpeed;
    }

    //Move down
    if (keyboard[SDL_SCANCODE_E])
    {
        position.y -= cameraSpeed;
    }

    App->editorCamera->Translate(position);

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	ENGINE_LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
