#include "LightBase.h"

LightBase::LightBase()
{
	color = vec3(1, 1, 1);
}

LightBase::~LightBase()
{
}

void LightBase::Draw(Camera* cam)
{
	// slow code, but works for a gizmo so let it be :P

	// this tells openGL we dont want to use a shader
	glUseProgram(0);

	// send projection matrix to gpu
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*) &cam->GeneratePerspectiveProjectionMatrix());

	// send model view matrix to gpu
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 ModelView = cam->GenerateViewMatrix() * transform.GetModel();
	glLoadMatrixf((const GLfloat*) &ModelView[0]);


	// draw our gizmo
	glBegin(GL_LINES);
	{
		glm::vec3 p1 = this->transform.getPos();
		glm::vec3 p2 = p1;

		// set a colour
		glColor3f(1, 0, 0);
		// send a vertex position
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(1, 0, 0) * 0.1f;
		glColor3f(1, 0, 0);
		glVertex3fv(&p2.x);


		glColor3f(0, 1, 0);
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(0, 1, 0) * 0.1f;
		glColor3f(0, 1, 0);
		glVertex3fv(&p2.x);


		glColor3f(0, 0, 1);
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(0, 0, 1) * 0.1f;
		glColor3f(0, 0, 1);
		glVertex3fv(&p2.x);
	}
	glEnd();
}

Transform* LightBase::getTransform()
{
	return &transform;
}

vec3 LightBase::getColor()
{
	return color;
}
