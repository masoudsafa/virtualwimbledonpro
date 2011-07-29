package com.vmp.main;

import java.util.ArrayList;

import com.qualcomm.QCARSamples.ImageTargets.DebugLog;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Vibrator;

public class VWPFeedbackManager {

	// SoundPool is used for short sound effects which are used frequently in
	// the game play
	private SoundPool mSoundPool;
	// An array of sound clip ids
	private ArrayList<Integer> mSoundIds;
	private Vibrator androidVibrator = null;

	/** Initialize the sound pool and load our sound clips. */
	public VWPFeedbackManager(Context context) {
		mSoundPool = new SoundPool(4, AudioManager.STREAM_MUSIC, 0);
		mSoundIds = new ArrayList<Integer>();
		mSoundIds.add(mSoundPool.load(context, R.raw.single_bounce, 1));
		mSoundIds.add(mSoundPool.load(context, R.raw.crowd_cheer, 2));
		mSoundIds.add(mSoundPool.load(context, R.raw.crowd_cheer_concert, 3));
		mSoundIds.add(mSoundPool.load(context, R.raw.ball_wall, 4));
		mSoundIds.add(mSoundPool.load(context, R.raw.crowd_woo, 5));

		// Init vibrator
		androidVibrator = (Vibrator) context
				.getSystemService(Context.VIBRATOR_SERVICE);

	}

	/** Play a sound with the given index and volume. */
	public void playSound(int soundIndex, float volume) {
		if (VWPPref.isSoundEnabled()) { // if sound is enabled
			if (soundIndex < 0 || soundIndex >= mSoundIds.size()) {
				DebugLog.LOGE("Sound id " + soundIndex + " out of range");
				return;
			}
			// No need to panic - here is what it means
			// public final int play (int soundID, float leftVolume, float
			// rightVolume, int priority, int loop, float rate)
			mSoundPool.play(mSoundIds.get(soundIndex), volume, volume, 1, 0,
					1.0f);
		}
	}

	public void vibratePhoneShort() {
		if (VWPPref.isVibrateEnabled()) // only vibrate iff it is enabled!
			androidVibrator.vibrate(500);
	}

	public void vibratePhoneLong() {
		if (VWPPref.isVibrateEnabled())
			androidVibrator.vibrate(1000);
	}

}
