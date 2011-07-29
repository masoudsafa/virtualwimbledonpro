/*==============================================================================
            Copyright (c) 2010-2011 QUALCOMM Incorporated.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
            
@file 
    ImageTargetsRenderer.java

@brief
    Sample for ImageTargets

==============================================================================*/


package com.qualcomm.QCARSamples.ImageTargets;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.media.MediaPlayer;
import android.opengl.GLSurfaceView;

import com.qualcomm.QCAR.QCAR;


/** The renderer class for the ImageTargets sample. */
public class ImageTargetsRenderer implements GLSurfaceView.Renderer
{
    public boolean mIsActive = false;
    
   //private Boolean lock = new Boolean(false);
   private Lock aLock = new ReentrantLock();


    
    /** Native function for initializing the renderer. */
    public native void initRendering();
    
    
    /** Native function to update the renderer. */
    public native void updateRendering(int width, int height);

    
    /** Called when the surface is created or recreated. */
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        DebugLog.LOGD("GLRenderer::onSurfaceCreated");

        // Call native function to initialize rendering:
        initRendering();
        
        // Call QCAR function to (re)initialize rendering after first use
        // or after OpenGL ES context was lost (e.g. after onPause/onResume):
        QCAR.onSurfaceCreated();
        
        
        
        
        
    }
    
    
    /** Called when the surface changed size. */
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        DebugLog.LOGD("GLRenderer::onSurfaceChanged");
        
        // Call native function to update rendering when render surface parameters have changed:
        updateRendering(width, height);

        // Call QCAR function to handle render surface size changes:
        QCAR.onSurfaceChanged(width, height);
    }    
    
    /** The native render function. */    
    public native void renderFrame();
    
   
    public native float[] getCourtModelView();
    public native float[] getBallModelView();
    public native void dumpBallMVM();
    
    
    /** Called to draw the current frame. */
    public void onDrawFrame(GL10 gl)
    {
    	DebugLog.LOGD("onDrawFrame::renderFrame:java:Ball MVM");
    	if (!mIsActive)
            return;

        // Call our native function to render content
    	
    	try {
    		this.aLock.lock();
    		renderFrame();
    	} finally {
    		this.aLock.unlock();
    	}
        
        
        float[] mvmBall = getBallModelView() ;
        for(int i=0;i<mvmBall.length;i+=4)
        	DebugLog.LOGD("["+new Float(mvmBall[i]).toString()+" "
        			+new Float(mvmBall[i+1]).toString()+" "
        			+new Float(mvmBall[i+2]).toString()+" "
        			+new Float(mvmBall[i+3]).toString()+"]");
      
        
        
        
    }
    
    
    
    
    
    
}
