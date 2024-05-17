#include "Camera.h"


Camera::Camera() : _fFov(45.0f), _aspectRatio(1.0), _fNear(0.1), _fFar(100.0),
lookAtMid(false),lookAtTroll1(false), lookAtTroll2(false), lookAtTroll3(false)
{
	_position = { 0.f,2.f,0.f };
	_rotation = { 0.f,0.f,0.f };
	_localVectorUp = { 0.f,0.f,1.f };


}

void Camera::Update()
{
	const float radius = 15.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	LookAt();
	switch (myCameraState) {
		case Orbit:
			lookAtMid = true;
			_localVectorUp = { 0.f,1.f,0.f };
			_position.x = camX;
			_position.z = camZ;
			_rotation.x = 15.f;

			return;
		case FocusTroll1:
			lookAtTroll1 = true;
			_localVectorUp = { 0.f,1.f,0.f };
			_rotation = { 0.f,0.f,0.f };
			_position = { 0.f,1.35f,3.f };
			return;
		case FocusTroll2:
			lookAtTroll2 = true;
			_localVectorUp = { 0.f,1.f,0.f };
			_rotation = { 0.f,0.f,0.f };
			_position = { 1.85f,1.85f,3.22f };
			return;

		case FocusTroll3:
			lookAtTroll3 = true;
			_localVectorUp = { 0.f,1.f,0.f };
			_rotation = { 0.f,0.f,0.f };
			_position = { 0.f,1.35f,3.f };
			_fFov += 0.001f; // Adjust FOV to zoom out (increase for faster effect)
			_position.z += .1f; // Move the camera forward
	
			return;
	}
}

void Camera::LookAt()
{
	glm::vec3 targetPos;
	if (lookAtMid) {
		targetPos = { 0.f,0.f,0.f };
		lookAtMid = false;
	
	}
	if (lookAtTroll1) {
		targetPos = { -3.f,1.35f,3.f };
		lookAtTroll1 = false;
	}
	if (lookAtTroll2) {
		targetPos = { 3.f,2.f,3.f };
		lookAtTroll2 = false;
	}

	if (lookAtTroll3) {
		targetPos = { 0.f,1.35f,6.f };
		lookAtTroll3 = false;
	}
	_viewMatrix = glm::lookAt(_position, targetPos, _localVectorUp);
	MatrixView(_viewMatrix);
}

glm::mat4 Camera::MatrixView(glm::mat4 viewMat)
{
	return viewMat;
}
