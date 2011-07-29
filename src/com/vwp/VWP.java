package com.vwp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.vmp.main.VMPMain;
import com.vmp.main.VWPSplash;

public class VWP extends Activity {

	
	private static final int SPLASH_ACT = 12121;
	private static final int MAIN_ACT = 21212;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.d("MAIN","onCreate");

		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		startSplashActivity();
	
	}
	
	
	
	private void startSplashActivity() {
		Log.d("MAIN","startSplash");

	    Intent intent = new Intent(this, VWPSplash.class);
		startActivityForResult(intent, SPLASH_ACT);

		
	}
	
@Override
protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	Log.d("MAIN","onActivityResult");
	
	// TODO Auto-generated method stub
	if(requestCode == SPLASH_ACT){
		if(resultCode == Activity.RESULT_OK)
		  startMainActivity();
	}
//	if(requestCode == MAIN_ACT){
//		
//	}
	if(resultCode == Activity.RESULT_CANCELED)
		  finish();
}
	private void startMainActivity() {
		Log.d("MAIN","startMainActivity");

	    Intent intent = new Intent(this, VMPMain.class);
		startActivityForResult(intent, MAIN_ACT);

		
	}



	/* (non-Javadoc)
	 * @see android.app.Activity#onPause()
	 */
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		Log.d("MAIN","onPause");
		super.onPause();
		
	}



	/* (non-Javadoc)
	 * @see android.app.Activity#onRestart()
	 */
	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		Log.d("MAIN","onRestart");
		super.onRestart();
	}



	/* (non-Javadoc)
	 * @see android.app.Activity#onResume()
	 */
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		Log.d("MAIN","onResume");
		super.onResume();
	}



	/* (non-Javadoc)
	 * @see android.app.Activity#onStart()
	 */
	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		Log.d("MAIN","onStart");
		super.onStart();
	}



	/* (non-Javadoc)
	 * @see android.app.Activity#onStop()
	 */
	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		Log.d("MAIN","onStop");
		super.onStop();
	}
	
}
