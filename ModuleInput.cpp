#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "ModuleRender.h"
#include "ModuleRenderExercise.h"
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

    cameraRotationSpeed = App->editorCamera->GetCameraRotationSpeed();

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
            case SDL_DROPFILE:
                App->rendererExercise->resetAssimpLog();
                App->rendererExercise->pushAssimpLog("File dropped");
                App->rendererExercise->SetNewModel(sdlEvent.drop.file);
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
    float3 rotation(0.0f, 0.0f, 0.0f);

    //Duplicate speed
    if (keyboard[SDL_SCANCODE_LSHIFT])
    {
        cameraSpeed *= 2;
    }

    //move forward
    if (keyboard[SDL_SCANCODE_S])
    {
        position.z += cameraSpeed;
    }

    //move backward
    if (keyboard[SDL_SCANCODE_W])
    {   
        position.z -= cameraSpeed;
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

    //Pitch
    float cameraUp = App->editorCamera->GetCameraUp().y;
    if (keyboard[SDL_SCANCODE_UP])
    {
        rotation.y += cameraRotationSpeed;
    }
    if (keyboard[SDL_SCANCODE_DOWN] && cameraUp > 0.2)
    {
        rotation.y -= cameraRotationSpeed;
    }

    //Yaw
    if (keyboard[SDL_SCANCODE_RIGHT])
    {
        rotation.x -= cameraRotationSpeed;
    }
    if (keyboard[SDL_SCANCODE_LEFT])
    {
        rotation.x += cameraRotationSpeed;
    }
    App->editorCamera->Rotate(rotation);

    //Mouse wheel
    if (sdlEvent.type == SDL_MOUSEWHEEL)
    {
        float fov = App->editorCamera->GetFOV();
        if (sdlEvent.wheel.y > 0) // scroll up
        {
            if (fov > 0.5f)
            {
                fov -= 0.05;
            }
        }
        else if (sdlEvent.wheel.y < 0) // scroll down
        {
            if (fov < 2.0f)
            {
                fov += 0.05;
            }
        }
        App->editorCamera->SetFOV(fov);

        /*if (sdlEvent.wheel.x > 0) // scroll right
        {
            position.x += cameraSpeed * 1000;
        }
        else if (sdlEvent.wheel.x < 0) // scroll left
        {
            position.x -= cameraSpeed * 1000;
        }*/
    }

    //Mouse motion
    if (sdlEvent.type == SDL_MOUSEMOTION && sdlEvent.button.button == SDL_BUTTON_LEFT)
    {
        float cameraUp = App->editorCamera->GetCameraUp().y;
        //ENGINE_LOG("UP: %f", cameraUp);
        //Down motion
        if (sdlEvent.motion.yrel > 0 && cameraUp > 0.2)
        {
            rotation.y -= cameraRotationSpeed * 10;
        }
        //Up motion
        else if (sdlEvent.motion.yrel < 0 /* && cameraUp > 0.1*/)
        {
            rotation.y += cameraRotationSpeed * 10;
        }
        
        //Right motion
        if (sdlEvent.motion.xrel > 0)
        {
            rotation.x -= cameraRotationSpeed * 10;
        }
        //Left motion
        else if (sdlEvent.motion.xrel < 0)
        {
            rotation.x += cameraRotationSpeed * 10;
        }

        //Orbit
        if (keyboard[SDL_SCANCODE_LALT])
        {
            App->editorCamera->OrbitObject(rotation);
        }
        //Rotation
        else
        {
            App->editorCamera->Rotate(rotation);
        }
    }

    //Focus object
    if (keyboard[SDL_SCANCODE_F])
    {
        App->editorCamera->SetPositionAndRotationAccordingToModel();
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
