//
//  game-gl.c
//  Test
//
//  Created by Jackie Gleason on 5/29/13.
//  Copyright (c) 2013 Jackie Gleason. All rights reserved.
//
#include "game-gl.h"

GLfloat gObj[12] = {
	-1.0f, 0.25f, 1.0f,
	1.0f, 0.25f, 1.0f,
	-1.0f, -0.25f, 1.0f,
	1.0f, -0.25f, 1.0f
};

GLbyte vShaderStr[] = "attribute vec4 vPosition;   \n"
"uniform mat4 Projection;   \n"
"uniform mat4 xRotation;   \n"
"uniform mat4 yRotation;   \n"
"uniform mat4 zRotation;   \n"
"attribute vec2 texPosition; \n"
"attribute vec4 inColor;   \n"
"varying vec4 fragColor;   \n"
"varying vec2 v_texCoord;     \n"
"void main()                 \n"
"{                           \n"
"   gl_Position = vPosition; \n"
"   gl_Position = xRotation * vPosition; \n"
"   gl_Position = yRotation * gl_Position; \n"
"   gl_Position = zRotation * gl_Position; \n"
"   gl_Position = Projection * gl_Position; \n"
"   fragColor = inColor; \n"
"   v_texCoord = texPosition; \n"
"}                           \n";
GLbyte fShaderStr[] =
   "precision mediump float;                            \n"
   "varying vec2 v_texCoord;                            \n"
   "uniform sampler2D s_texture;                        \n"
   "void main()                                         \n"
   "{                                                   \n"
   "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
   "}                                                   \n";



GLuint programObject;
GLint textCoordLoc;
GLint sampler;

float widthScreen;
float heightScreen;

AAsset* textureAsset;
void setTextureAsset(AAsset* asset){
	textureAsset = asset;
}

void glXRotate(float angle){
	float DEG2RAD = 3.14159f / 180.0f;
	float rad = angle * DEG2RAD;
	float rotation[16] = {
        1, 0, 0, 0,
        0, cosf(rad), sin(rad), 0,
        0, - sinf(rad), cosf(rad), 0,
        0, 0, 0, 1
	};
	GLint projectionUniform = glGetUniformLocation(programObject, "xRotation");
	glUniformMatrix4fv(projectionUniform, 1, 0, &rotation[0]);
}

void glYRotate(float angle){
	float DEG2RAD = 3.14159f / 180.0f;
	float rad = angle * DEG2RAD;
	float rotation[16] = {
        cosf(rad), 0, sin(rad), 0,
        0, 1, 0, 0,
        - (sinf(rad)), 0, cosf(rad), 0,
        0, 0, 0, 1
    };
	GLint projectionUniform = glGetUniformLocation(programObject, "yRotation");
	glUniformMatrix4fv(projectionUniform, 1, 0, &rotation[0]);
}

void glZRotate(float angle){
	float DEG2RAD = 3.14159f / 180.0f;
	float rad = angle * DEG2RAD;
	float rotation[16] = {
        cosf(rad), sin(rad), 0, 0,
        - sinf(rad), cosf(rad), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
	GLint projectionUniform = glGetUniformLocation(programObject, "zRotation");
	glUniformMatrix4fv(projectionUniform, 1, 0, &rotation[0]);
}
static float aspectRatio = 1.0f;
void glFrustumf(float near, float far){
    float DEG2RAD = 3.14159f / 180.0f;
    float fov = 90*DEG2RAD;
    float h = cosf(0.5f*fov)/sinf(0.5f*fov);
    float w = h * aspectRatio;
    float a =  - (near+far)/(near - far);
    float b = - ((2*far*near)/(far-near));

    float proj[16] = {
        w, 0, 0, 0,
        0, h, 0, 0,
        0, 0, a, 1,
        0, 0, b, 0
    };
    GLint projectionUniform = glGetUniformLocation(programObject, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &proj[0]);
}

GLuint getProgram(){
    return programObject;
}

GLuint LoadShader(GLenum type, const char *shaderSrc) {
#ifdef __APPLE__
    printf("Compiling Shader \n");
#endif
	GLuint shader;
	GLint compiled;
	// Create the shader object
	shader = glCreateShader(type);

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
#ifdef __APPLE__
        printf("Shader Failed \n");
#endif
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		glDeleteShader(shader);
		return 0;
	}
    else{
#ifdef __APPLE__
        printf("Shader Created \n");
#endif
    }
	return shader;

}

