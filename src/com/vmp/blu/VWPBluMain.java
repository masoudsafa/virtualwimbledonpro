/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.vmp.blu;

import com.qualcomm.QCARSamples.ImageTargets.ImageTargets;
import com.vmp.blu.VWPBluMain;
import com.vmp.main.GamePlay;
import com.vmp.main.R;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

/**
 * This is the main Activity that displays the current chat session.
 * 
 * @param <MyActivity>
 */
public class VWPBluMain extends Activity {
	// Debugging
	private static final String TAG = "VWPBluMain";
	private static final boolean D = true;
	private boolean perm;
	private boolean isAccepting = false;

	// Message types sent from the VWPBluService Handler
	public static final int MESSAGE_STATE_CHANGE = 1;
	public static final int MESSAGE_READ = 2;
	public static final int MESSAGE_WRITE = 3;
	public static final int MESSAGE_DEVICE_NAME = 4;
	public static final int MESSAGE_TOAST = 5;

	// Key names received from the VWPBluService Handler
	public static final String DEVICE_NAME = "device_name";
	public static final String TOAST = "toast";

	// Intent request codes
	protected static final int REQUEST_CONNECT_DEVICE = 1;
	private static final int REQUEST_ENABLE_BT = 2;
	protected static final int CONN_LOST = 88976;
	protected static final int CONN_FAIL = 99698;
	public static final int CONN_REJ = 98728;

	public static final int CNTG = -987292;
	protected static final int BLU_CONN_REQ = 88769;
	private static final int LOAD_GAME = 77289;

	// Layout Views
	private TextView mTitle;
	private ListView mConversationView;
	private static EditText mOutEditText;
	private Button mSendButton;

	// Name of the connected device
	private String mConnectedDeviceName = null;
	// Array adapter for the conversation thread
	private ArrayAdapter<String> mConversationArrayAdapter;
	// String buffer for outgoing messages
	private static StringBuffer mOutStringBuffer;
	// Local Bluetooth adapter
	private BluetoothAdapter mBluetoothAdapter = null;
	// Member object for the chat services
	protected static VWPBluService mChatService = null;

	private static final int GAME_START = 12198;
	private static final int GAME_HELP = -98656;
	private static final int GAME_SET = 88789;
	private static final int GAME_EXIT = 786675;
	public static final int GAME_WON = 12345;
	protected static final int BLU_GAME_WON = 887668;
	public static final int GAME_LOST = 54321;
	private static final int BLU_GAME_LOST = 87654;
	public static final int BLU_GAME_OPP_WON = 98754;
	private ImageView vwpSplash;
	ProgressDialog progressDialog;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (D)
			Log.e(TAG, "+++ ON CREATE +++");

		// Get local Bluetooth adapter
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		setContentView(R.layout.main);
		vwpSplash = (ImageView) findViewById(R.id.imageView2);
		vwpSplash.setImageResource(R.drawable.vmp_back_land);		
	    
		Toast toast = Toast.makeText(getApplicationContext(),
				"Choose Options from the options menu", Toast.LENGTH_SHORT);
		toast.setGravity(Gravity.BOTTOM, 0, 0);
		toast.show();

