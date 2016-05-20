#ifndef SCENEMULTISHADERS_H
#define SCENEMULTISHADERS_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboteapot.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneMultiShaders : public Scene
{
private:
	GLSLProgram prog;     // ²èºø ¹âÕÕ
	GLSLProgram prog2;    // Æ½Ãæ ÌùÍ¼

	int width, height;
	VBOPlane *plane;
	VBOTeapot *teapot;

	mat4 model;
	mat4 view;
	mat4 projection;

	void initProgScene();
	void initProg2Scene();
	void setMatrices();
	void setMatrices2();
	void compileAndLinkShader();
	void compileAndLinkShader2();

public:
	SceneMultiShaders();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};

#endif // SCENEMULTISHADERS_H
