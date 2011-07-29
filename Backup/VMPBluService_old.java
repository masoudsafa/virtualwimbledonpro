/**
 * 
 */
package com.vmp.blu;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

/**
 * @author tckb
 * 
 */
public class VMPBluService_old extends Activity {

	// States of the Bluetooth service
	public static final int BLU_FREE = -990876;
	public static final int BLU_CONN = -990866;
	public static final int BLU_CNTD = -9908986;
	public static final int BLU_BUSY = -990976;
	public static final int BLU_CONN_FAILED = -990996;
	public static final int BLU_NULL = -1;
	public static final int BLU_MESSAGE_WRITE=-998121;
	public static final int BLU_MESSAGE_READ=-908621;

	public static final UUID BLU_UUID = UUID.randomUUID();
	protected static final String BLU_NAME = "VWP Blue service";
	protected static BluetoothAdapter mAdapter = null;

	// state
	private int bluState = -1;

	protected Handler messageHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {

			switch (msg.what) {
			case VMPBluService_old.BLU_FREE:
				Toast.makeText(getApplicationContext(),
						"Blue service initiated", Toast.LENGTH_SHORT);
				break;
			case VMPBluService_old.BLU_CONN:
				Toast.makeText(getApplicationContext(),
						"Blue service connecting...", Toast.LENGTH_SHORT);
				break;
			case VMPBluService_old.BLU_CNTD:
				Toast.makeText(getApplicationContext(),
						"Blue service connected!", Toast.LENGTH_SHORT);
				break;
			case VMPBluService_old.BLU_BUSY:
				Toast.makeText(getApplicationContext(),
						"Blue service busy!", Toast.LENGTH_SHORT);
				break;
			case VMPBluService_old.BLU_CONN_FAILED:
				Toast.makeText(getApplicationContext(),
						"Blue service connection failed!", Toast.LENGTH_LONG);
				break;
			 case BLU_MESSAGE_WRITE:
	                byte[] writeBuf = (byte[]) msg.obj;
	                // construct a string from the buffer
	           //TODO: handle message write
	               
	                break;
	            case BLU_MESSAGE_READ:
	                byte[] readBuf = (byte[]) msg.obj;
	                // construct a string from the valid bytes in the buffer
	                if(readBuf.length==1){
	                	if(readBuf[0]==BLU_BUSY){
	                		Toast.makeText(VMPBluService_old.this.getApplicationContext(),
	        						"Sorry your buddy is already engaged!", Toast.LENGTH_LONG);
	                	}
	                }
	                
	                
	                String readMessage = new String(readBuf, 0, msg.arg1);
	            	Toast.makeText(VMPBluService_old.this.getApplicationContext(),
							readMessage, Toast.LENGTH_LONG);
	                
	                break;
				
				
				
			}

		}
	};

	private Thread acceptThread = new Thread("BluAcceptThread") {
		// The local server socket
		private BluetoothServerSocket mmServerSocket;
		BluetoothSocket socket = null;
		
		InputStream mmInStream;
		OutputStream mmOutStream;

		public void Thread() {
			BluetoothServerSocket tmp = null;

			// Create a new listening server socket
			try {
				tmp = mAdapter.listenUsingRfcommWithServiceRecord(BLU_NAME,
						BLU_UUID);
			} catch (IOException e) {
				;
			}
			mmServerSocket = tmp;
		}

		public void run() {
			InputStream tmpIn = null;
			OutputStream tmpOut = null;

			// Listen to all connections first
			while (getBluState() != BLU_NULL) {

				// Only Listen when the blu service is free i.e.,

				try {
					// This is a blocking call and will only return on a
					// successful connection or an exception
					socket = mmServerSocket.accept();
					//lameDevice = socket.getRemoteDevice();
					updateBluState(BLU_CNTD);
					// Get the BluetoothSocket input and output streams
					try {
						tmpIn = socket.getInputStream();
						tmpOut = socket.getOutputStream();
					} catch (IOException e) {
						;
					}
					mmInStream = tmpIn;
					mmOutStream = tmpOut;
				} catch (IOException e) {

					break;
				}

				// Now handle the socket
				new Runnable() {
					public void run() {

						// Sorry dude! I'm busy; Send in BUSY message
						if (getBluState() == BLU_BUSY) {
							try {
								write(mmOutStream, new Integer(BLU_BUSY).byteValue());
								//close connection
								cancel();
							} catch (IOException e) {
								// TODO Failed to write handle it
								
								e.printStackTrace();
							}

						}
						// Okay now am free, come connect me
						else {

							if (getBluState() == BLU_FREE) {
								updateBluState(BLU_BUSY);
								
								while (true) {
									try {
										byte[] data = read1024(mmInStream);
										
										messageHandler.obtainMessage(BLU_MESSAGE_READ,	data.length, -1, data);										
									
										
									} catch (IOException e) {
										// TODO Auto-generated catch block

										// we've lost connection here
										// set back the state to free
										updateBluState(BLU_FREE);
										cancel();
										e.printStackTrace();
									}

								}
							}
						}

					}

				}; // end of runnable

			}
		}

		public void cancel() {
			try {
				socket.close();
			} catch (IOException e) {
				;
			}
		}

	};

	protected void updateBluState(int state) {
		this.bluState = state;
		messageHandler.sendEmptyMessage(BLU_BUSY);
	}

	public int getBluState() {
		return bluState;
	}

	public synchronized void write(OutputStream mmOutStream, byte[] buffer)
			throws IOException {

		mmOutStream.write(buffer);

	}
	public synchronized void write(OutputStream mmOutStream, byte byteData)
	throws IOException {

			mmOutStream.write(byteData);

}
	
	
	
	// Now Keep listening to the InputStream while connected
	public synchronized byte[] read1024(InputStream mmInStream)
			throws IOException {

		byte[] b = new byte[1024];
		mmInStream.read(b);
		return b;

	}

	private Thread sendThread = new Thread("BluSendThread") {

		public void Thread() {
			// TODO:

		}

		@Override
		public void run() {

		}
	};

	public boolean startService() {
		mAdapter = BluetoothAdapter.getDefaultAdapter();
		acceptThread.start();
		sendThread.start();
		
		
		
		
		return true;

	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);

		// notify that the service has started successfully
		updateBluState(BLU_FREE);
		if (this.startService())
			messageHandler.sendEmptyMessage(VMPBluService_old.BLU_FREE);
		else
			messageHandler.sendEmptyMessage(VMPBluService_old.BLU_CONN_FAILED);
	}

	public synchronized void connect(BluetoothDevice device) {
		// TODO: Connecting thread

	}

	public Handler getHandler() {
		return messageHandler;
	}

	private class ConnThread extends Thread {

		private BluetoothSocket mmSocket = null;
		private BluetoothDevice mmDevice = null;

		public void ConnectThread(BluetoothDevice device) {
			mmDevice = device;
			BluetoothSocket tmp = null;

			// Get a BluetoothSocket for a connection with the
			// given BluetoothDevice
			try {
				tmp = device
						.createRfcommSocketToServiceRecord(VMPBluService_old.BLU_UUID);
				this.setName("BluConnThread: " + device.getName());
			} catch (IOException e) {
				;
			}
			mmSocket = tmp;
		}

		@Override
		public void run() {

			// Make a connection to the BluetoothSocket
			try {
				// This is a blocking call and will only return on a
				// successful connection or an exception
				mmSocket.connect();
			} catch (IOException e) {
				connectionFailed();
				// Close the socket
				try {
					mmSocket.close();
				} catch (IOException e2) {
					;
				}
				// Start the service over to restart listening mode
				// VWPBluService.this.start();
				return;
			}

			// // Reset the ConnectThread because we're done
			// synchronized (VWPBluService.this) {
			// mConnectThread = null;
			// }

			// Start the connected thread
			connected(mmSocket, mmDevice);
		}

		private void connected(BluetoothSocket mmSocket2,
				BluetoothDevice mmDevice2) {
			// TODO Auto-generated method stub

		}

		public void cancel() {
			try {
				mmSocket.close();
			} catch (IOException e) {
				;
			}
		}

		private void connectionFailed() {
			messageHandler.sendEmptyMessage(BLU_CONN_FAILED);
		}

	}

}
