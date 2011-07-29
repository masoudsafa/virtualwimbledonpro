/*==============================================================================
            Copyright (c) 2010-2011 QUALCOMM Incorporated.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
            
@file 
    ImageTargets.cpp

@brief
    Sample for ImageTargets

==============================================================================*/

#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

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
#include<math.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Demo Code -- to be deleted!
float someNumber =0.0f; // 50.0f 

	
// --

	
// Unit cube variables
// Used for collision detection
	QCAR::Vec3F ballBaseVertices[8];
	QCAR::Vec3F courtBaseVertices[8];
		QCAR::Vec3F cameraBaseVertices[8];
QCAR::Vec3F dominoTransformedVerticesA[8];
QCAR::Vec3F dominoTransformedVerticesB[8];
QCAR::Vec3F dominoNormals[3];
QCAR::Matrix44F transformBall;
QCAR::Matrix44F transformCourt;
QCAR::Matrix44F ballTransform;
QCAR::Matrix44F courtTransform;
	QCAR::Vec4F cameraPosition;	
	QCAR::Matrix44F cameraTransform;	
bool collision = false;

//--





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




void initCourtBaseVertices();
void initBallBaseVertices();

void initDominoNormals();
bool checkIntersection(QCAR::Matrix44F transformA, QCAR::Matrix44F transformB,bool camera);
bool isSeparatingAxis(QCAR::Vec3F axis);
void dumpMVM(const float * matrx);
void handleCollision();
void detectAndHandleCollision();

void detectCollision();	
// Demo Code -- to be deleted!
void animateBall(float*);
void updateCourtTransform();


// Constants:
static const float kObjectScale = 3.f * 75.0f;
	float val =1.0f;




//Structs

struct Vector3
{
float x;
float y;
float z;
};

struct BoundingBox
{
Vector3 max;
Vector3 min;
};



