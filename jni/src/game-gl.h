//
//  game-gl.h
//  test3
//
//  Created by Jackie Gleason on 5/29/13.
//  Copyright (c) 2013 Jackie Gleason. All rights reserved.
//
#include <math.h>
#include <stdio.h>
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#define LOG_TAG "Native"
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#ifndef test3_game_gl_h
#define test3_game_gl_h



#endif
void setPixels(int *i);
void renderFrameLine();
void InitializeOpenGL(int i, int p, GLuint pro);
void resizeViewport(int newWidth, int newHeight);
void setTextureAsset(AAsset* asset);
void ShutDown();
void setPixelWidth(int i);
void setPixelHeight(int i);
void setLen(int i);
