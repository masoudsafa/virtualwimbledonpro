/*
 *  BoundingBoxUnitCube.cpp
 *  
 *
 *  Created by Chandra Tungathurthi on 7/13/11.
 *  Copyright 2011 RWTH Aachen. All rights reserved.
 *
 */


// Collision detection - based on unit cube

void initBallBaseVertices();
void initCourtBaseVertices();
void initDominoNormals();



void
initBallBaseVertices()
{
	// Initialize a set of vertices describing the unit cube
	
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
	// Initialize a set of vertices describing the unit cube
	
	
	
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
initDominoNormals()
{
	// Initialize a set of normals for the unit cube
	
	dominoNormals[0] = QCAR::Vec3F(1, 0, 0);
	dominoNormals[1] = QCAR::Vec3F(0, 1, 0);
	dominoNormals[2] = QCAR::Vec3F(0, 0, 1);
}

JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_onQCARInitializedNative(JNIEnv *env, jobject obj)
{
    // Comment in to enable tracking of up to 2 targets simultaneously and
    // split the work over multiple frames:
	//     QCAR::setHint(QCAR::HINT_MAX_SIMULTANEOUS_IMAGE_TARGETS, 2);
	//     QCAR::setHint(QCAR::HINT_IMAGE_TARGET_MULTI_FRAME_ENABLED, 1);
	
	// Initialise the unit cube
	initBallBaseVertices();
	initCourtBaseVertices();
	initDominoNormals();

	// Store the java environment for later use
	// Note that this environment is only safe for use in this thread
    javaEnv = env;
    
	// Store the calling object for later use
	// Make a global reference to keep it valid beyond the scope of this function
    javaObj = env->NewGlobalRef(obj);
    
    // Store the class of the calling object for later use
    jclass objClass = env->GetObjectClass(obj);
    javaClass = (jclass) env->NewGlobalRef(objClass);
	
	
	
	
}

