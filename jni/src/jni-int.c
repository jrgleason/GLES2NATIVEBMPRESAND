#include "game-gl.h"
#include <jni.h>
#include <stdlib.h>


JNIEXPORT void JNICALL Java_com_gleason_gl_nat_Native_init(JNIEnv* env, jclass class, jint resize, jint perspective)
{
	InitializeOpenGL(resize, perspective, 0);
}
JNIEXPORT void JNICALL Java_com_gleason_gl_nat_Native_resize(JNIEnv* env, jclass class, jint width, jint height)
{
	resizeViewport(width, height);
}
JNIEXPORT void JNICALL Java_com_gleason_gl_nat_Native_render(JNIEnv* env, jclass class)
{
  renderFrameLine();
}


JNIEXPORT void JNICALL Java_com_gleason_gl_nat_Native_initBMPTexture(JNIEnv * env, jclass envClass, jintArray pixels, jint width, jint height)
{
	LOGD("Starting initBMP");
	jsize len = (*env)->GetArrayLength(env, pixels);
	jint *body = (*env)->GetIntArrayElements(env, pixels, 0);
	int i = 0;
	int pixel[len];
	LOGD("Trying to see the values, Length is: %d",len);
	for (i=0; i < len; i++){
		if(i<20){
			LOGD("Before Pixel value %d", pixel[i]);
		}
//		unsigned char *bytePtr=(unsigned char*)&n;
//		// should be redone I am not sure this is nessacary just not good with pointers
//		pixel[i] = body[i];
//		if(i<20){
//			LOGD("Pixel value %d", pixel[i]);
//		}
//
	}

	setPixelHeight(height);
	setPixelWidth(width);
	setLen(len);
	setPixels(body);
//	(*env)->ReleaseIntArrayElements(env, pixels, body, 0);
}

JNIEXPORT void JNICALL Java_com_gleason_gl_nat_Native_shutDown(JNIEnv * env, jclass envClass)
{
	ShutDown();
}
