#include "scenebasic.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>
using std::ifstream;
#include <sstream>
#include <string>
using std::string;

#include "glutils.h"

SceneBasic::SceneBasic() { }

void SceneBasic::initScene()
{
    //////////////////////////////////////////////////////
    /////////// Vertex shader //////////////////////////
    //////////////////////////////////////////////////////

    // Load contents of file
    ifstream inFile( "shader/basic.vert" );
    if (!inFile) {
        fprintf(stderr, "Error opening file: shader/basic.vert\n" );
        exit(1);
    }

    std::stringstream code;
    code << inFile.rdbuf();
	inFile.close();
	string codeStr(code.str());

    // Create the shader object
    GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
    if (0 == vertShader) {
      fprintf(stderr, "Error creating vertex shader.\n");
      exit(EXIT_FAILURE);
    }

    // Load the source code into the shader object
    const GLchar* codeArray[] = {codeStr.c_str()};
    glShaderSource(vertShader, 1, codeArray, NULL);

    // Compile the shader
    glCompileShader( vertShader );

    // Check compilation status
    GLint result;
    glGetShaderiv( vertShader, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ) {
       fprintf( stderr, "Vertex shader compilation failed!\n" );

       GLint logLen;
       glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &logLen );

       if (logLen > 0) {
           char * log = (char *)malloc(logLen);

           GLsizei written;
           glGetShaderInfoLog(vertShader, logLen, &written, log);

           fprintf(stderr, "Shader log: \n%s", log);

           free(log);
       }
    }

    //////////////////////////////////////////////////////
    /////////// Fragment shader //////////////////////////
    //////////////////////////////////////////////////////

    // Load contents of file into shaderCode here
    ifstream fragFile( "shader/basic.frag" );
    if (!fragFile) {
        fprintf(stderr, "Error opening file: shader/basic.frag\n" );
        exit(1);
    }

    std::stringstream fragCode;
    fragCode << fragFile.rdbuf();
	fragFile.close();
	codeStr = fragCode.str();

    // Create the shader object
    GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
    if (0 == fragShader) {
      fprintf(stderr, "Error creating fragment shader.\n");
      exit(1);
    }

    // Load the source code into the shader object
    codeArray[0] = codeStr.c_str();
    glShaderSource( fragShader, 1, codeArray, NULL );

    // Compile the shader
    glCompileShader( fragShader );

    // Check compilation status
    glGetShaderiv( fragShader, GL_COMPILE_STATUS, &result );
    if (GL_FALSE == result) {
       fprintf( stderr, "Fragment shader compilation failed!\n" );

       GLint logLen;
       glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &logLen );

       if (logLen > 0) {
           char * log = (char *)malloc(logLen);

           GLsizei written;
           glGetShaderInfoLog(fragShader, logLen, &written, log);

           fprintf(stderr, "Shader log: \n%s", log);

           free(log);
       }
    }

    linkMe(vertShader, fragShader);

    /////////////////// Create the VBO ////////////////////
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f };
    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };

	/*
	To summarize, we followed these steps:
	1. Make sure to specify the generic vertex attribute indexes for each attribute in the
	vertex shader using the layout qualifier.
	2. Create and populate the buffer objects for each attribute.
	3. Create and define the vertex array object by calling glVertexAttribPointer while
	the appropriate buffer is bound.
	4. When rendering, bind to the vertex array object and call glDrawArrays, or other
	appropriate rendering function (e.g. glDrawElements).
	*/

	// Vertex attributes are the input variables to our vertex shader
	// Vertex attributes' indices are simply integers between 0 and GL_MAX_VERTEX_ATTRIBS¨C 1.

/////////// Step 2 ////////////
    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

	// Function: bind the buffer object to the GL_ARRAY_BUFFER binding point
	// parameter 1£ºthe target binding point(such as GL_ARRAY_BUFFER,GL_UNIFORM_BUFFER, or GL_ELEMENT_ARRAY_BUFFER)
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	// Function: populate the buffer with our data
	// parameter 1: The first argument indicates the target buffer object
	// parameter 2:the size of the array
	// parameter 3:a pointer to the array containing the data
	// parameter 4:gives OpenGL a hint about how the data will be used so that it
	// can determine how best to manage the buffer internally
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

/////////// Step 3 ////////////
    // Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

	// Function: Enable the generic vertex attribute
    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color

	// makes the connection between the buffer objects and the generic vertex attribute indexes
	// Function: bind the buffer object to the GL_ARRAY_BUFFER binding point
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	// parameter1:The first argument is the generic attribute index
	// parameter2:The second is the number of components per vertex attribute (1, 2, 3,or 4)
	// parameter3:The third argument is the data type of each component in the buffer
	// parameter4:The fourth is a Boolean which specifies whether or not 
	//            the data should be automatically normalized(mapped to a range
	//            of[-1, 1] for signed integral values or[0, 1] for unsigned integral values)
	// parameter5:The fifth argument is the stride, which indicates the byte offset between consecutive attributes
	// parameter6:The last argument is a pointer, which is not treated as a pointer!
	//            Instead, its value is interpreted as a byte offset from the 
	//            beginning of the buffer to the first attribute in the buffer.
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
}

void SceneBasic::linkMe(GLint vertShader, GLint fragShader)
{
    // Create the program object
    GLuint programHandle = glCreateProgram();
    if(0 == programHandle) {
        fprintf(stderr, "Error creating program object.\n");
        exit(1);
    }

	/*
	If you'd rather not clutter up your vertex shader code with the layout qualifiers (or you're
	using a version of OpenGL that doesn't support them), you can define the attribute indexes
	within the OpenGL program. We can do so by calling glBindAttribLocation just prior to
	linking the shader program. For example, we'd add the following code to the main OpenGL
	program just before the link step:
	*/
    // Bind index 0 to the shader input variable "VertexPosition"
    //glBindAttribLocation(programHandle, 0, "VertexPosition");
    // Bind index 1 to the shader input variable "VertexColor"
    //glBindAttribLocation(programHandle, 1, "VertexColor");

	// Attach the shaders to the program object
    glAttachShader( programHandle, vertShader );
    glAttachShader( programHandle, fragShader );

    // Link the program
    glLinkProgram( programHandle );

    // Check for successful linking
    GLint status;
    glGetProgramiv( programHandle, GL_LINK_STATUS, &status );
    if (GL_FALSE == status) {

        fprintf( stderr, "Failed to link shader program!\n" );

        GLint logLen;
        glGetProgramiv( programHandle, GL_INFO_LOG_LENGTH, &logLen );

        if (logLen > 0) {
            char * log = (char *)malloc(logLen);

            GLsizei written;
            glGetProgramInfoLog(programHandle, logLen, &written, log);

            fprintf(stderr, "Program log: \n%s", log);

            free(log);
        }
    } else {
        glUseProgram( programHandle );
    }
}

void SceneBasic::update( float t )
{

}

void SceneBasic::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );
}

void SceneBasic::resize(int w, int h)
{
    glViewport(0,0,w,h);
}
