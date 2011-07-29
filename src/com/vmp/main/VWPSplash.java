/**
 * 
 */
package com.vmp.main;


import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;


/**
 * @author tckb
 *
 */
public class VWPSplash extends Activity {

	private final int STOPSPLASH = -12345;
	private final int SWITCHSPLASH = -245678;
	private int SPLASHTIME=0; 
	private static ImageView vwpSplash;

	
	
	
	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		vwpSplash = (ImageView) findViewById(R.id.imageView2);
		
		SPLASHTIME = getResources().getInteger(R.integer.splash_time);
	
		startTask();
	
		
	}

	private void startTask() {
		
		Log.d("SPLASH","Splash.startTask::Splash time"+new Integer(SPLASHTIME).toString());
		
		
		
		
		 Handler splashHandler = new Handler() {
			
			@Override
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case SWITCHSPLASH:
					Log.d("SPLASH","Splash.startTask.switchSplash");
					VWPSplash.vwpSplash.setVisibility(View.GONE);
					VWPSplash.vwpSplash.setImageResource(R.drawable.ar_qc);
					VWPSplash.vwpSplash.setVisibility(View.VISIBLE);
					break;
				case STOPSPLASH:
					Log.d("SPLASH","Splash.startTask.stopSplash");
					VWPSplash.vwpSplash.setVisibility(View.GONE);
					VWPSplash.vwpSplash.setImageResource(R.drawable.welcome_splash_land);
					setResult(Activity.RESULT_OK);
					finish();
					break;
				}
				
			}

		};
		
			
	splashHandler.sendMessageDelayed(splashHandler.obtainMessage(this.SWITCHSPLASH), this.SPLASHTIME);
	splashHandler.sendMessageDelayed(splashHandler.obtainMessage(this.STOPSPLASH), 2*SPLASHTIME);
		

	}



}
