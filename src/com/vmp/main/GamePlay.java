package com.vmp.main;

import com.qualcomm.QCARSamples.ImageTargets.DebugLog;

import android.app.Activity;
import android.content.Context;
import android.media.MediaPlayer;
import android.os.Vibrator;

public class GamePlay{

	private  boolean isRunning=true;
	private Activity gameActivity;
	public static int yourScore=0; //workaround
	private int oppScore=0;
	public static int MAX_SCORE=5;
	public static boolean gameStarted=true;

	public static boolean oppWon=false;

	 public native boolean isBallBounce();
	 public native boolean isBallHit();
	
	public void initGame(){
	     yourScore =0;
	
	}
	/**
	 * @return the yourScore
	 */
	public int getYourScore() {
		return yourScore;
	}
	/**
	 * @param yourScore the yourScore to set
	 */
	public int increaseYourScore() {
		this.yourScore++;
		return yourScore;
		}
	/**
	 * @return the oppScore
	 */
	public int getOppScore() {
		return oppScore;
	}
	/**
	 * @param oppScore the oppScore to set
	 */
	public int increaseOppScore() {
		this.oppScore++;
		return oppScore;
	}
	
	public int decreaseYourScore(){
		return this.yourScore--;
	}
	
	public int decreaseOppScore(){
		return this.oppScore--;
	}
	
	
	 public boolean checkYourWin(){
		 if(yourScore==MAX_SCORE){
			 return true;
		 }
		 
		 return false;
	 }
	
	 public boolean checkOppWin(){
		 if(oppScore==MAX_SCORE){
			 return true;
		 }
		 
		 return false;
	 }
	
	
}
