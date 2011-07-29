/**
 * 
 */
package com.vmp.main;

import com.qualcomm.QCARSamples.ImageTargets.ImageTargets;
import com.vmp.blu.VWPBluDevList;
import com.vmp.blu.VWPBluMain;

import dalvik.system.VMStack;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Layout;
import android.text.format.Time;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.OrientationEventListener;
import android.view.SubMenu;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageSwitcher;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.Toast;
import android.widget.ViewSwitcher.ViewFactory;

/**
 * @author tckb
 * 
 */
public class VMPMain extends Activity {
	private static final int DEV = -7651;
	private static final int LOAD_GAME = 16256;
	private static final int GAME_WON = 12345;
	private static final int GAME_LOST = 54321;
	private static final int BLU_REQ = 17652;
	
	private Message splMsg = new Message();
	private ImageView vwpSplash;
	
	BluetoothAdapter mBluetoothAdapter;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		vwpSplash = (ImageView) findViewById(R.id.imageView2);
		

		// TODO: Remove this after integration
		Toast toast = Toast.makeText(getApplicationContext(),
				"Choose Options from the options menu", Toast.LENGTH_SHORT);
		toast.setGravity(Gravity.BOTTOM, 0, 0);
		toast.show();
		//
		// Get local Bluetooth adapter
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		// If the adapter is null, then Bluetooth is not supported
		if (mBluetoothAdapter == null) {
			Toast.makeText(this, "Bluetooth is not available",
					Toast.LENGTH_LONG).show();
			finish();
			return;
		}
	
		

	}


@Override
public void onBackPressed() {

	
	
	
	AlertDialog.Builder builder = new AlertDialog.Builder(
			this);
	builder.setMessage(
			"Are you sure you want to quit ?")
			.setCancelable(false)
			.setPositiveButton("Yes",
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog,
								int id) {
							setResult(RESULT_CANCELED);
							finish();
							
						}
					})
			.setNegativeButton("No",
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog,
								int id) {
							// Absorb it and do nothing
						}
					});
	AlertDialog alert = builder.create();
	alert.show();
	
	
	
	
	
	
}
	
	
	// Pop up the options menu as soon as the activity starts
	@Override
	public void onAttachedToWindow() {
		super.onAttachedToWindow();
		openOptionsMenu();
	}

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();

		if (!mBluetoothAdapter.isEnabled()) {
			Intent enableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, BLU_REQ);

		}

	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub

		this.openOptionsMenu();

		return super.onTouchEvent(event);

	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {

		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		this.openOptionsMenu();

		Toast toast;
		if (newConfig.orientation == newConfig.ORIENTATION_LANDSCAPE) {
			vwpSplash.setImageResource(R.drawable.welcome_splash_land);
			toast = Toast.makeText(getApplicationContext(), "Mode: Landscape",
					Toast.LENGTH_SHORT);
			toast.setGravity(Gravity.BOTTOM, 0, 0);
			toast.show();

		} else if (newConfig.orientation == newConfig.ORIENTATION_PORTRAIT) {
			vwpSplash.setImageResource(R.drawable.welcome_screen_port);
			toast = Toast.makeText(getApplicationContext(), "Mode: Potrait",
					Toast.LENGTH_SHORT);
			toast.setGravity(Gravity.BOTTOM, 0, 0);
			toast.show();
		} else if (newConfig.orientation == newConfig.ORIENTATION_UNDEFINED) {
			vwpSplash.setImageResource(R.drawable.welcome_screen_port);
			toast = Toast.makeText(getApplicationContext(), "Mode: Undefined",
					Toast.LENGTH_SHORT);
			toast.setGravity(Gravity.BOTTOM, 0, 0);
			toast.show();
		}

	}

	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.mainmenu, menu);

		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		final Toast toast;
		switch (item.getItemId()) {

		

		case R.id.GAME_HELP:

			Dialog dialog = new Dialog(this);

			dialog.setContentView(R.layout.about);
			dialog.setTitle("Help");

			TextView text = (TextView) dialog.findViewById(R.id.text);
			text.setText("Please Check the tutorial video for guidelines");
			ImageView image = (ImageView) dialog.findViewById(R.id.image);
			image.setImageResource(R.drawable.icon_vwp);
			
			dialog.show();

			break;

		case R.id.GAME_SET:

			 Intent settingsActivity = new Intent(getBaseContext(),
                     VWPPref.class);
			 startActivity(settingsActivity);
			   
//			    Log.d("VMPMain", "Preferences:");
//				Log.d("VMPMain", "isSoundEnabled:"+VWPPref.isSoundEnabled());
//				Log.d("VMPMain", "isVibrateEnabled:"+VWPPref.isVibrateEnabled());
//				Log.d("VMPMain", "ballSpeed:"+VWPPref.getBallSpeed());
//				Log.d("VMPMain", "court texture:"+VWPPref.getCourtTex());
				
			
			break;

		case R.id.GAME_ABOUT:

			
			 dialog = new Dialog(this);

			dialog.setContentView(R.layout.about);
			dialog.setTitle("About VWP");

			 text = (TextView) dialog.findViewById(R.id.text);
			text.setText(R.string.app_about);
			 image = (ImageView) dialog.findViewById(R.id.image);
			image.setImageResource(R.drawable.icon_vwp);
			
			dialog.show();

			break;
		
			
		case R.id.SINGLE_PLAYER:

			final Intent intent = new Intent(this, ImageTargets.class);
			startActivityForResult(intent, LOAD_GAME);

			break;
		case R.id.MULTI_PLAYER:

			final Intent menu = new Intent(this, VWPBluMain.class);
			startActivityForResult(menu,R.id.MULTI_PLAYER);

			break;

		}
		return super.onOptionsItemSelected(item);

	}
	
	
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		


		
		
		
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);

		switch (requestCode) {
		case LOAD_GAME:
			if (resultCode == GAME_WON) { // If the user go back to menu
											// manually

				AlertDialog.Builder builder = new AlertDialog.Builder(this);
				builder.setMessage("Congratualtions, you won the game! ")
						.setCancelable(false)
						.setPositiveButton("Awesome",
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int id) {

										// Silently accept
									}
								});

				AlertDialog alert = builder.create();
				alert.show();
			}

			if (resultCode == GAME_LOST) { // If the user go back to menu
											// manually to the menu

				AlertDialog.Builder builder = new AlertDialog.Builder(this);
				builder.setMessage(
						"Unfortuantely, you lost the game, better luck next time!")
						.setCancelable(false)
						.setPositiveButton("Try again!",
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int id) {

										// finish();
									}
								});

				AlertDialog alert = builder.create();
				alert.show();
			}
			break;
		case BLU_REQ:
			if (resultCode == Activity.RESULT_CANCELED ) { 
				Toast.makeText(getApplicationContext(), "Bluetooth must be enabled to play the game!", Toast.LENGTH_LONG).show();
				setResult(Activity.RESULT_CANCELED);
				finish();
			}
			break;
		}

	}
	

		
	
		
		
		

	
	
	
	

}
