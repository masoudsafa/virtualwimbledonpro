/**
 * 
 */
package com.vmp.main;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SubMenu;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import com.vmp.blu.VWPBluMain;


/**
 * @author tckb
 *
 */
public class VMPMenu extends Activity {
	private static final int MENU = -8890;
	private static final int GAME_START = 12198;
	private static final int GAME_HELP = -98656;
	private static final int GAME_SET = 88789;
	private static final int GAME_EXIT = 786675;
	private ImageView vwpSplash;
	private ImageView backSplash;

	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		vwpSplash = (ImageView) findViewById(R.id.imageView2);
        backSplash = (ImageView) findViewById(R.id.imageView1);
        
        vwpSplash.setVisibility(View.GONE);
        backSplash.setVisibility(View.VISIBLE);
        
        Toast toast = Toast.makeText(getApplicationContext(), "Choose Options from the options menu", Toast.LENGTH_SHORT);
		toast.setGravity(Gravity.BOTTOM, 0, 0);
		toast.show();
   
		
	}
	
	// Pop the menu as soon as view is displayed
	
	@Override
	public void onAttachedToWindow() {
	    super.onAttachedToWindow();
	    openOptionsMenu();
	}

	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
			MenuInflater inflater = getMenuInflater();
		    inflater.inflate(R.menu.mainmenu, menu);
		
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public View onCreateView(String name, Context context, AttributeSet attrs) {
		// TODO Auto-generated method stub
		return super.onCreateView(name, context, attrs);
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
	    	if(newConfig.orientation==newConfig.ORIENTATION_LANDSCAPE){
				backSplash.setImageResource(R.drawable.vmp_back_land);
				 toast = Toast.makeText(getApplicationContext(), "Mode: Landscape", Toast.LENGTH_SHORT);
				toast.setGravity(Gravity.BOTTOM, 0, 0);
				toast.show();
				
			}
			else
				if(newConfig.orientation==newConfig.ORIENTATION_PORTRAIT){
					backSplash.setImageResource(R.drawable.vwp_back_port);
				    toast = Toast.makeText(getApplicationContext(), "Mode: Potrait", Toast.LENGTH_SHORT);
					toast.setGravity(Gravity.BOTTOM, 0, 0);
					toast.show();
				}
				else
					if(newConfig.orientation==newConfig.ORIENTATION_UNDEFINED){
						backSplash.setImageResource(R.drawable.vwp_back_port);
					    toast = Toast.makeText(getApplicationContext(), "Mode: Undefined", Toast.LENGTH_SHORT);
						toast.setGravity(Gravity.BOTTOM, 0, 0);
						toast.show();
					}
	    
	    	
	    	
	    }
	    
	
	  @Override
		public boolean onOptionsItemSelected(MenuItem item) {
		  AlertDialog.Builder builder = new AlertDialog.Builder(this);
		  final Toast toast;
			switch (item.getItemId()) {
			
			case R.id.GAME_START:
				
				Intent vmpBlue = new Intent(this,VMPBluMain.class);
				this.startActivity(vmpBlue);
			//	return true;
				break;

			case R.id.GAME_HELP:
			
				toast = Toast.makeText(getApplicationContext(), "Help Me!", Toast.LENGTH_SHORT);
	    		toast.setGravity(Gravity.CENTER, 0, 0);
	    		toast.show();
	    		
	    		break;

		
			
			case R.id.GAME_SET:
				
				toast = Toast.makeText(getApplicationContext(), "Change the Settings!", Toast.LENGTH_SHORT);
	    		toast.setGravity(Gravity.CENTER, 0, 0);
	    		toast.show();
				
				break;
			
			case R.id.GAME_ABOUT:
				
				toast = Toast.makeText(getApplicationContext(), R.string.app_description, Toast.LENGTH_SHORT);
	    		toast.setGravity(Gravity.CENTER, 0, 0);
	    		toast.show();
				
	    		break;			
			
			
			
			}
			return super.onOptionsItemSelected(item);
	    
	
	  }

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	

}