void InitializeOpenGL(int i, int p, GLuint pro) {
#ifdef __APPLE__
    printf("Testing the init \n");
#endif
	//LOGD("Initialization Started");
	GLuint vertexShader;
	GLuint fragmentShader;
#ifdef __APPLE__
    printf("Test %d\n", GL_VERTEX_SHADER);
#endif
	vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);
    if(pro == 0){
        programObject = glCreateProgram();
        if(programObject == 0){
#ifdef __APPLE__
            printf("Invalid program returned \n");
#else
            LOGD("Invalid program returned");
#endif
        }
    }
    else{
        programObject = pro;
    }
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
    glEnable(GL_DEPTH_TEST);
	glBindAttribLocation(programObject, 0, "vPosition");
    glLinkProgram(programObject);
	glUseProgram(programObject);
	textCoordLoc = glGetAttribLocation (programObject, "texPosition" );
	sampler = glGetUniformLocation(programObject, "s_texture" );

	GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        // printf("Error linking program:\n");
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
#ifdef __APPLE__
            char* infoLog = (char*)malloc(sizeof(char)*infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);

            printf("%s\n",infoLog);
            free(infoLog);
#endif
        }
        glDeleteProgram(programObject);
    }
    else{
        // printf("No linker error \n");

    }

	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
#ifdef __APPLE__
    printf("Initialization Complete \n");
#else
    LOGD("Initialization Complete");
#endif
}
void resizeViewport(int newWidth, int newHeight) {
#ifdef __APPLE__
    printf("Width: %d Height: %d \n", newWidth, newHeight);
#endif
	glViewport(0, 0, newWidth, newHeight);

//	aspectRatio = newHeight/((float)newWidth);
	LOGD("Setting aspect ratio... width %d %.1f",aspectRatio);
}

float z = 0.0f;
float s = 0.5f;
float f = 0.0f;

int *pixels;
int len = 0;
int pixelWidth = 2;
int pixelHeight = 2;
GLubyte *buffer;

void generateBuffer(){
    buffer = (GLubyte *)malloc(pixelWidth * pixelHeight * 3);
    int c;
    for(c = 0; c < pixelWidth * pixelHeight * 3; c++)
      buffer[c] = pixels[c];
}
void setPixels(int *i){
	pixels = i;
}

void setLen(int i){
	len =i;
}

void setPixelWidth(int i){
	pixelWidth = i;
}
void setPixelHeight(int i){
	pixelHeight = i;
}

static GLuint textureId = 0;
int render = 1;

void renderMe(){
	if(z>1.0f){
	        z = 0.0f;
	    }
	    else{
	        z = z+.01;
	    }
	    if (f > 359.0f) {
	        f = 0.0f;
	    } else {
	        f = f + 1.0f;
	    }
	    GLubyte bytePixChar[4 * 3]={
	          255, 0, 0, //red
	          0, 255, 0, //green
	          0, 0, 255,  //blue
	          255, 255, 0  //yellow
	     };
	    int o;

	      GLfloat texC[8] = {
	        0.0, 0.0,
	        1.0, 0.0,
	        0.0, 1.0,
	        1.0, 1.0

	      };
	    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	    glGenTextures(1, &textureId);
	    glBindTexture(GL_TEXTURE_2D, textureId);
	//    GLubyte *byteBuffer = (GLubyte *)malloc(pixelWidth * pixelHeight * 3);
	//        int c;
	//        for(c = 0; c < pixelWidth * pixelHeight * 3; c++)
	//          byteBuffer[c] = pixels[c];
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixelWidth, pixelHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glXRotate(0.0f);
		glYRotate(0.0f);
		glZRotate(0.0f);
		glFrustumf(0.1f, 10.0f);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, gObj);
		glVertexAttribPointer(textCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, texC);

		glUniform1i(sampler, 0 );
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(textCoordLoc);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		render = 0;
}

void renderFrameLine() {
    if(render == 1){
    	renderMe();
    }
}

void ShutDown(){
	if(textureId != 0)
		glDeleteTextures ( 1,&textureId);
	glDeleteProgram ( programObject );
}
