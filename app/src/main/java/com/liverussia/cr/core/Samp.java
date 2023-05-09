package com.liverussia.cr.core;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;

import androidx.annotation.ColorInt;
import androidx.annotation.NonNull;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.liverussia.cr.R;
import com.liverussia.cr.gui.AucContainer;
import com.liverussia.cr.gui.Casino;
import com.liverussia.cr.gui.CasinoBaccarat;
import com.liverussia.cr.gui.casino.Dice;
import com.liverussia.cr.gui.DailyReward;
import com.liverussia.cr.gui.hud.Binder;
import com.liverussia.cr.gui.hud.HudManager;
import com.liverussia.cr.gui.tab.Tab;
import com.liverussia.cr.gui.util.Utils;

import java.security.MessageDigest;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

import eltos.simpledialogfragment.SimpleDialog;
import eltos.simpledialogfragment.color.SimpleColorDialog;

public class Samp extends GTASA implements SimpleDialog.OnDialogResultListener
{
    public Binder binder;
    public static float maxFps;
    public static WindowManager windowManager = null;
    public static Activity activity;
    public static final int INVALID_PLAYER_ID = 65535;
    public static native void playUrlSound(String url);
    public static DecimalFormat formatter = null;
    public static SoundPool soundPool = null;

    native void initSAMP(String game_path, float maxFps);

    @Override
    public void onCreate(Bundle bundle) {
        activity = this;

        Log.i("java", "calling initSAMP");
        windowManager = (WindowManager) this.getSystemService(WINDOW_SERVICE);

        Display display = Samp.windowManager.getDefaultDisplay();
        maxFps = display.getRefreshRate();

        initSAMP(getExternalFilesDir(null).toString() + "/", maxFps);

        super.onCreate(bundle);

        init();
    }

    float getMaxFps() {
        Display display = Samp.windowManager.getDefaultDisplay();
        return display.getRefreshRate();
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
        new AucContainer(this);
        new DailyReward(this);
        new Tab(this);
        new Casino(this);
        binder = new Binder();
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

    //FIXME: абстракция :C
    @Override
    public boolean onResult(@NonNull String dialogTag, int which, @NonNull Bundle extras) {
        if ("BINDERCOLOR".equals(dialogTag)){
            @ColorInt int color = extras.getInt(SimpleColorDialog.COLOR);
            binder.adapter.changeColor(color);
            return true;
        }
        return false;
    }

    public static String getSignature() {
        String apkSignature = null;
        try {
            PackageInfo packageInfo = activity.getPackageManager().getPackageInfo(
                    activity.getPackageName(),
                    PackageManager.GET_SIGNATURES
            );
            for (Signature signature : packageInfo.signatures) {
                MessageDigest md = MessageDigest.getInstance("SHA");
                md.update(signature.toByteArray());
                apkSignature = Base64.encodeToString(md.digest(), Base64.DEFAULT);
            }
        } catch (Exception ignored) {}
        return apkSignature;
    }

}
