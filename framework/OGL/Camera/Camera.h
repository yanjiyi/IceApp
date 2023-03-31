#ifndef IFRAMEWORK_OGL_CAMERA_HEADER
#define IFRAMEWORK_OGL_CAMERA_HEADER

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	Camera(float pos_x, float pos_y, float pos_z, float front_x, float front_y, float front_z, float up_x, float up_y, float up_z);
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
	virtual ~Camera();

	void CameraForward(float deltaTime);
	void CameraBackward(float deltaTime);
	void CameraWardLeft(float deltaTime);
	void CameraWardRight(float deltaTime);
	void CameraJumpUp(float deltaTime);
	void SetCameraSpeed(float speed);
	void SetCameraDistance(float distance);
	void CameraMouseProcess(int xoffset,int yoffset,int preciseY,float deltaTime);
	void SetCameraMouseMode(bool surround = false);

	glm::vec3 GetCameraPos(void) const;
	void SetCameraPos(glm::vec3 pos);

	glm::vec3 GetCameraFront(void) const;
	void SetCameraFront(glm::vec3 front);

	glm::vec3 GetCameraUp(void) const;
	void SetCameraUp(glm::vec3 up);

	glm::mat4 GetCameraMatrix(void) const;

	void CameraUpdate(void);

protected:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float cameraSpeed = 5.5f;
	float sensitiviy = 0.1f;
	bool cameraSurround = false;

	float yaw = -90.0f;
	float pitch = 0.0f;
	float distance = 1.0f;
	glm::mat4 cameraMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	float pos_Range = 20.0f;
};


#endif