JNIEXPORT int JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_getOpenGlEsVersionNative(JNIEnv *, jobject)
{
#ifdef USE_OPENGL_ES_1_1        
    return 1;
#else
    return 2;
#endif
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_setActivityPortraitMode(JNIEnv *, jobject, jboolean isPortrait)
{
    isActivityInPortraitMode = isPortrait;
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_onQCARInitializedNative(JNIEnv *, jobject)
{
    // Comment in to enable tracking of up to 2 targets simultaneously and
    // split the work over multiple frames:
//     QCAR::setHint(QCAR::HINT_MAX_SIMULTANEOUS_IMAGE_TARGETS, 2);
//     QCAR::setHint(QCAR::HINT_IMAGE_TARGET_MULTI_FRAME_ENABLED, 1);
	
	// Initialise the unit cube
	 initCourtBaseVertices();
	 initBallBaseVertices();
	 initDominoNormals();
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_renderFrame(JNIEnv *, jobject)
{
    //LOG("Java_com_qualcomm_QCARSamples_ImageTargets_GLRenderer_renderFrame");

    // Clear color and depth buffer 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render video background:
    QCAR::State state = QCAR::Renderer::getInstance().begin();
        
#ifdef USE_OPENGL_ES_1_1
    // Set GL11 flags:
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
        
#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Did we find any trackables this frame?
    for(int tIdx = 0; tIdx < state.getNumActiveTrackables(); tIdx++)
    {
        // Get the trackable:
        const QCAR::Trackable* trackable = state.getActiveTrackable(tIdx);
       QCAR::Matrix44F modelViewMatrix = QCAR::Tool::convertPose2GLMatrix(trackable->getPose());


#ifdef USE_OPENGL_ES_1_1
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_GLRenderer_renderFrame: USE_OPENGL_ES_1_1 UNSUPPORTED");
#else



        QCAR::Matrix44F modelViewProjection;


        //Tennis Court in 3 objects
		
		/////////////////////////////////////////////////////////////////////////////////////////
		//        The Court																		//
		/////////////////////////////////////////////////////////////////////////////////////////  
		
		transformCourt =  QCAR::Tool::convertPose2GLMatrix(trackable->getPose());
		 QCAR::Matrix44F inverseModelView = SampleMath::Matrix44FInverse(modelViewMatrix);
	
		cameraPosition.data[0] = 0.0f;
		cameraPosition.data[1] = 0.0f;
		cameraPosition.data[2] = 0.0f;
		cameraPosition.data[3] = 1.0f;
		
		
		
		
		
		cameraPosition = SampleMath::SampleMath::Vec4FTransform(cameraPosition,inverseModelView);
		cameraTransform = SampleMath::Matrix44FIdentity();
		SampleUtils::translatePoseMatrix(cameraPosition.data[0],cameraPosition.data[1],cameraPosition.data[2],
										 &cameraTransform.data[0]);
		
		
		LOG("Camera Position:: [%f, %f, %f]",cameraPosition.data[0],cameraPosition.data[1],cameraPosition.data[2]);
		
		LOG("->Camera Transform");
		dumpMVM(&cameraTransform.data[0]);
		
		
		float* transformPtr = &transformCourt.data[0];
		courtTransform = SampleMath::Matrix44FIdentity();
		
		
		
		
		
		LOG("Court transform:Before");
		dumpMVM(&courtTransform.data[0]);
	
        SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, 4*kObjectScale,
                                    transformPtr);
		
	//	SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, 4*kObjectScale, &courtTransform.data[0]);
		LOG("Court transform:After");
		dumpMVM(&courtTransform.data[0]);
	
		
        SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
                                    transformPtr ,
                                    &modelViewProjection.data[0]);

		
		// Render the court
        glUseProgram(shaderProgramID);


        glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, 0,
        					  boundaryVerts);
        glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0,
        					  boundaryNormals);

        glEnableVertexAttribArray(vertexHandle);
        glEnableVertexAttribArray(normalHandle);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, textures[3]->mTextureID);
        glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                           (GLfloat*)&modelViewProjection.data[0] );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, boundaryNumVerts);

        

		/////////////////////////////////////////////////////////////////////////////////////////
		//        Lines & Net																	//
		/////////////////////////////////////////////////////////////////////////////////////////  


        //second object - Lines
        modelViewMatrix =
                    QCAR::Tool::convertPose2GLMatrix(trackable->getPose());
        SampleUtils::translatePoseMatrix(0.0f, 0.0f, 1,
                                         &modelViewMatrix.data[0]);
        SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, 4*kObjectScale,
                                     &modelViewMatrix.data[0]);
        SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
                                    &modelViewMatrix.data[0] ,
                                    &modelViewProjection.data[0]);

        glEnableVertexAttribArray(vertexHandle);
        glEnableVertexAttribArray(normalHandle);

        glBindTexture(GL_TEXTURE_2D, textures[2]->mTextureID);
        glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                           (GLfloat*)&modelViewProjection.data[0] );
        glDrawArrays(GL_TRIANGLES, 0, boundaryNumVerts);


        modelViewMatrix =
                    QCAR::Tool::convertPose2GLMatrix(trackable->getPose());

        SampleUtils::translatePoseMatrix(0.0f,0.0f, (kObjectScale / 10),
                                         &modelViewMatrix.data[0]);
        SampleUtils::scalePoseMatrix(2*kObjectScale/3, kObjectScale, kObjectScale,
                                     &modelViewMatrix.data[0]);
        SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
                                    &modelViewMatrix.data[0] ,
                                    &modelViewProjection.data[0]);

        glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, 0,
        					  justnetVerts);
        glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0,
        					  justnetNormals);
//        glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0,
//        					justnetTexCoords);

        glEnableVertexAttribArray(vertexHandle);
        glEnableVertexAttribArray(normalHandle);
//        glEnableVertexAttribArray(textureCoordHandle);

        glBindTexture(GL_TEXTURE_2D, textures[2]->mTextureID);
        glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                           (GLfloat*)&modelViewProjection.data[0] );
        glDrawArrays(GL_TRIANGLES, 0, justnetNumVerts);
		

