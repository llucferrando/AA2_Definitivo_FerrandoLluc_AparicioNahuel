#include "MyInputManager.h"

void MyInputManager::InputTransforms(GLFWwindow * window)
{
	key1Pressed = false;
	key2Pressed = false;
	key3Pressed = false;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && key1Pressed==false) {
		key1Pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && key2Pressed == false) {
		key2Pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && key3Pressed == false) {
		key3Pressed = true;
	}
	
}
