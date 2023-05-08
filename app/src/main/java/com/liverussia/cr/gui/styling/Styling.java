package com.liverussia.cr.gui.styling;

import static com.liverussia.cr.core.Samp.activity;

import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.google.android.material.button.MaterialButton;
import com.liverussia.cr.R;
import com.liverussia.cr.core.Samp;
import com.skydoves.colorpickerview.ColorEnvelope;

public class Styling extends ColorPicker {

    static final int COLOR_TYPE_NEON = 0;
    static final int COLOR_TYPE_LIGHT = 1;
    static final int COLOR_TYPE_TONER = 2;
    static final int COLOR_TYPE_BODY1 = 3;
    static final int COLOR_TYPE_BODY2 = 4;
    static final int COLOR_TYPE_WHEEL = 5;

    int colorType;

    ConstraintLayout styilyng_gui_layout;
    ConstraintLayout mainLayout;

    private Animation animation;

    private TextView moneyText;
    private TextView howMuchText;

    private ImageView styling_buy_butt;

    native void exitClick();
    native void sendOnChooseVinil();
    native void sendChoosedColor(int type, int r, int g, int b, int a);

    native void sendBuy();

    native void onChangeColor(int type, int r, int g, int b, int a);
    native boolean isAvailableStyle(int type);

    native void changeVinyls(boolean isNext);

    native int getActiveColor(int type);

    public Styling(int money, int total, int prices[]) {
        activity.runOnUiThread(() -> {
            styilyng_gui_layout = activity.findViewById(R.id.styilyng_gui_layout);

            animation = AnimationUtils.loadAnimation(activity, R.anim.button_click);

            moneyText = activity.findViewById(R.id.styling_money);

            ConstraintLayout styling_exit_butt = activity.findViewById(R.id.styling_exit_butt);
            styling_exit_butt.setOnClickListener(view -> {
                activity.runOnUiThread(() -> {
                    mainLayout.setVisibility(View.GONE);
                });
                exitClick();
                // toStock();
            });

            styling_buy_butt = activity.findViewById(R.id.styling_buy_butt);
            howMuchText = activity.findViewById(R.id.styling_howmuch);

            styling_buy_butt.setOnClickListener(view -> {
                view.startAnimation(animation);
                sendBuy();
            });

            // neon
            TextView neon_price = activity.findViewById(R.id.neon_price);
            neon_price.setText(String.format("%s руб.", Samp.formatter.format(prices[0])));

            ConstraintLayout neonLayout = activity.findViewById(R.id.neon_layout);
            neonLayout.setOnClickListener(view -> {
                showColorPicker(COLOR_TYPE_NEON, false, false);
            });

            // lights
            TextView lights_price = activity.findViewById(R.id.lights_price);
            lights_price.setText(String.format("%s руб.", Samp.formatter.format(prices[1])));

            ConstraintLayout lightsLayout = activity.findViewById(R.id.lights_layout);
            lightsLayout.setOnClickListener(view -> {
                showColorPicker(COLOR_TYPE_LIGHT, false, false);
            });

            // toner
            TextView toner_price = activity.findViewById(R.id.toner_price);
            toner_price.setText(String.format("%s руб.", Samp.formatter.format(prices[2])));

            ConstraintLayout tonerLayout = activity.findViewById(R.id.toner_layout);
            tonerLayout.setOnClickListener(view -> {
                showColorPicker(COLOR_TYPE_TONER, true, true);
            });

            // Body 1
            TextView body1_price = activity.findViewById(R.id.body1_price);
            body1_price.setText(String.format("%s руб.", Samp.formatter.format(prices[3])));

            ConstraintLayout color_body1_layout = activity.findViewById(R.id.color_body1_layout);
            color_body1_layout.setOnClickListener(view -> {
                showColorPicker(COLOR_TYPE_BODY1, false, true);
                Samp.playUrlSound("http://files.liverussia.online/sounds/styling/shake_ballon.mp3");
            });

            // Body 2
            TextView body2_price = activity.findViewById(R.id.body2_price);
            body2_price.setText(String.format("%s руб.", Samp.formatter.format(prices[3])));

            ConstraintLayout color_body2_layout = activity.findViewById(R.id.color_body2_layout);
            color_body2_layout.setOnClickListener(view -> {
                showColorPicker(COLOR_TYPE_BODY2, false, true);
                Samp.playUrlSound("http://files.liverussia.online/sounds/styling/shake_ballon.mp3");
            });

            // Wheel
            TextView wheel_price = activity.findViewById(R.id.wheel_price);
            wheel_price.setText(String.format("%s руб.", Samp.formatter.format(prices[4])));

            ConstraintLayout wheel_layout = activity.findViewById(R.id.wheel_layout);
            wheel_layout.setOnClickListener(view -> {
                showColorPicker(COLOR_TYPE_WHEEL, false, true);
                Samp.playUrlSound("http://files.liverussia.online/sounds/styling/shake_ballon.mp3");
            });

            // Vinyls
            TextView vinils_price = activity.findViewById(R.id.vinils_price);
            vinils_price.setText(String.format("%s руб.", Samp.formatter.format(prices[5])));

            ConstraintLayout vinils_layout = activity.findViewById(R.id.vinils_layout);
            vinils_layout.setOnClickListener(view -> {
                toggleVinylsLayout(true);
            });

            ImageView styling_vin_left = activity.findViewById(R.id.styling_vin_left);
            styling_vin_left.setOnClickListener(view -> { // prev butt
                view.startAnimation(animation);
                changeVinyls(false);
            });

            ImageView styling_vin_right = activity.findViewById(R.id.styling_vin_right);
            styling_vin_right.setOnClickListener(view -> { // next butt
                view.startAnimation(animation);
                changeVinyls(true);
            });

            MaterialButton style_vin_cancel_butt = activity.findViewById(R.id.style_vin_cancel_butt);
            style_vin_cancel_butt.setOnClickListener(view -> {
                toggleVinylsLayout(false);
            });

            MaterialButton style_vin_ok_butt = activity.findViewById(R.id.style_vin_ok_butt);
            style_vin_ok_butt.setOnClickListener(view -> {
                toggleVinylsLayout(false);
                sendOnChooseVinil();
            });


            update(money, total);

            mainLayout = activity.findViewById(R.id.main_layout);
            mainLayout.setVisibility(View.VISIBLE);
        });
    }

