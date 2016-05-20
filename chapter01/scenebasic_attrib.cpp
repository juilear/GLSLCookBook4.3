#include "scenebasic_attrib.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;
#include <iostream>
using std::cerr;
using std::endl;

#include "glutils.h"

SceneBasic_Attrib::SceneBasic_Attrib() { }

void SceneBasic_Attrib::initScene()
{
	glDebugMessageCallback(GLUtils::debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, 
		GL_DEBUG_SEVERITY_NOTIFICATION, -1 , "Start debugging");

	compileShaderProgram();
	
	std::cout << std::endl;
	
	prog.printActiveAttribs();

    /////////////////// Create the VBO ////////////////////
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f };
    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };

    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color

//    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
//    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

//    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
//    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

	/*
	OpenGL 4.3
	implement the same functionality as in step 3 of the previous
	*/

	// Function:bind our two buffers to two different indexes
	// parameter1:specifies the index within the vertex buffer binding point(usually from 0 - 15)
	// parameter2:The second argument is the buffer to be bound
	// parameter3:the third is the offset from the beginning of the buffer to where the data begins
	// parameter4:the fourth is the stride, which is the distance between successive elements within the buffer
	//            Unlike glVertexAttribPointer, we can't use a 0 value here for tightly packed
	//            data, because OpenGL can't determine the size of the data without more information, so we
	//            need to specify it explicitly here.
	glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat)*3);
	glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat)*3);
	
	// Function:specify the format of the data for the attribute
	// The arguments are the same as the first four arguments to glVertexAttribPointer
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	// Function:specifies the relationship between buffers that 
	//          are bound to the vertex buffer binding point and attributes
	// parameter1:The first argument is the attribute location
	// parameter2:The second is the index within the vertex buffer binding point
	glVertexAttribBinding(0, 0);
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);    
    glVertexAttribBinding(1, 1);
    
    glBindVertexArray(0);
}

void SceneBasic_Attrib::compileShaderProgram()
{
	try {
		prog.compileShader("shader/basic.vert" );
		prog.compileShader("shader/basic.frag" );
		prog.link();
		prog.use();	
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Attrib::update( float t ) { }

void SceneBasic_Attrib::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );
}

void SceneBasic_Attrib::resize(int w, int h)
{
    glViewport(0,0,w,h);
}