		progressDialog = new ProgressDialog(this);

	}

	@Override
	public void onStart() {
		super.onStart();
		if (D)
			Log.e(TAG, "++ ON START ++");
		//
		// // If BT is not on, request that it be enabled.
		// // setupChat() will then be called during onActivityResult
		// if (!mBluetoothAdapter.isEnabled()) {
		// Intent enableIntent = new
		// Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		// startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
		// // Otherwise, setup the chat session
		// } else {
		if (mChatService == null)
			setupBluMenu();
		// }
	}

	@Override
	public synchronized void onResume() {
		super.onResume();
		if (D)
			Log.e(TAG, "+ ON RESUME +");

		// Performing this check in onResume() covers the case in which BT was
		// not enabled during onStart(), so we were paused to enable it...
		// onResume() will be called when ACTION_REQUEST_ENABLE activity
		// returns.
		if (mChatService != null) {
			// Only if the state is STATE_NONE, do we know that we haven't
			// started already
			if (mChatService.getState() == VWPBluService.STATE_NONE) {
				// Start the Bluetooth chat services
				mChatService.start();
				
				
				progressDialog.dismiss();

			}
		}
	}

	private void setupBluMenu() {

		// Initialize the VWPBluService to perform bluetooth connections
		mChatService = new VWPBluService(this, mHandler);

		// Initialize the buffer for outgoing messages
		mOutStringBuffer = new StringBuffer("");

	}

	@Override
	public synchronized void onPause() {
		super.onPause();
		if (D)
			Log.e(TAG, "- ON PAUSE -");
	}

	@Override
	public void onStop() {
		super.onStop();
		if (D)
			Log.e(TAG, "-- ON STOP --");
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		// Stop the Bluetooth chat services
		if (mChatService != null)
			mChatService.stop();
		if (D)
			Log.e(TAG, "--- ON DESTROY ---");
	}

	private void ensureDiscoverable() {
		if (D)
			Log.d(TAG, "ensure discoverable");
		if (mBluetoothAdapter.getScanMode() != BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
			Intent discoverableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			discoverableIntent.putExtra(
					BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
			startActivity(discoverableIntent);
		}
	}

	/**
	 * Sends a message.
	 * 
	 * @param message
	 *            A string of text to send.
	 */
	public static void sendMessage(String message) {
		// Check that we're actually connected before trying anything
		if (mChatService.getState() != VWPBluService.STATE_CONNECTED) {
			// Toast.makeText(this, R.string.not_connected,
			// Toast.LENGTH_SHORT).show();
			return;
		}

		// Check that there's actually something to send
		if (message.length() > 0) {
			// Get the message bytes and tell the VWPBluService to write
			byte[] send = message.getBytes();
			mChatService.write(send);

			// Reset out string buffer to zero and clear the edit text field
			mOutStringBuffer.setLength(0);
			mOutEditText.setText(mOutStringBuffer);
		}
	}

	// The Handler that gets information back from the VWPBluService
	private final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			String messg = null;
			progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);

			switch (msg.what) {
			case MESSAGE_STATE_CHANGE:
				if (D)
					Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
				switch (msg.arg1) {
				case VWPBluService.STATE_CONNECTED:
					progressDialog.dismiss();

					// Show this progress dialogue only if the connection is
					// being requested
					if (VWPBluService.connectRequestThreadFlag) {
						progressDialog.setMessage(" Awaiting permission...");
						progressDialog.setCancelable(false);
						
						progressDialog.show();

					}

					break;
				case VWPBluService.STATE_CONNECTING:

					progressDialog.setMessage("Connecting to \""
							+ mConnectedDeviceName + "\"");
					progressDialog.setCancelable(false);
					
					if(!isFinishing())
						progressDialog.show();
					

					break;
				case VWPBluService.STATE_LISTEN:
					progressDialog.dismiss();

					Toast.makeText(getApplicationContext(), "Listner Started",
							Toast.LENGTH_SHORT).show();

				case VWPBluService.STATE_NONE:
					// mTitle.setText(R.string.title_not_connected);
					break;
				}
				break;
			case MESSAGE_WRITE:
				byte[] writeBuf = (byte[]) msg.obj;
				// construct a string from the buffer
				String writeMessage = new String(writeBuf);

				break;
			case MESSAGE_READ:
				byte[] readBuf = (byte[]) msg.obj;

				// construct a string from the valid bytes in the buffer
				String readMessage = new String(readBuf, 0, msg.arg1);

				if (readMessage.equals("CONN_OK")) {
					progressDialog.dismiss();
					startGame();
				} else {
					if (readMessage.equals("CONN_REJ")) {

						Toast.makeText(getApplicationContext(),
								"Sorry! Your connection was rejected!",
								Toast.LENGTH_LONG).show();
					} else
						messageReceived(readMessage);

				}

				break;
			case MESSAGE_DEVICE_NAME:
				// save the connected device's name
				mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
				// Toast.makeText(getApplicationContext(), "Connected to "
				// + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
				break;
			case MESSAGE_TOAST:
				Toast.makeText(getApplicationContext(),
						msg.getData().getString(TOAST), Toast.LENGTH_SHORT)
						.show();
				break;
			case CONN_LOST:
				progressDialog.dismiss();
				if (!isAccepting)
					messg = "Something went wrong, I lost the connection!";

				break;
			case CONN_FAIL:
				progressDialog.dismiss();
				messg = "Sorry, I cannot connect to \"" + mConnectedDeviceName
						+ "\"";

				break;
			case CONN_REJ:

				progressDialog.dismiss();
				messg = "Sorry, your connection was rejected!";
				break;

			case BLU_GAME_WON:

				progressDialog.dismiss();
				messg = "Congratulations! You  won the game";
				break;
			case BLU_GAME_OPP_WON:

				progressDialog.dismiss();
				messg = "Am sorry, your opponent won";
				break;
			case BLU_CONN_REQ:
				isAccepting = true;

				Log.d(TAG, "Asking permission");
				AlertDialog.Builder builder = new AlertDialog.Builder(
						VWPBluMain.this);
				builder.setMessage(
						"'" + mChatService.getConnectingDevice()
								+ "' wants to connect to you.  Allow ?")
						.setCancelable(false)
						.setPositiveButton("Yes",
								new DialogInterface.OnClickListener() {
									@Override
									public void onClick(DialogInterface dialog,
											int id) {
										mChatService.acceptConnection();
										progressDialog.dismiss();
										startGame();
									}
								})
						.setNegativeButton("No",
								new DialogInterface.OnClickListener() {
									@Override
									public void onClick(DialogInterface dialog,
											int id) {
										Toast.makeText(
												getApplicationContext(),
												"'"
														+ mChatService
																.getConnectingDevice()
														+ "'  request rejected!",
												Toast.LENGTH_LONG).show();
										mChatService.rejectConnection();
										progressDialog.dismiss();

									}
								});
				AlertDialog alert = builder.create();
				alert.show();

				break;

			}

			if (messg != null) {
				AlertDialog.Builder builder = new AlertDialog.Builder(
						VWPBluMain.this);
				builder.setMessage(messg)
						.setCancelable(false)
						.setPositiveButton("okay",
								new DialogInterface.OnClickListener() {
									@Override
									public void onClick(DialogInterface dialog,
											int id) {

										// TODO: Try finishing it with a result,  cancel or something. Instead of killing it
										
//										VWPBluMain.this.setResult(Activity.RESULT_OK);
//										VWPBluMain.this.finish();
//										// Tried it, tired of it! not working!
										
										
										
										//VWPBluMain.this.reload();
										// finish() would cause activity to finish,  but never does in time
										// so, kill it
										// NOT A GOOD IDEA TO KILL! screws the splash screen! 
									System.exit(0); 							
									}
								});

				AlertDialog alert = builder.create();
				
				
				// Damn this activity never finishes in time! So,  show iff  the activity has finished 
				// else this bugger would cause 
				// android.view.WindowManager$BadTokenException: Unable to add window -- token android.os.BinderProxy@47bafe28 is not valid; is your activity running?
				// Exception!
				if(!VWPBluMain.this.isFinishing())
					alert.show();
			}

		}
	};

	protected void messageReceived(String readMessage) {

		if (readMessage.equals("5")) {
			// mChatService.terminateConnection();
			// mHandler.sendMessage(mHandler.obtainMessage(BLU_GAME_OPP_WON));
			GamePlay.oppWon = true;
		}
		if (readMessage.equals("-1")) {
			Toast.makeText(ImageTargets.myContext,
					"Your opponent missed, you gained a point",
					Toast.LENGTH_SHORT).show();

			GamePlay.yourScore++;
			
			
			

		}

	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (D)
			Log.d(TAG, "onActivityResult " + resultCode);
		switch (requestCode) {
		case REQUEST_CONNECT_DEVICE:
			// When DeviceListActivity returns with a device to connect
			if (resultCode == VWPBluMain.CNTG) {
				// Get the device MAC address
				String address = data.getExtras().getString(
						VWPBluDevList.EXTRA_DEVICE_ADDRESS);
				// Get the BLuetoothDevice object
				BluetoothDevice device = mBluetoothAdapter
						.getRemoteDevice(address);
				// Attempt to connect to the device
				mChatService.connect(device);

			}
			if(resultCode == Activity.RESULT_CANCELED){ 
				
				finish(); // finish the activity since the user just went without selecting anything
				
			}
			break;
		case REQUEST_ENABLE_BT:
			// When the request to enable Bluetooth returns
			if (resultCode == Activity.RESULT_OK) {
				// Bluetooth is now enabled, so set up a chat session
				setupBluMenu();

			} else {
				if (resultCode == Activity.RESULT_CANCELED) {
					// User did not enable Bluetooth or an error occured

					Toast.makeText(this, "You must enable bluetooth to play!",
							Toast.LENGTH_SHORT).show();
					finish();
				}
			}
			break;
		case LOAD_GAME:
			if (resultCode == Activity.RESULT_OK) { // If the user go back to
													// menu manually to the menu

				mChatService.terminateConnection();

			} else {
				if (resultCode == GAME_WON) { // If the user go back to menu
												// manually to the menu

					mChatService.terminateConnection();
					mHandler.sendMessage(mHandler.obtainMessage(BLU_GAME_WON));

				}
				if (resultCode == GAME_LOST) { // If the user go back to menu
												// manually to the menu

					mChatService.terminateConnection();
					mHandler.sendMessage(mHandler.obtainMessage(BLU_GAME_LOST));

				}
				if (resultCode == BLU_GAME_OPP_WON) { // If the user go back to
														// menu manually to the
														// menu

					mChatService.terminateConnection();
					mHandler.sendMessage(mHandler
							.obtainMessage(BLU_GAME_OPP_WON));

				}
			}

			break;
		}
	}

	protected void startGame() {
		final Intent intent = new Intent(this, ImageTargets.class);
		startActivityForResult(intent, LOAD_GAME);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub

		this.openOptionsMenu();
		return super.onTouchEvent(event);
	}

	// Pop the menu as soon as view is displayed

	@Override
	public void onAttachedToWindow() {
		super.onAttachedToWindow();
		showBluMenu();
	}

	public void showBluMenu() {
		Intent serverIntent = new Intent(this, VWPBluDevList.class);
		startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {

		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);

		// this.openOptionsMenu();

		Toast toast;
		if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
			vwpSplash.setImageResource(R.drawable.vmp_back_land);
			toast = Toast.makeText(getApplicationContext(), "Mode: Landscape",
					Toast.LENGTH_SHORT);
			toast.setGravity(Gravity.BOTTOM, 0, 0);
			toast.show();

		} else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
			vwpSplash.setImageResource(R.drawable.vwp_back_port);
			toast = Toast.makeText(getApplicationContext(), "Mode: Potrait",
					Toast.LENGTH_SHORT);
			toast.setGravity(Gravity.BOTTOM, 0, 0);
			toast.show();
		} else if (newConfig.orientation == Configuration.ORIENTATION_UNDEFINED) {
			vwpSplash.setImageResource(R.drawable.vwp_back_port);
			toast = Toast.makeText(getApplicationContext(), "Mode: Undefined",
					Toast.LENGTH_SHORT);
			toast.setGravity(Gravity.BOTTOM, 0, 0);
			toast.show();
		}

	}

	
	
	 private void reload() {

		    Intent intent = getIntent();
		    overridePendingTransition(0, 0);
		    intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
		    
		    finish();
		 //   System.exit(0);

		    overridePendingTransition(0, 0);
		    startActivityForResult(intent,1);
		}

	
	
}