/////////////////////////////////////////////////////////////////////////////////////////
//        Ball																			//
/////////////////////////////////////////////////////////////////////////////////////////      
		
		transformBall =  QCAR::Tool::convertPose2GLMatrix(trackable->getPose());
		
		
		LOG(" Trackable :");
		dumpMVM(&transformBall.data[0]);
		
		
		
		transformPtr = &transformBall.data[0];
		ballTransform = SampleMath::Matrix44FIdentity();
		
        animateBall(transformPtr);
		animateBall(&ballTransform.data[0]);
		
		
		//updateCourtTransform();
		LOG(" In Render7");
		
		
		// Scaling doesn't work here so neglect it
		//SampleUtils::scalePoseMatrix((kObjectScale/32), (kObjectScale/32), (kObjectScale/32), &ballTransform.data[0]);

		SampleUtils::scalePoseMatrix((kObjectScale/32), (kObjectScale/32), (kObjectScale/32),
									 transformPtr);
		
		LOG("Court transform:After");
		dumpMVM(&ballTransform.data[0]);
			
		SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
								transformPtr,
									&modelViewProjection.data[0]);

		// Now render the ball


		glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, 0,
							  ballVerts);
		glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0,
							ballTexCoords);

		glEnableVertexAttribArray(vertexHandle);
		glEnableVertexAttribArray(textureCoordHandle);

		glBindTexture(GL_TEXTURE_2D, textures[0]->mTextureID);
		glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
						   (GLfloat*)&modelViewProjection.data[0] );
		glDrawArrays(GL_TRIANGLES, 0, ballNumVerts);
		SampleUtils::checkGlError("ImageTargets renderFrame");
		
// Done with rendering now check for collision	 	
// Uncomment to check out the collision detection
	
		detectCollision();
		
		
		//detectAndHandleCollision();
		
		
// --		
		
#endif

    }

    glDisable(GL_DEPTH_TEST);

#ifdef USE_OPENGL_ES_1_1        
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
    glDisableVertexAttribArray(vertexHandle);
    glDisableVertexAttribArray(normalHandle);
    glDisableVertexAttribArray(textureCoordHandle);

    glDisableVertexAttribArray(vertexHandleNet);
    glDisableVertexAttribArray(normalHandleNet);
#endif

    QCAR::Renderer::getInstance().end();
}



