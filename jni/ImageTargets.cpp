/*==============================================================================
            Copyright (c) 2010-2011 QUALCOMM Incorporated.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
            
@file 
    ImageTargets.cpp

@brief
    Sample for ImageTargets

==============================================================================*/

// One big header file!
#include "VWPHeader.h"

// WARNING: Do not use the both header files at the same time as, both provide the same but different impl

//#include "BoundingBoxUnitCube.h"  
#include "BoundingBox.h" 



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


// onQCARInitializedNative(JNIEnv *env, jobject obj) -> "BoundingBox.h"  &  BoundingBoxUnitCube.h


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
		float* transformPtr = &transformCourt.data[0];
		courtTransform = SampleMath::Matrix44FIdentity();
		
		LOG("Court transform:Before");
		dumpMVM(&courtTransform.data[0]);
	
        SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, 4*kObjectScale,
                                    transformPtr);
		
		SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, 4*kObjectScale, &courtTransform.data[0]);
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

        glBindTexture(GL_TEXTURE_2D, textures[4]->mTextureID);
		
		
		
		
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

        glEnableVertexAttribArray(vertexHandle);
        glEnableVertexAttribArray(normalHandle);

        glBindTexture(GL_TEXTURE_2D, textures[2]->mTextureID);
        glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                           (GLfloat*)&modelViewProjection.data[0] );
        glDrawArrays(GL_LINES, 0, justnetNumVerts);
		

/////////////////////////////////////////////////////////////////////////////////////////
//        Ball																			//
/////////////////////////////////////////////////////////////////////////////////////////      
		
		transformBall =  QCAR::Tool::convertPose2GLMatrix(trackable->getPose());
		transformPtr = &transformBall.data[0];
		ballTransform = SampleMath::Matrix44FIdentity();
     
		
		animateBall(transformPtr,&modelViewProjection.data[0]);


		

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
		
	detectCollision();
		
		
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

// J0sh's animateBall: Ver 2.0 :P -buggy!!
	

void
animateBall(float* transform, float* projection)
{
    static float rotateBallAngle = 360.0f;
    static double prevTime = getCurrentTime();
    double time = getCurrentTime();             // Get real time difference
    float dt = (float)(time-prevTime);          // from frame to frame
	const double value1 = sin((rotateBallAngle * M_PI) / 180.0);
	static int value2 = -1; //This tells you the side of the court.  Used for translation.
	//Collision detection needs to be thread-safe.. Let's fake it instad.
	if(lockBallTranslate) {
		//Backwall.  The user did not tap fast enough, award a point
		if (value1 > 0.95) {
			direction*=-1;	 // I think this where the "collision" is ? - Chandra
			
			missedHitMessage();
			playSoundEffect(BALL_WALL_HIT,1.0f); // would sound ball bounce on Wall
				
			
			
		  }
		//We are going to unlock the translate.  The ball is about to travel over the net
		//This means the ball will be in the center.
		else if(value1 < 0) {
			rotateBallAngle-=180;
			lockBallTranslate=false; //release lock.
			value2 *=-1; //Prepare for the next lockTranslate
			
			
			playSoundEffect(BALL_BOUNCE,1.0f); // would sound ball bounce on court (left?)

			
			
		}
	} else {
		//Ball is about to collide on one side of the court.
		if(value1 < 0) {
			rotateBallAngle-=180;
			lockBallTranslate=true; //Lock the translate to move.
			
			
			playSoundEffect(BALL_BOUNCE,1.0f);  // would sound ball bounce court ( right? )

			
		}
	} 
	
	//Somehow we went below the court, adjust
	if(value1 < -0.05)
		rotateBallAngle-=90; //Needs to be adjusted
		 //rotateBallAngle+=(7*direction);
	
	dt *= direction; //This makes the difference positive or negative
	rotateBallAngle += dt * 180.0f/3.1415f;     // Animate angle based on time

	animateBallTranslation(transform, projection, rotateBallAngle,value2);
	animateBallTranslation(&ballTransform.data[0], NULL, rotateBallAngle,value2);
	prevTime = time;							
	//Dumping the projection
 //   LOG("Dumping the MVM");
	//dumpMVM(transform);
	//LOG("------------");
	LOG(">rotated the ball %f degrees", rotateBallAngle);
	LOG(">Finishesd animateBall\n>------------");
	LOG("->lockBallTranslate %i: direction: %i collision: %i",lockBallTranslate,direction,collision);
	
}




