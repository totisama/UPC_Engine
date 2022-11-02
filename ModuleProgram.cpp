#include "ModuleProgram.h"

ModuleProgram::ModuleProgram()
{
}

// Destructor
ModuleProgram::~ModuleProgram()
{}

// Called before render is available
bool ModuleProgram::Init()
{
    return true;
}

// Called before quitting
bool ModuleProgram::CleanUp()
{
    return true;
}