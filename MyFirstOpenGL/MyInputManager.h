#pragma once
#include "Engine.h"
#include "Camera.h"
class MyInputManager : public Engine
{
public:
	static MyInputManager& getInstance() { static MyInputManager instance; return instance; }
	MyInputManager() = default;

	void InputTransforms(GLFWwindow * window);

	//Getters
	bool getKey1Pressed() const { return key1Pressed; }
	bool getKey2Pressed() const { return key2Pressed; }
	bool getKey3Pressed() const { return key3Pressed; }

private:
	bool key1Pressed;
	bool key2Pressed;
	bool key3Pressed;
	MyInputManager(const MyInputManager&) = delete;
	MyInputManager& operator=(const MyInputManager&) = delete;
};

