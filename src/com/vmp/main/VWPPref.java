package com.vmp.main;

import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.MenuItem;
import android.widget.Toast;

public class VWPPref extends PreferenceActivity {


	private static SharedPreferences prefs = null;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		addPreferencesFromResource(R.xml.settings);
		this.setTitle("Game Settings");
		prefs = PreferenceManager.getDefaultSharedPreferences(this);
		prefs.registerOnSharedPreferenceChangeListener(new PrefernceListener());
		
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		// Whenever menu item is selected
		// Read the all the preference items and update it in the default
	
		return super.onMenuItemSelected(featureId, item);
		

	}

	/**
	 * @return the isSoundEnabled
	 */
	public static boolean isSoundEnabled() {
	
		
		return prefs.getBoolean("soundPref",false);
		
		
		
	}

	/**
	 * @return the isVibrateEnabled
	 */
	public static boolean isVibrateEnabled() {
		return  prefs.getBoolean("vibratePref",false);
	}

	

	/**
	 * @return the courtTex
	 */
	public static  String getCourtTex() {
		return prefs.getString("courtTexPref", "100");
	}

	

	/**
	 * @return the ballSpeed
	 */
	public static String getBallSpeed() {
		return prefs.getString("ballSpeedPref", "400");
	}

	
	public void printPrefs(){
		
		
		Log.d("VMPMain", "Preferences:");
		Log.d("VMPMain", "isSoundEnabled:"+VWPPref.isSoundEnabled());
		Log.d("VMPMain", "isVibrateEnabled:"+VWPPref.isVibrateEnabled());
		Log.d("VMPMain", "ballSpeed:"+getBallSpeed());
		Log.d("VMPMain", "court texture:"+getCourtTex());
		
		
		

	}
	
	
class PrefernceListener implements OnSharedPreferenceChangeListener{
	@Override
	public void onSharedPreferenceChanged(SharedPreferences sharedPreferences,
			String key) {
		printPrefs();
		
		String mesg="";
		
		if(key.equals("soundPref")||key.equals("vibratePref")){
			CheckBoxPreference l =  (CheckBoxPreference) getPreferenceScreen().findPreference(key);
			
			mesg="'"+l.getTitle()+"' is now: "+(l.isChecked()?"enabled":"disabled");
		}
		else{
			ListPreference l =  (ListPreference) getPreferenceScreen().findPreference(key);
			mesg="'"+l.getTitle()+"' has been changed to: "+l.getEntry();
		}
		
	
		Toast.makeText(VWPPref.this,
				mesg, Toast.LENGTH_LONG).show();
		
		
	}
}
}
