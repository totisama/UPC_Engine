#pragma once
#include "Module.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	bool CleanUp();

	unsigned CreateTriangleVBO();
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	unsigned CompileShader(unsigned type, const char* source);
	void RenderVBO(unsigned vbo, unsigned program);
	char* LoadShaderSource(const char* shader_file_name);

};

