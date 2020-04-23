#include "Camera.h"

void Camera::Translate(vec3 direction)
{
	m_target += direction;

	m_transform.Translate(direction);
}

mat4 Camera::GenerateViewMatrix()
{
	xTheta = fmod(xTheta + (targetDeltaX * sensitivity), 360.0f);
	float xVal = xTheta * (M_PI / 180.0f);

	yTheta = fmod(yTheta + (targetDeltaY * sensitivity), 360.0f);
	float yVal = yTheta * (M_PI / 180.0f);

	m_target.x = sin(xVal);
	m_target.y = -cos(yVal);
	m_target.z = cos(xVal) + sin(yVal);
	//m_target.y += targetDeltaY * sensitivity;
	forwardTarget = normalize(m_target);
	Forward = normalize(m_target - m_transform.getPos());


	//std::cout << "TargetX" << xVal << std::endl;
	//std::cout << "xTheta" << xTheta << std::endl;


	// reset delta because it floats
	targetDeltaY = 0;
	targetDeltaX = 0;

	Right = normalize(cross(vec3(0, 1, 0), Forward));
	Up = cross(Forward, Right);

	ViewMatrix = glm::lookAt(m_transform.getPos(), m_transform.getPos() + forwardTarget, Up);

	//std::cout << "Target: " << m_target.x << " " << m_target.y << " " << m_target.z << std::endl;
	//std::cout << "Position: " << m_transform.getPos().x << " " << m_transform.getPos().y << " " << m_transform.getPos().z << std::endl;

	//std::cout << targetDeltaX <<   << targetDeltaY << std::endl;

	return ViewMatrix;

	//m_target = Forward;
}

mat4 Camera::GeneratePerspectiveProjectionMatrix()
{
	PerspectiveProjection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	return PerspectiveProjection;
}
