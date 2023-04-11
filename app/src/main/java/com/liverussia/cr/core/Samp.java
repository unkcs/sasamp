package com.liverussia.cr.core;

import android.app.Activity;
import android.content.Context;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Bundle;
import android.util.Log;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.liverussia.cr.R;
import com.liverussia.cr.gui.AucContainer;
import com.liverussia.cr.gui.Casino;
import com.liverussia.cr.gui.CasinoBaccarat;
import com.liverussia.cr.gui.CasinoDice;
import com.liverussia.cr.gui.DailyReward;
import com.liverussia.cr.gui.hud.HudManager;
import com.liverussia.cr.gui.tab.Tab;
import com.liverussia.cr.gui.util.Utils;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

public class Samp extends GTASA
{
    public static Activity activity;

    public static native void playUrlSound(String url);
    public static DecimalFormat formatter = null;
    public static SoundPool soundPool = null;

    native void initSAMP(String game_path);

    @Override
    public void onCreate(Bundle bundle) {
        activity = this;

        Log.i("java", "calling initSAMP");
        initSAMP(getExternalFilesDir(null).toString() + "/");

        super.onCreate(bundle);

        init();
    }

    void init()
    {
        DecimalFormatSymbols otherSymbols = new DecimalFormatSymbols(Locale.getDefault());
        otherSymbols.setGroupingSeparator('.');
        formatter = new DecimalFormat("###,###.###", otherSymbols);

        AudioAttributes attributes = new AudioAttributes.Builder()
                .setUsage(AudioAttributes.USAGE_GAME)
                .setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION)
                .build();
        soundPool = new SoundPool.Builder().setAudioAttributes(attributes).build();

        new HudManager(this);
        new CasinoDice(this);
        new CasinoBaccarat(this);
        new AucContainer(this);
        new DailyReward(this);
        new Tab(this);
        new Casino(this);
    }

    public void playLocalSound(int soundID, float speed){
        soundPool.load(this, soundID, 0);

        soundPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
            @Override
            public void onLoadComplete(SoundPool soundPool, int i, int i1) {
                AudioManager audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);

                float curVolume = audioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
                float maxVolume = audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
                float leftVolume = curVolume / maxVolume;
                float rightVolume = curVolume / maxVolume;

                soundPool.play(i, leftVolume, rightVolume, 1, 0, speed);

                soundPool.unload(i);
            }
        });

    }

    public void hideLoadingScreen() {
        TimerTask task = new TimerTask() {
            public void run() {
                activity.runOnUiThread(() -> {
                    ConstraintLayout loadscreen_main_layout = activity.findViewById(R.id.loadscreen_main_layout);
                    Utils.HideLayout(loadscreen_main_layout, true);
                });

            }
        };
        Timer timer = new Timer("Timer");

        timer.schedule(task, 900L);
    }
}
