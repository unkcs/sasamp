package com.liverussia.cr.gui.theft_auto;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.os.CountDownTimer;
import android.os.Vibrator;
import android.util.TypedValue;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.Toast;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.liverussia.cr.R;
import com.liverussia.cr.core.Samp;
import com.liverussia.cr.gui.adapters.TheftAutoLevelsAdapter;
import com.liverussia.launcher.ui.domain.ExpandableHeightGridView;

import java.util.ArrayList;
import java.util.List;

public class TheftAuto implements View.OnClickListener {

    private static final int LEFT_CENTRAL_POINT = 14000;
    private static final int RIGHT_CENTRAL_POINT = 16000;
    private static final int POINT_SIZE = 2000;
    private static final int SHIFT_FACTOR = 50;

    private ImageView mainButton;
    private ImageView theftAutoPoint;
    private ConstraintLayout theftAutoHud;
    private ExpandableHeightGridView levelsView;
    private ProgressBar theftAutoProgressBar;
    private Activity activity;

    private Animation animation;
    private Vibrator vibrator;

    private int failCount;
    private CountDownTimer countDownTimer;
    private long tick;
    private int success;
    private int shiftInDp;




    public TheftAuto(Activity activity) {
        this.activity = activity;

        activity.runOnUiThread(() -> {
            vibrator = (Vibrator) activity.getSystemService(Context.VIBRATOR_SERVICE);
            animation = AnimationUtils.loadAnimation(activity, R.anim.button_click);
            mainButton = activity.findViewById(R.id.theft_auto_main_btn);
            mainButton.setOnClickListener(this);
            theftAutoHud = activity.findViewById(R.id.theft_auto_hud);
            levelsView = activity.findViewById(R.id.theft_auto_levels);
            theftAutoProgressBar = activity.findViewById(R.id.theft_auto_progress);
            theftAutoPoint = activity.findViewById(R.id.theft_auto_point);

            init();
        });
    }

    private void init() {
        List<Integer> initLevelsColor = getInitLevelsColor();
        TheftAutoLevelsAdapter levelsAdapter = new TheftAutoLevelsAdapter(activity, initLevelsColor);
        levelsView.setAdapter(levelsAdapter);
    }

    private List<Integer> getInitLevelsColor() {
        List<Integer> initLevelsColor = new ArrayList<>();

        for (int i = 0; i < 10; i++) {
            initLevelsColor.add(R.color.theft_auto_init_level_color);
        }

        return initLevelsColor;
    }

    public void showRendering() {
        activity.runOnUiThread(()-> {
            theftAutoHud.setVisibility(View.VISIBLE);

            LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.WRAP_CONTENT,
                    LinearLayout.LayoutParams.WRAP_CONTENT
            );

            shiftInDp = 500;

            Resources r = activity.getResources();
            int shiftInPx = (int) TypedValue.applyDimension(
                    TypedValue.COMPLEX_UNIT_DIP,
                    shiftInDp,
                    r.getDisplayMetrics()
            );

            lp.setMargins(shiftInPx, 0, 0, 0);
            theftAutoPoint.setLayoutParams(lp);

            success = -1;
            theftAutoProgressBar.setProgress(30000);
            startCountdown();
        });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.theft_auto_main_btn:
                v.startAnimation(animation);
                doMainButtonAction();
                break;
            default:
                break;
        }
    }

    private void doMainButtonAction() {

        int leftPointWithShift = shiftInDp * SHIFT_FACTOR;
        int rightPointWithShift = shiftInDp * SHIFT_FACTOR + POINT_SIZE;

        if (theftAutoProgressBar.getProgress() > leftPointWithShift && theftAutoProgressBar.getProgress() < rightPointWithShift && success != 0)
        {
//            Samp.soundPool.play(sawSound, 0.1f, 0.1f, 1, 0, 1.2f);
            success = 1;
            theftAutoPoint.setImageResource(R.drawable.samwill_green);
            tick = rightPointWithShift;
            return;
        }else if (theftAutoProgressBar.getProgress() < rightPointWithShift) {
            if (vibrator.hasVibrator()) {
                vibrator.vibrate(200);
            }
            tick = rightPointWithShift;
            success = 0;
            theftAutoPoint.setImageResource(R.drawable.samwill_red);
            return;
        }
    }

    public void startCountdown() {

        int rightPointWithShift = shiftInDp * SHIFT_FACTOR + POINT_SIZE;

        if (countDownTimer != null) {
            countDownTimer.cancel();
            countDownTimer = null;
            tick = 0;
        }
        countDownTimer = new CountDownTimer(999999999, 17) {
            @Override
            public void onTick(long j) {
                tick+=40;
                theftAutoProgressBar.setProgress((int)tick);
//                int progresstext = samwill_progress.getProgress() / 25 / 10;
//                samwill_procent.setText(progresstext + "%");
                if (theftAutoProgressBar.getProgress() > rightPointWithShift && success != 1)
                {
                    success = 0;
                    theftAutoPoint.setImageResource(R.drawable.samwill_red);
                }

            }
            @Override
            public void onFinish() {
                // Hide();
            }
        }.start();
    }
}