    @Override
    public void onChange(ColorEnvelope color) {
        int[] argb = color.getArgb();
        onChangeColor(colorType, argb[1], argb[2], argb[3], argb[0]);
    }

    @Override
    public void colorSelected(ColorEnvelope color) {
        super.colorSelected(color);

        activity.runOnUiThread(() -> {
            styilyng_gui_layout.setVisibility(View.VISIBLE);
        });

        int[] argb = color.getArgb();
        sendChoosedColor(colorType, argb[1], argb[2], argb[3], argb[0]);

        if (colorType == COLOR_TYPE_BODY1 || colorType == COLOR_TYPE_BODY2 ||
                colorType == COLOR_TYPE_WHEEL) {
            Samp.playUrlSound("http://files.liverussia.online/sounds/styling/spray.mp3");
        }
    }

    @Override
    public void onClose() {
        super.onClose();

        activity.runOnUiThread(() -> {
            styilyng_gui_layout.setVisibility(View.VISIBLE);
        });
    }

    void update(int money, int total) {
        activity.runOnUiThread(() -> {
            moneyText.setText(Samp.formatter.format(money));

            howMuchText.setText(Samp.formatter.format(total));
        });
    }

    void showColorPicker(int type, boolean withAlpha, boolean withBrithness) {
        if(!isAvailableStyle(type)) return;

        activity.runOnUiThread(() -> {
            styilyng_gui_layout.setVisibility(View.GONE);
            showColorPicker(withAlpha, withBrithness, getActiveColor(type));
        });

        colorType = type;
    }

    void toggleVinylsLayout(boolean toggle) {
        activity.runOnUiThread(() -> {
            ConstraintLayout style_choose_vinyls_layout = activity.findViewById(R.id.style_choose_vinyls_layout);

            if (toggle) {
                styilyng_gui_layout.setVisibility(View.GONE);
                style_choose_vinyls_layout.setVisibility(View.VISIBLE);
            } else {
                styilyng_gui_layout.setVisibility(View.VISIBLE);
                style_choose_vinyls_layout.setVisibility(View.GONE);
            }
        });

    }

}
