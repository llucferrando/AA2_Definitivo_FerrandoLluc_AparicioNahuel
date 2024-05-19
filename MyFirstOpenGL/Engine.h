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
	//Singleton to call from everywhere
	static Engine& getInstance() { static Engine instance; return instance; }
	Engine() = default;

	//Functions utils
	Model* LoadOBJModel(int IDProgram, const std::string& filePath, const char* texturefilePath, GLenum textureUnit, ModelType type);
	void Init();
	void Update(GLFWwindow* window);
	void InputTransforms(GLFWwindow* window);

	//Getters
	//float getDeltaTime() const {};
	bool getKey1Pressed() const { return key1Pressed; }
	bool getKey2Pressed() const { return key2Pressed; }
	bool getKey3Pressed() const { return key3Pressed; }


	
private:
	//Input variables
	bool key1Pressed;
	bool key2Pressed;
	bool key3Pressed;
	//Time variables
	float deltaTime;
	 float lastFrameTime;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};

