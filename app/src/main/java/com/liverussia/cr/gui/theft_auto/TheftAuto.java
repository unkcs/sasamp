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

    private static final int MIN_POINT_MARGIN = 100;
    private static final int MAX_POINT_MARGIN = 500;
    private static final int POINT_SIZE = 2000;
    private static final int SHIFT_FACTOR = 50;
    private static final int PROGRESS_BAR_MAX_VALUE = 30000;
    private static final int PROGRESS_SPEED = 300;
    private static final int GAME_PASSED_STATUS = 1;
    private static final int GAME_NOT_PASSED_STATUS = 0;
    private static final int MAX_FAIL_COUNT = 3;
    private static final int MAX_LEVEL = 10;


    private ImageView mainButton;
    private ImageView theftAutoPoint;
    private ConstraintLayout theftAutoHud;
    private ExpandableHeightGridView levelsView;
    private ProgressBar theftAutoProgressBar;
    private Activity activity;

    private TheftAutoLevelsAdapter levelsAdapter;

    private Animation animation;
    private Vibrator vibrator;

    private int failCount;
    private int currentLevel;
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

    native void finishRendering(int status);

    private void init() {
        List<Integer> initLevelsColor = getInitLevelsColor();
        levelsAdapter = new TheftAutoLevelsAdapter(activity, initLevelsColor);
        levelsView.setAdapter(levelsAdapter);

        failCount = 0;
        currentLevel = 1;
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
            theftAutoPoint.setImageResource(R.drawable.samwill_gray);

            LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.WRAP_CONTENT,
                    LinearLayout.LayoutParams.WRAP_CONTENT
            );

            shiftInDp = getRandomMargin(MIN_POINT_MARGIN, MAX_POINT_MARGIN);

            Resources r = activity.getResources();
            int shiftInPx = (int) TypedValue.applyDimension(
                    TypedValue.COMPLEX_UNIT_DIP,
                    shiftInDp,
                    r.getDisplayMetrics()
            );

            lp.setMargins(shiftInPx, 0, 0, 0);
            theftAutoPoint.setLayoutParams(lp);

            success = -1;
            theftAutoProgressBar.setProgress(PROGRESS_BAR_MAX_VALUE);
            startCountdown();
        });
    }

    public int getRandomMargin(int min, int max) {
        return (int) ((Math.random() * (max - min)) + min);
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
            levelsAdapter.updateItem(currentLevel - 1, R.color.theft_auto_passed_level_color);
            tick = rightPointWithShift;
            return;
        }else if (theftAutoProgressBar.getProgress() < rightPointWithShift) {
            if (vibrator.hasVibrator()) {
                vibrator.vibrate(200);
            }
            tick = rightPointWithShift;
            success = 0;
            theftAutoPoint.setImageResource(R.drawable.samwill_red);
            levelsAdapter.updateItem(currentLevel - 1, R.color.theft_auto_not_passed_level_color);
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
                tick += PROGRESS_SPEED;
                theftAutoProgressBar.setProgress((int)tick);
                if (theftAutoProgressBar.getProgress() > rightPointWithShift && success != 1) {
                    success = 0;
                    theftAutoPoint.setImageResource(R.drawable.samwill_red);
                    levelsAdapter.updateItem(currentLevel - 1, R.color.theft_auto_not_passed_level_color);
                }
                if(theftAutoProgressBar.getProgress() >= theftAutoProgressBar.getMax()){
                    countDownTimer.cancel();
                    countDownTimer.onFinish();
                }

            }
            @Override
            public void onFinish() {
                goToNextLevelOrHide();
            }
        }.start();
    }

    private void goToNextLevelOrHide() {
        if (R.color.theft_auto_not_passed_level_color == levelsAdapter.getItem(currentLevel - 1)) {
            failCount++;
        }

        if (failCount == MAX_FAIL_COUNT) {
            finishRendering(GAME_NOT_PASSED_STATUS);
            theftAutoHud.setVisibility(View.GONE);
            return;
        }

        if (currentLevel == MAX_LEVEL) {
            finishRendering(GAME_PASSED_STATUS);
            theftAutoHud.setVisibility(View.GONE);
            return;
        }

        currentLevel++;
        startNewLevel();
    }

    private void startNewLevel() {
        showRendering();
    }
}
