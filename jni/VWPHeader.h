/*
 *  VWPHeader.h
 *  
 *
 *  Created by Chandra Tungathurthi on 7/13/11.
 *  Copyright 2011 RWTH Aachen. All rights reserved.
 *
 */



#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#ifdef USE_OPENGL_ES_1_1
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <QCAR/QCAR.h>
#include <QCAR/CameraDevice.h>
#include <QCAR/Renderer.h>
#include <QCAR/VideoBackgroundConfig.h>
#include <QCAR/Trackable.h>
#include <QCAR/Tool.h>
#include <QCAR/Tracker.h>
#include <QCAR/CameraCalibration.h>

#include "SampleUtils.h"
#include "SampleMath.cpp"
#include "Texture.h"
#include "CubeShaders.h"
#include "Teapot.h"
#include "boundary.h"
#include "justnet.h"
#include "ball.h"

#ifdef __cplusplus
extern "C"
{
#endif
	
	// Demo Code -- to be deleted!
	float someNumber =50.0f;
	// Unit cube variables
	// Used for collision detection
	QCAR::Vec3F ballBaseVertices[8];
	QCAR::Vec3F courtBaseVertices[8];
	QCAR::Vec3F dominoTransformedVerticesA[8];
	QCAR::Vec3F dominoTransformedVerticesB[8];
	QCAR::Vec3F dominoNormals[3];
	QCAR::Matrix44F transformBall;
	QCAR::Matrix44F transformCourt;
	QCAR::Matrix44F ballTransform;
	QCAR::Matrix44F courtTransform;
	bool collision = false;
	bool lockBallTranslate = false;
	
	//--
	
	
#define MAX_TAP_TIMER 200
#define MAX_TAP_DISTANCE2 400
	
	
	// Textures:
	int textureCount                = 0;
	Texture** textures              = 0;
	
	// OpenGL ES 2.0 specific:
#ifdef USE_OPENGL_ES_2_0
	unsigned int shaderProgramID    = 0;
	GLint vertexHandle              = 0;
	GLint normalHandle              = 0;
	GLint textureCoordHandle        = 0;
	
	GLint vertexHandleNet           = 1;
	GLint normalHandleNet           = 1;
	GLint textureCoordHandleNet     = 1;
	
	GLint mvpMatrixHandle           = 0;
#endif
	
	// Screen dimensions:
	unsigned int screenWidth        = 0;
	unsigned int screenHeight       = 0;
	
	// Indicates whether screen is in portrait (true) or landscape (false) mode
	bool isActivityInPortraitMode   = false;
	
	//Switch direction
	int direction		 = -1;
	
	// The projection matrix used for rendering virtual objects:
	QCAR::Matrix44F projectionMatrix;


	// Method signatures of bounding box moved to "BoundingBox.h"
	
	
	bool checkIntersection(QCAR::Matrix44F transformA, QCAR::Matrix44F transformB);
	bool isSeparatingAxis(QCAR::Vec3F axis);
	void dumpMVM(const float * matrx);
	void handleCollision();
	
	void detectCollision();	
	// Demo Code -- to be deleted!
	void animateBall(float*, float*);
	
	void animateBallTranslation(float*, float*, float, int); //V2.0
	//void animateBallTranslation(float*, float*, float); // V1.0
	
//	void animateBall(float*);

	// Constants:
	static const float kObjectScale = 3.f * 75.0f;
	
	// Values have to in-sync with the index values in SoundManager - Check it before you update the values!
	// Native method -> java method variables & functions
	const int BALL_BOUNCE = 0;
	const int CROWD_CHEER = 1;
	const int CROWD_CHEER_CONCERT = 2;
	const int CROWD_WOO = 5;
	const int BALL_WALL_HIT=3;
	const int GAME_START_SOUND=6;
	
	
	// Game play 
	
	int yourScore=0;
	void updateScore() ; // increase your score by 1
	
	
	JNIEnv* javaEnv;
	jobject javaObj;
	jclass javaClass;
	
	void playSoundEffect(int soundIndex, float volume = 1.0f);
	void showServeButton();
	void displayMessage(char* message);
	void vibratePhoneShort();
	void vibratePhoneLong();	
	void missedHitMessage();	

	
	
	enum ActionType {
		ACTION_DOWN,
		ACTION_MOVE,
		ACTION_UP,
		ACTION_CANCEL
	};
	
	
	
	typedef struct _TouchEvent {
		bool isActive;
		int actionType;
		int pointerId;
		float x;
		float y;
		float lastX;
		float lastY;
		float startX;
		float startY;
		float tapX;
		float tapY;
		unsigned long startTime;
		unsigned long dt;
		float dist2;
		bool didTap;
	} TouchEvent;
	
TouchEvent touch1, touch2;