/*
void
animateBall(float* transform, float* projection)
{
	static float rotateBallAngle = 0.0f;
	static double prevTime = getCurrentTime();
	double time = getCurrentTime();             // Get real time difference
	float dt = (float)(time-prevTime);          // from frame to frame
	const double value1 = sin((rotateBallAngle * M_PI) / 180.0);
	static int value2 = -1;
	//Collision detection needs to be thread-safe.. Let's fake it instad.
	if(lockBallTranslate) {
		if (value1 > 0.95) {
			direction*=-1;
			
			missedHitMessage();
			playSoundEffect(BALL_WALL_HIT,1.0f); // would sound ball bounce on Wall
			
			
			
		}
		else if(value1 < 0.05) {
			rotateBallAngle+=(177*direction);
			lockBallTranslate=false;
			value2 *=-1;
			
			playSoundEffect(BALL_BOUNCE,1.0f); // would sound ball bounce on court (left?)

			
			
		}
	} else {
		if(value1 < 0.05) {
			rotateBallAngle+=(177*direction);
			lockBallTranslate=true; 
			
			playSoundEffect(BALL_BOUNCE,1.0f); // would sound ball bounce on court (left?)

			
		}
	}
	
	while(sin((rotateBallAngle * M_PI) / 180.0) < 0) {
		rotateBallAngle+=(7*direction);
	}
	
	if(rotateBallAngle > 1440) rotateBallAngle-=1440;
	if(rotateBallAngle < -1440) rotateBallAngle+=1440;
	
	dt *= direction; //This makes the difference positive or negative
	rotateBallAngle += dt * 180.0f/3.1415f;     // Animate angle based on time
	
	animateBallTranslation(transform, projection, rotateBallAngle,value2);
	animateBallTranslation(&ballTransform.data[0], NULL, rotateBallAngle,value2);
	prevTime = time;       
	//Dumping the projection
	//   LOG("Dumping the MVM");
	//dumpMVM(transform);
	//LOG("------------");
	LOG("->rotated the ball %f degrees", rotateBallAngle);
	LOG(">Finishesd animateBall\n>------------");
	LOG("->lockBallTranslate %i: direction: %i collision: %i",lockBallTranslate,direction,collision);

}
*/

void
animateBallTranslation(float* transform, float* projection, float rotateBallAngle, int value) {
	//TODO:  Pass in a boolean instead relying on a global variable.
	if(lockBallTranslate) {
			int temp = -1 * value;
			SampleUtils::translatePoseMatrix(0.0f,temp * kObjectScale/2,0.0f,transform);
			SampleUtils::translatePoseMatrix(0.0f,temp * kObjectScale/2,0.0f,&ballTransform.data[0]);	
		}
	//LOG("->>lockBallTranslate %i: rotateBallAngle %f: direction: %i collision: %i",lockBallTranslate,rotateBallAngle,direction,collision);
	SampleUtils::rotatePoseMatrix(rotateBallAngle, 1.0f, 0.0f, 0.0f,
								   transform);							   
	SampleUtils::translatePoseMatrix(0.0f, kObjectScale/4, 0.0f,
									  transform);
	 // SampleUtils::translatePoseMatrix(-10, 0.0f, 0.0f,
									 // transform);
	SampleUtils::scalePoseMatrix((kObjectScale/32), (kObjectScale/32), (kObjectScale/32),
										   transform);
	if (projection != NULL) {															   
		SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
									transform ,
									projection);
	}
}
	
	
	

// J0sh animateBall : Ver 1.0  -- buggy
	
	
	
