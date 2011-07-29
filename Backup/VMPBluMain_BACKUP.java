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


import java.io.IOException;

import com.qualcomm.QCARSamples.ImageTargets.ImageTargets;
import com.vmp.blu.VMPBluMain_BACKUP;
import com.vmp.main.VMPMenu;


import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

/**
 * This is the main Activity that displays the current chat session.
 * @param <MyActivity>
 */
public class VMPBluMain_BACKUP extends Activity {
    // Debugging
    private static final String TAG = "VMPBluMain";
    private static final boolean D = true;
	private boolean perm;

    // Message types sent from the VMPBluService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;

    // Key names received from the VMPBluService Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    // Intent request codes
    protected static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;
	protected static final int CONN_LOST = 88976;
	protected static final int CONN_FAIL = 99698;
	public static final int CNTG = -987292;
	protected static final int BLU_CONN_REQ=88769;
	
	
	
	
    // Layout Views
    private TextView mTitle;
    private ListView mConversationView;
    private EditText mOutEditText;
    private Button mSendButton;

    // Name of the connected device
    private String mConnectedDeviceName = null;
    // Array adapter for the conversation thread
    private ArrayAdapter<String> mConversationArrayAdapter;
    // String buffer for outgoing messages
    private StringBuffer mOutStringBuffer;
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
    // Member object for the chat services
    private VMPBluService mChatService = null;


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(D) Log.e(TAG, "+++ ON CREATE +++");

      
        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // If the adapter is null, then Bluetooth is not supported
//        if (mBluetoothAdapter == null) {
//            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
//            finish();
//            return;
//        }
    }

    @Override
    public void onStart() {
        super.onStart();
        if(D) Log.e(TAG, "++ ON START ++");
//
//        // If BT is not on, request that it be enabled.
//        // setupChat() will then be called during onActivityResult
//        if (!mBluetoothAdapter.isEnabled()) {
//            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
//            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
//        // Otherwise, setup the chat session
//        } else {
            if (mChatService == null) setupBluMenu();
        //}
    }

    @Override
    public synchronized void onResume() {
        super.onResume();
        if(D) Log.e(TAG, "+ ON RESUME +");

        // Performing this check in onResume() covers the case in which BT was
        // not enabled during onStart(), so we were paused to enable it...
        // onResume() will be called when ACTION_REQUEST_ENABLE activity returns.
        if (mChatService != null) {
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (mChatService.getState() == VMPBluService.STATE_NONE) {
              // Start the Bluetooth chat services
              mChatService.start();
            }
        }
    }

    private void setupBluMenu() {
    	
        // Initialize the VMPBluService to perform bluetooth connections
        mChatService = new VMPBluService(this, mHandler);

        // Initialize the buffer for outgoing messages
        mOutStringBuffer = new StringBuffer("");
        
        Intent serverIntent = new Intent(this, VMPBluDevList.class);
        startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
        
        
        
    }

    @Override
    public synchronized void onPause() {
        super.onPause();
        if(D) Log.e(TAG, "- ON PAUSE -");
    }

    @Override
    public void onStop() {
        super.onStop();
        if(D) Log.e(TAG, "-- ON STOP --");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        // Stop the Bluetooth chat services
        if (mChatService != null) mChatService.stop();
        if(D) Log.e(TAG, "--- ON DESTROY ---");
    }

    private void ensureDiscoverable() {
        if(D) Log.d(TAG, "ensure discoverable");
        if (mBluetoothAdapter.getScanMode() !=
            BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            startActivity(discoverableIntent);
        }
    }

    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendMessage(String message) {
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != VMPBluService.STATE_CONNECTED) {
            //Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the VMPBluService to write
            byte[] send = message.getBytes();
            mChatService.write(send);

            // Reset out string buffer to zero and clear the edit text field
            mOutStringBuffer.setLength(0);
            mOutEditText.setText(mOutStringBuffer);
        }
    }


    // The Handler that gets information back from the VMPBluService
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
        	String messg=null;
        	ProgressDialog progressDialog = new ProgressDialog(VMPBluMain_BACKUP.this);
            progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            
            switch (msg.what) {
            case MESSAGE_STATE_CHANGE:
                if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
                switch (msg.arg1) {
                case VMPBluService.STATE_CONNECTED:
               
                	progressDialog.dismiss();
                	 startGame();
                    break;
                case VMPBluService.STATE_CONNECTING:
                
                   
                	progressDialog.setMessage("Connecting to \""+mConnectedDeviceName+"\"");
                    progressDialog.setCancelable(false);
                    progressDialog.show();
          
                    break;
                case VMPBluService.STATE_LISTEN:
                	Toast.makeText(getApplicationContext(), "Listner Started", Toast.LENGTH_SHORT).show();
            	
                case VMPBluService.STATE_NONE:
                  //  mTitle.setText(R.string.title_not_connected);
                    break;
                }
                break;
            case MESSAGE_WRITE:
                byte[] writeBuf = (byte[]) msg.obj;
                // construct a string from the buffer
                String writeMessage = new String(writeBuf);
                mConversationArrayAdapter.add("Me:  " + writeMessage);
                break;
            case MESSAGE_READ:
                byte[] readBuf = (byte[]) msg.obj;
                // construct a string from the valid bytes in the buffer
                String readMessage = new String(readBuf, 0, msg.arg1);
                mConversationArrayAdapter.add(mConnectedDeviceName+":  " + readMessage);
                break;
            case MESSAGE_DEVICE_NAME:
                // save the connected device's name
                mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
//                Toast.makeText(getApplicationContext(), "Connected to "
//                               + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                break;
            case MESSAGE_TOAST:
                Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                               Toast.LENGTH_SHORT).show();
                break;
            case CONN_LOST:
            	progressDialog.dismiss();
            	messg = "Oops something went wrong! Connection Lost";
            	
            	break;
            case CONN_FAIL:
            	progressDialog.dismiss();
            	messg = "I'm Sorry I cannnot connect to \""+mConnectedDeviceName+"\"";
            	
            	break;
            case BLU_CONN_REQ:
            	if(askPermissionToConnect())
            		mChatService.acceptConnection();
            	else
            		mChatService.rejectConnection();
         	
            	
            }
            
            if(messg!=null){
            	AlertDialog.Builder builder = new AlertDialog.Builder(VMPBluMain_BACKUP.this);
            	builder.setMessage(messg)
            	       .setCancelable(false)
            	       .setPositiveButton("Try again", new DialogInterface.OnClickListener() {
            	           public void onClick(DialogInterface dialog, int id) {
            	              //  VMPBluMain.this.openOptionsMenu();
            	               // openOptionsMenu();
            	                finish();
            	           }
            	       });
            	       
            	AlertDialog alert = builder.create();
            	alert.show();
            }
            
        }
    };

    

 private boolean askPermissionToConnect() {
       	
    	Log.d(TAG,"Asking permission");
     	AlertDialog.Builder builder = new AlertDialog.Builder(this);
    	builder.setMessage(mChatService.getConnectingDevice() +" wants to connect. Allow ?")
    	       .setCancelable(false)
    	       .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
    	           public void onClick(DialogInterface dialog, int id) {
    	        	 perm=true;
    	           }
    	       })
    	       .setNegativeButton("No", new DialogInterface.OnClickListener() {
    	           public void onClick(DialogInterface dialog, int id) {
    	        	   perm=false;
    	           }
    	       });
    	AlertDialog alert = builder.create();
    	alert.show();
    	
    	return perm;
		
	}
    
    
    
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(D) Log.d(TAG, "onActivityResult " + resultCode);
        switch (requestCode) {
        case REQUEST_CONNECT_DEVICE:
          //   When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                // Get the device MAC address
                String address = data.getExtras()
                                     .getString(VMPBluDevList.EXTRA_DEVICE_ADDRESS);
                // Get the BLuetoothDevice object
                BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
                // Attempt to connect to the device
               mChatService.connect(device);
            }
            break;
        case REQUEST_ENABLE_BT:
            // When the request to enable Bluetooth returns
            if (resultCode == Activity.RESULT_OK) {
                // Bluetooth is now enabled, so set up a chat session
                setupBluMenu();
            } else {
                // User did not enable Bluetooth or an error occured
              
                Toast.makeText(this, "You must enable bluetooth to play!", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
 
//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        MenuInflater inflater = getMenuInflater();
//        inflater.inflate(R.menu.option_menu, menu);
//        return true;
//    }

    protected void startGame() {
    	final Intent intent = new Intent(this, ImageTargets.class);
    	startActivity(intent);
	}

	@Override
    public boolean onOptionsItemSelected(MenuItem item) {
     //   switch (item.getItemId()) {
//        case R.id.scan:
//            // Launch the DeviceListActivity to see devices and do scan
//            Intent serverIntent = new Intent(this, DeviceListActivity.class);
//            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
//            return true;
//        case R.id.discoverable:
//            // Ensure this device is discoverable by others
//            ensureDiscoverable();
//            return true;
//        }
       return false;
    }

}