void
configureVideoBackground()
{
    // Get the default video mode:
    QCAR::CameraDevice& cameraDevice = QCAR::CameraDevice::getInstance();
    QCAR::VideoMode videoMode = cameraDevice.
                                getVideoMode(QCAR::CameraDevice::MODE_DEFAULT);


    // Configure the video background
    QCAR::VideoBackgroundConfig config;
    config.mEnabled = true;
    config.mSynchronous = true;
    config.mPosition.data[0] = 0.0f;
    config.mPosition.data[1] = 0.0f;
    
    if (isActivityInPortraitMode)
    {
        //LOG("configureVideoBackground PORTRAIT");
        config.mSize.data[0] = videoMode.mHeight
                                * (screenHeight / (float)videoMode.mWidth);
        config.mSize.data[1] = screenHeight;
    }
    else
    {
        //LOG("configureVideoBackground LANDSCAPE");
        config.mSize.data[0] = screenWidth;
        config.mSize.data[1] = videoMode.mHeight
                            * (screenWidth / (float)videoMode.mWidth);
    }

    // Set the config:
    QCAR::Renderer::getInstance().setVideoBackgroundConfig(config);
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_initApplicationNative(
                            JNIEnv* env, jobject obj, jint width, jint height)
{
    LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_initApplicationNative");
    
    // Store screen dimensions
    screenWidth = width;
    screenHeight = height;
        
    // Handle to the activity class:
    jclass activityClass = env->GetObjectClass(obj);

    jmethodID getTextureCountMethodID = env->GetMethodID(activityClass,
                                                    "getTextureCount", "()I");
    if (getTextureCountMethodID == 0)
    {
        LOG("Function getTextureCount() not found.");
        return;
    }

    textureCount = env->CallIntMethod(obj, getTextureCountMethodID);    
    if (!textureCount)
    {
        LOG("getTextureCount() returned zero.");
        return;
    }

    textures = new Texture*[textureCount];

    jmethodID getTextureMethodID = env->GetMethodID(activityClass,
        "getTexture", "(I)Lcom/qualcomm/QCARSamples/ImageTargets/Texture;");

    if (getTextureMethodID == 0)
    {
        LOG("Function getTexture() not found.");
        return;
    }

    // Register the textures
    for (int i = 0; i < textureCount; ++i)
    {

        jobject textureObject = env->CallObjectMethod(obj, getTextureMethodID, i); 
        if (textureObject == NULL)
        {
            LOG("GetTexture() returned zero pointer");
            return;
        }

        textures[i] = Texture::create(env, textureObject);
    }
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_deinitApplicationNative(
                                                        JNIEnv* env, jobject obj)
{
    LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_deinitApplicationNative");

    // Release texture resources
    if (textures != 0)
    {    
        for (int i = 0; i < textureCount; ++i)
        {
            delete textures[i];
            textures[i] = NULL;
        }
    
        delete[]textures;
        textures = NULL;
        
        textureCount = 0;
    }
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_startCamera(JNIEnv *,
                                                                         jobject)
{
    LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_startCamera");

    // Initialize the camera:
    if (!QCAR::CameraDevice::getInstance().init())
        return;

    // Configure the video background
    configureVideoBackground();

    // Select the default mode:
    if (!QCAR::CameraDevice::getInstance().selectVideoMode(
                                QCAR::CameraDevice::MODE_DEFAULT))
        return;

    // Start the camera:
    if (!QCAR::CameraDevice::getInstance().start())
        return;

    // Uncomment to enable flash
    //if(QCAR::CameraDevice::getInstance().setFlashTorchMode(true))
    //	LOG("IMAGE TARGETS : enabled torch");

    // Uncomment to enable infinity focus mode, or any other supported focus mode
    // See CameraDevice.h for supported focus modes
    //if(QCAR::CameraDevice::getInstance().setFocusMode(QCAR::CameraDevice::FOCUS_MODE_INFINITY))
    //	LOG("IMAGE TARGETS : enabled infinity focus");

    // Start the tracker:
    QCAR::Tracker::getInstance().start();
 
    // Cache the projection matrix:
    const QCAR::Tracker& tracker = QCAR::Tracker::getInstance();
    const QCAR::CameraCalibration& cameraCalibration =
                                    tracker.getCameraCalibration();
    projectionMatrix = QCAR::Tool::getProjectionGL(cameraCalibration, 2.0f,
                                            2000.0f);
	
	
	LOG("Camera projection::startCamera");
	dumpMVM(&projectionMatrix.data[0]);
	
	
	
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_stopCamera(JNIEnv *,
                                                                   jobject)
{
    LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_stopCamera");

    QCAR::Tracker::getInstance().stop();

    QCAR::CameraDevice::getInstance().stop();
    QCAR::CameraDevice::getInstance().deinit();
}

JNIEXPORT jboolean JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_toggleFlash(JNIEnv*, jobject, jboolean flash)
{
    return QCAR::CameraDevice::getInstance().setFlashTorchMode((flash==JNI_TRUE)) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_autofocus(JNIEnv*, jobject)
{
    return QCAR::CameraDevice::getInstance().startAutoFocus()?JNI_TRUE:JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_setFocusMode(JNIEnv*, jobject, jint mode)
{
    return QCAR::CameraDevice::getInstance().setFocusMode(mode)?JNI_TRUE:JNI_FALSE;
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_initRendering(
                                                    JNIEnv* env, jobject obj)
{
    LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_initRendering");

    // Define clear color
    glClearColor(0.0f, 0.0f, 0.0f, QCAR::requiresAlpha() ? 0.0f : 1.0f);
    
    // Now generate the OpenGL texture objects and add settings
    for (int i = 0; i < textureCount; ++i)
    {
        glGenTextures(1, &(textures[i]->mTextureID));
        glBindTexture(GL_TEXTURE_2D, textures[i]->mTextureID);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[i]->mWidth,
                textures[i]->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                (GLvoid*)  textures[i]->mData);
    }
#ifndef USE_OPENGL_ES_1_1
  
    shaderProgramID     = SampleUtils::createProgramFromBuffer(cubeMeshVertexShader,
                                                            cubeFragmentShader);

    vertexHandle        = glGetAttribLocation(shaderProgramID,
                                                "vertexPosition");
    normalHandle        = glGetAttribLocation(shaderProgramID,
                                                "vertexNormal");
    textureCoordHandle  = glGetAttribLocation(shaderProgramID,
                                                "vertexTexCoord");
    mvpMatrixHandle     = glGetUniformLocation(shaderProgramID,
                                                "modelViewProjectionMatrix");

#endif

}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_updateRendering(
                        JNIEnv* env, jobject obj, jint width, jint height)
{
    LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_updateRendering");
    
    // Update screen dimensions
    screenWidth = width;
    screenHeight = height;

    // Reconfigure the video background
    configureVideoBackground();
}

///////////////////////////////////////////////////
//Custom Methods

double
getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double t = tv.tv_sec + tv.tv_usec/1000000.0;
    return t;
}


// Demo code -- to be deleted/changed
//void
//animateBall(float* transform)
//{
//	//if(!collision&&someNumber<=50.0f){
////		someNumber-=0.5f;
////		
////	}
////	else {
////		if(collision||someNumber==-50.0f){
////			someNumber+=0.5f;
////			if(someNumber==50.0f)
////				collision = false; // Reset collision 
////			
////		}
////	}
////
////	
////   SampleUtils::translatePoseMatrix(50.0f, 0.0f, float(someNumber),
////    										 transform);
//
//
//static float rotateBowlAngle = 0.0f;
//static double prevTime = getCurrentTime();
//double time = getCurrentTime();   	
//float dt = (float)(time-prevTime);  
//
//	if(collision){
//		direction*=-1;
//		collision = false;
//		LOG("Calling animate after collision6");
//		val+=5.0f;
//		//animateBall(transform);
//		
//		
//	}
//	
//	
//	   dt*=direction;	//Switches ball direction
//	
//	   rotateBowlAngle += dt * 180.0f/3.1415f;  // Animate angle based on time
//	LOG("Rotate angle:%f",rotateBowlAngle);
//	
//	
//	
//	SampleUtils::rotatePoseMatrix(rotateBowlAngle, 1.0f, 0.0f, 0.0f,
//                                  transform);
//	
//	//SampleUtils::translatePoseMatrix(0.0f, -0.50f*120.0f, 1.35f*120.0f,
////									 transform);
//	
//	SampleUtils::translatePoseMatrix(0.0f, 0.7f*120.0f, 0.0f,
//									 transform);
//	
//	SampleUtils::rotatePoseMatrix(-90.0f, 1.0f, 0, 0,
//								 transform);
//	
//	prevTime = time;	
//	
//
//}
//	
//	
//// This is likely to be changed later
//// Temporary fix!
//void updateCourtTransform(){
//	
//	//update the x, y co-ordinates to correctly detect collision instead of just origin
//	
//	QCAR::Matrix44F ball = SampleMath::Matrix44FInverse(ballTransform);
//	
//	
//	LOG("Dumping ball inverse:");
//	dumpMVM(&ball.data[0]);
//	
//	
//	float xpos =  ball.data[3]; //   X transformation
//	float ypos  = ball.data[7]; // Y Transformation
// 	float zpos = ball.data[11]; // Z Transformation
//	
//	LOG("Positions: [%f %f %f ]", -xpos, -ypos, -zpos);
//	
//	SampleUtils::translatePoseMatrix( -xpos, -ypos, courtTransform.data[2],
//									 &courtTransform.data[0]);
//	
//	
//	
//	
//	}

	
	
	// New Code
	
	
	void
	animateBall(float* transform)
	{
		//if(!collision&&someNumber<=50.0f){
//		//		someNumber-=0.5f;
//		//		
//		//	}
//		//	else {
//		//		if(collision||someNumber==-50.0f){
//		//			someNumber+=0.5f;
//		//			if(someNumber==50.0f)
//		//				collision = false; // Reset collision 
//		//			
//		//		}
//		//	}
//		//
//		//	
//		//   SampleUtils::translatePoseMatrix(50.0f, 0.0f, float(someNumber),
//		//    										 transform);
//		//
////		
		static float rotateBowlAngle = 0.0f;
		static double prevTime = getCurrentTime();
		double time = getCurrentTime();   	
		float dt = (float)(time-prevTime);  
		static bool firstTime=true;
		
		if(collision){
			direction*=-1;
			collision=false;
			LOG("Calling animate after collision99");
			val+=30.0f;
			rotateBowlAngle =0.0;
			firstTime=false;
			//animateBall(transform);
			
			
		}
		
		
		dt*=direction;	//Switches ball direction
		
		rotateBowlAngle += dt * 180.0f/3.1415f;  // Animate angle based on time
		LOG("Rotate angle:%f",rotateBowlAngle);
		

		
		SampleUtils::rotatePoseMatrix(rotateBowlAngle, val, 0.0f, 0.0f,
									  transform);
		
		
		
		SampleUtils::translatePoseMatrix(0.0f, +0.50f*120.0f, -1.35f*120.0f,
										 transform);
		
		
		SampleUtils::translatePoseMatrix(0.0f, -0.50f*120.0f, 1.35f*120.0f,
											 transform);
		
		
		SampleUtils::translatePoseMatrix(0.0f, 20.0f, 0.0f,transform);
			
//		
	
		
		
		prevTime = time;	
		
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
JNIEXPORT jfloatArray JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_getBallModelView(JNIEnv* env){

	LOG("MVM Ball::native");

//Global mvm of ball
	
jfloatArray ballMVM;
int size=16;

 ballMVM = env->NewFloatArray(size);
 if (ballMVM == NULL) {
	 LOG("Cannot Allocated MVM");
     return NULL; 
 }

 jfloat fill[size];
 for (int i = 0; i < size; i++) {
     fill[i] = ballTransform.data[i];
    // LOG("ballmvm:%d",fill[i]);
 }
 // move from the temp structure to the java structure
 env->SetFloatArrayRegion(ballMVM, 0, size, fill);



 return ballMVM;
	
}

JNIEXPORT jfloatArray JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_getCourtModelView(JNIEnv* env, jobject obj){
	LOG("Court MVM::native");
	/*
	
	//Global mvm of ball
	jfloatArray courtMVM;
	int size=16;

	courtMVM = env->NewFloatArray(size);
	 if (courtMVM == NULL) {
		 LOG("Cannot Allocated MVM");
	     return NULL;

	 }

	 jfloat fill[size];
	 for (int i = 0; i < size; i++) {
	     fill[i] = mvmCourt[i];
	 }
	 // move from the temp structure to the java structure
	 env->SetFloatArrayRegion(courtMVM, 0, size, fill);



	 return courtMVM;
	 */
	}

JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_dumpBallMVM(JNIEnv* env, jobject){
	LOG("dumpBallMVM");
	
	//SampleUtils::printMatrix(mvmBall);

}

// Change it later to send actual ball bounce
	
JNIEXPORT jboolean JNICALL
Java_com_vmp_main_GamePlay_isBallBounce(JNIEnv*, jobject)
{
		
	LOG("isBallBounce::Native");
	
	if(collision)
			return JNI_TRUE;
	return JNI_FALSE;

}
	
	
JNIEXPORT jboolean JNICALL
Java_com_vmp_main_GamePlay_isBallHit(JNIEnv*, jobject)
{
	
	
	if(collision)
		return JNI_TRUE;
	return JNI_FALSE;
}
	
	
	
	
	


void dumpMVM(const float* mvm){
	LOG("Dumping MVM:");
		SampleUtils::printMatrix(mvm);

}




// Collision detection
	



// ----------------------------------------------------------------------------
// Collision detection
// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	// Unitcube as bounding box - initialization
	// ----------------------------------------------------------------------------
	void
	initBallBaseVertices()
	{
		// Initialize a set of vertices describing the Bouding box for Ball
		
		ballBaseVertices[0] = QCAR::Vec3F(1.0f, 1.0f, 1.0f);
		ballBaseVertices[1] = QCAR::Vec3F(-1.0f, 1.0f, 1.0f);
		ballBaseVertices[2] = QCAR::Vec3F(1.0f, -1.0f, 1.0f);
		ballBaseVertices[3] = QCAR::Vec3F(-1.0f, -1.0f, 1.0f);
		ballBaseVertices[4] = QCAR::Vec3F(1.0f, 1.0f, -1.0f);
		ballBaseVertices[5] = QCAR::Vec3F(-1.0f, 1.0f, -1.0f);
		ballBaseVertices[6] = QCAR::Vec3F(1.0f, -1.0f, -1.0f);
		ballBaseVertices[7] = QCAR::Vec3F(-1.0f, -1.0f, -1.0f);
	}
	
	void
	initCourtBaseVertices()
	{
		// Initialize a set of vertices describing the bounding box of Court
		
		courtBaseVertices[0] = QCAR::Vec3F(kObjectScale/2, kObjectScale/2, 1.0f);
		courtBaseVertices[1] = QCAR::Vec3F(-kObjectScale/2, kObjectScale/2, 1.0f);
		courtBaseVertices[2] = QCAR::Vec3F(kObjectScale/2, -kObjectScale/2, 1.0f);
		courtBaseVertices[3] = QCAR::Vec3F(-kObjectScale/2, -kObjectScale/2, 1.0f);
		courtBaseVertices[4] = QCAR::Vec3F(kObjectScale/2, kObjectScale/2, -1.0f);
		courtBaseVertices[5] = QCAR::Vec3F(-kObjectScale/2, kObjectScale/2, -1.0f);
		courtBaseVertices[6] = QCAR::Vec3F(kObjectScale/2, -kObjectScale/2, -1.0f);
		courtBaseVertices[7] = QCAR::Vec3F(-kObjectScale/2, -kObjectScale/2, -1.0f);
	}	
	
	void
	initCameraBaseVertices()
	{
		// Initialize a set of vertices describing the bounding box of Court
		cameraBaseVertices[0] = QCAR::Vec3F(50.0f, 50.0f, 50.0f);
		cameraBaseVertices[1] = QCAR::Vec3F(-50.0f, 50.0f, 50.0f);
		cameraBaseVertices[2] = QCAR::Vec3F(50.0f, -50.0f, 50.0f);
		cameraBaseVertices[3] = QCAR::Vec3F(-50.0f, -50.0f, 50.0f);
		cameraBaseVertices[4] = QCAR::Vec3F(50.0f, 50.0f, -50.0f);
		cameraBaseVertices[5] = QCAR::Vec3F(-50.0f, 50.0f, -50.0f);
		cameraBaseVertices[6] = QCAR::Vec3F(50.0f, -50.0f, -50.0f);
		cameraBaseVertices[7] = QCAR::Vec3F(-50.0f, -50.0f, -50.0f);
	}	
	
	void
	initDominoNormals()
	{
		// Initialize a set of normals for the unit cube
		
		dominoNormals[0] = QCAR::Vec3F(1, 0, 0);
		dominoNormals[1] = QCAR::Vec3F(0, 1, 0);
		dominoNormals[2] = QCAR::Vec3F(0, 0, 1);
	}
	
	
	
	
	bool
	checkIntersection(QCAR::Matrix44F transformA, QCAR::Matrix44F transformB,bool camera)
	{
		// Use the separating axis theorem to determine whether or not
		// two object-oriented bounding boxes are intersecting
		
		transformA = SampleMath::Matrix44FTranspose(transformA);
		transformB = SampleMath::Matrix44FTranspose(transformB);
		
		LOG("--Printing tranforms--");
		LOG("Ball::");
		dumpMVM(&transformA.data[0]);
		LOG("Court::");
		dumpMVM(&transformB.data[0]);
		
		
		QCAR::Vec3F normalA1 = SampleMath::Vec3FTransformNormal(dominoNormals[0], transformA);
		QCAR::Vec3F normalA2 = SampleMath::Vec3FTransformNormal(dominoNormals[1], transformA);
		QCAR::Vec3F normalA3 = SampleMath::Vec3FTransformNormal(dominoNormals[2], transformA);
		
		QCAR::Vec3F normalB1 = SampleMath::Vec3FTransformNormal(dominoNormals[0], transformB);
		QCAR::Vec3F normalB2 = SampleMath::Vec3FTransformNormal(dominoNormals[1], transformB);
		QCAR::Vec3F normalB3 = SampleMath::Vec3FTransformNormal(dominoNormals[2], transformB);
		
		QCAR::Vec3F edgeAxisA1B1 = SampleMath::Vec3FCross(normalA1, normalB1);
		QCAR::Vec3F edgeAxisA1B2 = SampleMath::Vec3FCross(normalA1, normalB2);
		QCAR::Vec3F edgeAxisA1B3 = SampleMath::Vec3FCross(normalA1, normalB3);
		
		QCAR::Vec3F edgeAxisA2B1 = SampleMath::Vec3FCross(normalA2, normalB1);
		QCAR::Vec3F edgeAxisA2B2 = SampleMath::Vec3FCross(normalA2, normalB2);
		QCAR::Vec3F edgeAxisA2B3 = SampleMath::Vec3FCross(normalA2, normalB3);
		
		QCAR::Vec3F edgeAxisA3B1 = SampleMath::Vec3FCross(normalA3, normalB1);
		QCAR::Vec3F edgeAxisA3B2 = SampleMath::Vec3FCross(normalA3, normalB2);
		QCAR::Vec3F edgeAxisA3B3 = SampleMath::Vec3FCross(normalA3, normalB3);
		
		for (int i = 0; i < 8; i++) {
			dominoTransformedVerticesA[i] = SampleMath::Vec3FTransform(ballBaseVertices[i], transformA);
			
			
			if(!camera)
				dominoTransformedVerticesB[i] = SampleMath::Vec3FTransform(courtBaseVertices[i], transformB);
			else {
				dominoTransformedVerticesB[i] = SampleMath::Vec3FTransform(cameraBaseVertices[i], transformB);
			}

		}
		
		if (isSeparatingAxis(normalA1)) return false;
		if (isSeparatingAxis(normalA2)) return false;
		if (isSeparatingAxis(normalA3)) return false;
		if (isSeparatingAxis(normalB1)) return false;
		if (isSeparatingAxis(normalB2)) return false;
		if (isSeparatingAxis(normalB3)) return false;
		if (isSeparatingAxis(edgeAxisA1B1)) return false;
		if (isSeparatingAxis(edgeAxisA1B2)) return false;
		if (isSeparatingAxis(edgeAxisA1B3)) return false;
		if (isSeparatingAxis(edgeAxisA2B1)) return false;
		if (isSeparatingAxis(edgeAxisA2B2)) return false;
		if (isSeparatingAxis(edgeAxisA2B3)) return false;
		if (isSeparatingAxis(edgeAxisA3B1)) return false;
		if (isSeparatingAxis(edgeAxisA3B2)) return false;
		if (isSeparatingAxis(edgeAxisA3B3)) return false;
		
		return true;
	}
	

bool
isSeparatingAxis(QCAR::Vec3F axis)
{
    // Determine whether or not the given axis separates
    // the globally stored transformed vertices of the two bounding boxes

    float magnitude = axis.data[0] * axis.data[0] + axis.data[1] * axis.data[1] + axis.data[2] * axis.data[2];
    if (magnitude < 0.00001) return false;

    float minA, maxA, minB, maxB;

    minA = maxA = SampleMath::Vec3FDot(dominoTransformedVerticesA[0], axis);
    minB = maxB = SampleMath::Vec3FDot(dominoTransformedVerticesB[0], axis);

    float p;

    for (int i = 1; i < 8; i++) {
        p = SampleMath::Vec3FDot(dominoTransformedVerticesA[i], axis);
        if (p < minA) minA = p;
        if (p > maxA) maxA = p;

        p = SampleMath::Vec3FDot(dominoTransformedVerticesB[i], axis);
        if (p < minB) minB = p;
        if (p > maxB) maxB = p;
    }

    if (maxA < minB) return true;
    if (minA > maxB) return true;

    return false;
}


	void detectCollision(){
		
		
		// Dumping MVM
		LOG("Detecting Collision::");
		if(checkIntersection(ballTransform,courtTransform,false)){
			LOG("->Collision betweeb ball and court");
			handleCollision();
			
		}
		if(checkIntersection(ballTransform,cameraTransform,true)){
			LOG("->Collision betweeb ball and camera");
			handleCollision();
		}
		else {
			LOG("->safe");
		}


		
	}		


	void handleCollision(){
		
		// Demo code  -- to be deleted
		collision=true; // simulate ball bounce
		
		
	}
	
	
	
	// 
	
	void detectAndHandleCollision(){
		
		
		// Dumping MVM
		LOG("Detecting Collision::");
		if(checkIntersection(ballTransform,courtTransform,false)){
			LOG("->Collision betweeb ball and court");
			collision=true;
			
		}
		if(checkIntersection(ballTransform,cameraTransform,true)){
			LOG("->Collision betweeb ball and camera");
			collision=true;
		}
		else {
			LOG("->safe");
			collision=false;
		}
		
		
		
	}		
	















#ifdef __cplusplus
}
#endif