//	void
//	animateBall(float* transform, float* projection)
//	{
//		static float rotateBallAngle = 0.0f;
//		static double prevTime = getCurrentTime();
//		double time = getCurrentTime();             // Get real time difference
//		float dt = (float)(time-prevTime);          // from frame to frame
//		if(collision) {
//			LOG("------------");
//			// LOG("After the MVM");
//			// dumpMVM(transform);
//			// LOG("------------");
//			// LOG("Is Projection? %i", projection == NULL);
//			
//			//If we have a lock, then we had another collision.
//			rotateBallAngle-=180;
//			lockBallTranslate = true;
//			// collision=false;
//		} 
//		const double value = sin((rotateBallAngle * M_PI) / 180.0);
//		if(lockBallTranslate) {
//			if (value > 0.95 && value < 1.05) {
//				rotateBallAngle-=90;
//				direction*=-1;
//				lockBallTranslate=false;
//			}
//		}
//		if(value < -0.05) {
//			rotateBallAngle-=180;
//		}
//		dt *= direction; //This makes the difference positive or negative
//		rotateBallAngle += dt * 180.0f/3.1415f;     // Animate angle based on time
//		
//		animateBallTranslation(transform, projection, rotateBallAngle);
//		animateBallTranslation(&ballTransform.data[0], NULL, rotateBallAngle);
//		prevTime = time;
//		//Dumping the projection
//		//   LOG("Dumping the MVM");
//		//dumpMVM(transform);
//		//LOG("------------");
//		LOG(">rotated the ball %f degrees", rotateBallAngle);
//		LOG(">Finishesd animateBall\n>------------");
//		LOG("->lockBallTranslate %i: value %f: direction: %i collision: %i",lockBallTranslate,value,direction,collision);
//	}
//	
//	void
//	animateBallTranslation(float* transform, float* projection, float rotateBallAngle) {
//		//TODO:  Pass in a boolean instead relying on a global variable.
//		if(lockBallTranslate) {
//			int temp = -1 * direction;
//			SampleUtils::translatePoseMatrix(0.0f,kObjectScale/2,0.0f,transform);
//			SampleUtils::translatePoseMatrix(0.0f,kObjectScale/2,0.0f,&ballTransform.data[0]);
//		}
//		//LOG("->>lockBallTranslate %i: rotateBallAngle %f: direction: %i collision: %i",lockBallTranslate,rotateBallAngle,direction,collision);
//		SampleUtils::rotatePoseMatrix(rotateBallAngle, 1.0f, 0.0f, 0.0f,
//									  transform);   
//		SampleUtils::translatePoseMatrix(0.0f, kObjectScale/4, 0.0f,
//										 transform);
//		// SampleUtils::translatePoseMatrix(-10, 0.0f, 0.0f,
//		// transform);
//		SampleUtils::scalePoseMatrix((kObjectScale/32), (kObjectScale/32), (kObjectScale/32),
//									 transform);
//		if (projection != NULL) {   
//			SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
//transform ,
//projection);
//}
//}
//	
//	
//	
//	
	
	


	
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



void dumpMVM(const float* mvm){
	LOG("Dumping MVM:");
		SampleUtils::printMatrix(mvm);

}


