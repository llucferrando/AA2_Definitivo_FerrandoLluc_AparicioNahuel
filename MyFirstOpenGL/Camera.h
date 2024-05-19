
#pragma once
#include "GameObject.h"


extern class Engine;
 
enum CameraState { Orbit = 0, FocusTroll1 = 1, FocusTroll2 = 2, FocusTroll3 = 3 };
class Camera : public GameObject
{
public:
	static Camera& getInstance() { static Camera instance; return instance; }
	Camera();


	//Getters
	float getfFov() const { return _fFov; }
	float getAspectRatio() const { return _aspectRatio; }
	float getfNear() const { return _fNear; }
	float getfFar() const { return _fFar; }
	glm::mat4 getViewMatrix() const { return _viewMatrix; }
	

	//Setters
	void setfFov(float fFov) { _fFov = fFov; }
	void setaspectRatio(float aspectRatio) { _aspectRatio = aspectRatio; }
	void setfNear(float fNear) { _fNear = fNear; }
	void set_fFar(float fFar) { _fFar = fFar; }
	void setOrbit(CameraState state) { myCameraState = state; }
	

	void Update();
	void LookAt();
	void DollyEffect();
	glm::mat4 MatrixView(glm::mat4 viewMat);

private:
	Camera(const Camera&) = delete;
	void operator=(const Camera&) = delete;

	float _fFov;
	float _aspectRatio;
	float _fNear;
	float _fFar;

	bool lookAtMid;
	bool lookAtTroll1;
	bool lookAtTroll2;
	bool lookAtTroll3;
	bool spawnedCamera;
	glm::mat4 _viewMatrix;
	CameraState myCameraState;
	
};



