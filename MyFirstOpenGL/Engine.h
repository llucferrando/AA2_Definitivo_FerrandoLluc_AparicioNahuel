#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"
#include <iostream>

class Engine
{
public:
	static Engine& getInstance() { static Engine instance; return instance; }
	Engine() = default;

	Model* LoadOBJModel(int IDProgram, const std::string& filePath, const char* texturefilePath, GLenum textureUnit, ModelType type);
	
private:
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};

