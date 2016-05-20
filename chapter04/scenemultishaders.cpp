#include "scenemultishaders.h"

#include <cstdio>
#include <cstdlib>

#include "tgaio.h"

#include "glutils.h"

#include <iostream>
using std::endl;
using std::cerr;

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

SceneMultiShaders::SceneMultiShaders() { }

void SceneMultiShaders::initProgScene()
{
	compileAndLinkShader();

	teapot = new VBOTeapot(14, glm::mat4(1.0f));
	prog.setUniform("LightIntensity", vec3(0.9f, 0.9f, 0.9f));
}

void SceneMultiShaders::initProg2Scene()
{
	compileAndLinkShader2();

	plane = new VBOPlane(50.0f, 50.0f, 1, 1);
	prog2.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));

	glActiveTexture(GL_TEXTURE0);
	TGAIO::loadTex("../media/texture/brick1.tga");

	glActiveTexture(GL_TEXTURE1);
	TGAIO::loadTex("../media/texture/moss.tga");
}

void SceneMultiShaders::initScene()
{
	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(vec3(0.0f, 3.0f, 5.0f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);
	model = mat4(1.0f);
	initProgScene();
	initProg2Scene();
}

void SceneMultiShaders::update(float t) { }

void SceneMultiShaders::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.use();
	setMatrices();
	prog.setUniform("LightPosition", view * vec4(10.0f * cos(0.957283f), 3.0f, 10.0f * sin(0.957283f), 1.0f));
	prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
	prog.setUniform("Ks", 0.95f, 0.95f, 0.95f);
	prog.setUniform("Ka", 0.1f, 0.1f, 0.1f);
	prog.setUniform("Shininess", 100.0f);
	teapot->render();

	prog2.use();
	setMatrices2();
	prog2.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
	prog2.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
	prog2.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
	prog2.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
	prog2.setUniform("Material.Shininess", 100.0f);
	plane->render();
}

void SceneMultiShaders::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix",
		mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
}

void SceneMultiShaders::setMatrices2()
{
	mat4 mv = view * model;
	prog2.setUniform("ModelViewMatrix", mv);
	prog2.setUniform("NormalMatrix",
		mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog2.setUniform("MVP", projection * mv);
}

void SceneMultiShaders::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneMultiShaders::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/perfrag.vert", GLSLShader::VERTEX);
		prog.compileShader("shader/perfrag.frag", GLSLShader::FRAGMENT);
		prog.link();
		prog.use();
	}
	catch (GLSLProgramException & e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneMultiShaders::compileAndLinkShader2()
{
	try {
		prog2.compileShader("shader/multitex.vert", GLSLShader::VERTEX);
		prog2.compileShader("shader/multitex.frag", GLSLShader::FRAGMENT);
		prog2.link();
		prog2.use();
	}
	catch (GLSLProgramException & e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}
