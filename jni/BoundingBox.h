/*
 *  boundingBox
 *  
 *
 *  Created by Chandra Tungathurthi on 7/13/11.
 *  Copyright 2011 RWTH Aachen. All rights reserved.
 *
 */





// Code for collision detection - J0sh impl

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



void initBallBaseVertices(struct BoundingBox *box);
void initCourtBaseVertices(struct BoundingBox *box);
void initDominoNormals();
void constructBox(struct BoundingBox *box, int numOfVerts, float verts[]);




void
initBallBaseVertices(struct BoundingBox *box)
{
	// Initialize a set of vertices describing the unit cube
	
	ballBaseVertices[0] = QCAR::Vec3F(box->max.x, box->max.y, box->max.z);
	ballBaseVertices[1] = QCAR::Vec3F(box->min.x, box->max.y, box->max.z);
	ballBaseVertices[2] = QCAR::Vec3F(box->max.x, box->min.y, box->max.z);
	ballBaseVertices[3] = QCAR::Vec3F(box->min.x, box->min.y, box->max.z);
	ballBaseVertices[4] = QCAR::Vec3F(box->max.x, box->max.y, box->min.z);
	ballBaseVertices[5] = QCAR::Vec3F(box->min.x, box->max.y, box->min.z);
	ballBaseVertices[6] = QCAR::Vec3F(box->max.x, box->min.y, box->min.z);
	ballBaseVertices[7] = QCAR::Vec3F(box->min.x, box->min.y, box->min.z);
}

void
initCourtBaseVertices(struct BoundingBox *box)
{
	// Initialize a set of vertices describing the unit cube
	
	courtBaseVertices[0] = QCAR::Vec3F(box->max.x, box->max.y, box->max.z);
	courtBaseVertices[1] = QCAR::Vec3F(box->min.x, box->max.y, box->max.z);
	courtBaseVertices[2] = QCAR::Vec3F(box->max.x, box->min.y, box->max.z);
	courtBaseVertices[3] = QCAR::Vec3F(box->min.x, box->min.y, box->max.z);
	courtBaseVertices[4] = QCAR::Vec3F(box->max.x, box->max.y, box->min.z);
	courtBaseVertices[5] = QCAR::Vec3F(box->min.x, box->max.y, box->min.z);
	courtBaseVertices[6] = QCAR::Vec3F(box->max.x, box->min.y, box->min.z);
	courtBaseVertices[7] = QCAR::Vec3F(box->min.x, box->min.y, box->min.z);
}	


void
initDominoNormals()
{
	// Initialize a set of normals for the unit cube
	
	dominoNormals[0] = QCAR::Vec3F(1, 0, 0);
	dominoNormals[1] = QCAR::Vec3F(0, 1, 0);
	dominoNormals[2] = QCAR::Vec3F(0, 0, 1);
}


void constructBox(struct BoundingBox *box, int numOfVerts, float verts[]) {
    for (int i = 0; i < (numOfVerts / 3); i += 3) {
        if (box->min.x > verts[i])
            box->min.x = verts[i];
        if (box->min.y > verts[i + 1])
            box->min.y = verts[i + 1];
        if (box->min.z > verts[i + 2])
            box->min.z = verts[i + 2];
        if (box->max.x < verts[i])
            box->max.x = verts[i];
        if (box->max.y < verts[i + 1])
            box->max.y = verts[i + 1];
        if (box->max.z < verts[i + 2])
            box->max.z = verts[i + 2];
    }    
}


JNIEXPORT void JNICALL
Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_onQCARInitializedNative(JNIEnv *env, jobject obj)
{
	// Comment in to enable tracking of up to 2 targets simultaneously and
	// split the work over multiple frames:
	//     QCAR::setHint(QCAR::HINT_MAX_SIMULTANEOUS_IMAGE_TARGETS, 2);
	//     QCAR::setHint(QCAR::HINT_IMAGE_TARGET_MULTI_FRAME_ENABLED, 1);
	
	
    //Calculate the circle's bounding box here
	BoundingBox* boxBall = new BoundingBox;
	BoundingBox* boxCourt = new BoundingBox;
	
	constructBox(boxBall,ballNumVerts, ballVerts);
	constructBox(boxCourt,boundaryNumVerts, boundaryVerts);	 
	
	// Initialise the unit cube
	initBallBaseVertices(boxBall);
	initCourtBaseVertices(boxCourt);
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