// Bounding box code in "BoundingBox.h"


	
bool
checkIntersection(QCAR::Matrix44F transformA, QCAR::Matrix44F transformB)
{
    // Use the separating axis theorem to determine whether or not
    // two object-oriented bounding boxes are intersecting

    transformA = SampleMath::Matrix44FTranspose(transformA);
    transformB = SampleMath::Matrix44FTranspose(transformB);

//	LOG("--Printing tranforms--");
//	LOG("Ball::");
//	dumpMVM(&transformA.data[0]);
//	LOG("Court::");
//	dumpMVM(&transformB.data[0]);
	
	
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
        dominoTransformedVerticesB[i] = SampleMath::Vec3FTransform(courtBaseVertices[i], transformB);
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
		LOG("Value of A is: %f",p);
        p = SampleMath::Vec3FDot(dominoTransformedVerticesB[i], axis);
        if (p < minB) minB = p;
        if (p > maxB) maxB = p;
		LOG("Value of B is: %f",p);
    }
	LOG("->minA:=%f maxA:=%f minB:=%f maxB:=%f",minA,maxA,minB,maxB);
    if (maxA <= minB) return true;
    if (minA >= maxB) return true;
	LOG("->>Returning False....  maxA <= minB %i minA >= maxB %i", maxA <= minB, minA >= maxB);
    return false;
}


	void detectCollision(){
		static bool isIntersecting = false;
		collision = false;
		// Dumping MVM
	//	LOG("Detecting Collision::");
		if(checkIntersection(ballTransform,courtTransform)){
			LOG("->Collision");
			collision=true;
			
			//playSoundEffect(BALL_BOUNCE,1.0f); 
			
		}
		

		
	}		


	void handleCollision(){
		
		// Demo code  -- to be deleted
		collision=true; // simulate ball bounce
		
		
	}
	


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_nativeTouchEvent(JNIEnv* , jobject, jint actionType, jint pointerId, jfloat x, jfloat y)
{
   TouchEvent* touchEvent;
LOG(">->Entered the nativeTouchEvent");
// Determine which finger this event represents

//No idea what this does...
if (pointerId == 0) {
touchEvent = &touch1;
} else if (pointerId == 1) {
touchEvent = &touch2;
} else {
return;
}

//I know what this does!! :)
if (actionType == ACTION_UP && lockBallTranslate) {
	    	direction*=-1; //Change direction)
		
		updateScore();
		
		vibratePhoneShort();
		//playSoundEffect(CROWD_CHEER,1.0f);
		
		
    }

}


	
	// ----------------------------------------------------------------------------
	// Java static methods called from native
	// ----------------------------------------------------------------------------
	
	void
	playSoundEffect(int soundIndex, float volume)
	{
		
		LOG("->Playinng some sound");
		// Sound playback is always handled by the Android SDK
		// Use the environment and class stored in initNativeCallback
		// to call a Java method that plays a sound
		jmethodID method = javaEnv->GetMethodID(javaClass, "playSoundNative", "(IF)V");
		javaEnv->CallVoidMethod(javaObj, method, soundIndex, volume);
	}
	
	
	
	void
	showServeButton()
	{
		// For this application, buttons are handled by the Android SDK
		// Use the environment and class stored in initNativeCallback
		// to call a Java method that toggles the start button
		jmethodID method = javaEnv->GetMethodID(javaClass, "showServeButtonNative", "()V");
		javaEnv->CallVoidMethod(javaObj, method);
	}
	
	
	void
	displayMessage(char* message)
	{
		// Use the environment and class stored in initNativeCallback
		// to call a Java method that displays a message via a toast
		jstring js = javaEnv->NewStringUTF(message);
		jmethodID method = javaEnv->GetMethodID(javaClass, "displayMessageNative", "(Ljava/lang/String;)V");
		javaEnv->CallVoidMethod(javaObj, method, js);
	}
	
	void vibratePhoneShort(){
		jmethodID method = javaEnv->GetMethodID(javaClass, "vibratePhoneShortNative", "()V");
		javaEnv->CallVoidMethod(javaObj, method);
	}

	void vibratePhoneLong(){
		jmethodID method = javaEnv->GetMethodID(javaClass, "vibratePhoneLongNative", "()V");
		javaEnv->CallVoidMethod(javaObj, method);
	}


// shud call the java class but, for now lets update it here!

void updateScore(){
	
	jmethodID method = javaEnv->GetMethodID(javaClass, "updateScoreNative", "()V");
	javaEnv->CallVoidMethod(javaObj, method);


	
}

void missedHitMessage(){
	
	jmethodID method = javaEnv->GetMethodID(javaClass, "showMissedMessageNative", "()V");
	javaEnv->CallVoidMethod(javaObj, method);
	
	
	
}
#ifdef __cplusplus
}
#endif
