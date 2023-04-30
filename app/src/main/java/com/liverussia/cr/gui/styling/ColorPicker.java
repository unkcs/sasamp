package com.liverussia.cr.gui.styling;

import static com.liverussia.cr.core.Samp.activity;

import android.view.View;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.google.android.material.button.MaterialButton;
import com.liverussia.cr.R;
import com.skydoves.colorpickerview.ColorEnvelope;
import com.skydoves.colorpickerview.ColorPickerView;
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener;
import com.skydoves.colorpickerview.sliders.AlphaSlideBar;
import com.skydoves.colorpickerview.sliders.BrightnessSlideBar;

public abstract class ColorPicker {
    int start_color;
    ConstraintLayout color_picker_main_layout;
    ColorPickerView colorPicker;
    AlphaSlideBar color_picker_alphaSlideBar;
    BrightnessSlideBar color_picker_brightnessSlide;

    public ColorPicker() {
        activity.runOnUiThread(() -> {

            color_picker_main_layout = activity.findViewById(R.id.color_picker_main_layout);

            color_picker_alphaSlideBar = activity.findViewById(R.id.color_picker_alphaSlideBar);
            color_picker_brightnessSlide = activity.findViewById(R.id.color_picker_brightnessSlide);

            // ok butt
            MaterialButton color_picker_ok_butt = activity.findViewById(R.id.color_picker_ok_butt);
            color_picker_ok_butt.setOnClickListener(view -> {
                colorSelected(colorPicker.getColorEnvelope());
            });

            // cancel
            MaterialButton color_picker_cancel_butt = activity.findViewById(R.id.color_picker_cancel_butt);
            color_picker_cancel_butt.setOnClickListener(view -> {
                colorPicker.setInitialColor(start_color);
                onChange(colorPicker.getColorEnvelope());
                onClose();
            });

            // change color
            colorPicker = activity.findViewById(R.id.colorPicker);
            colorPicker.setColorListener((ColorEnvelopeListener) (envelope, fromUser) ->
                    onChange(envelope)
            );
        });
    }

    public void colorSelected(ColorEnvelope color) {
        activity.runOnUiThread(() -> {
            color_picker_main_layout.setVisibility(View.GONE);
        });
    }

    public void onChange(ColorEnvelope color) {
    }

    public void onClose() {
        activity.runOnUiThread(() -> {
            color_picker_main_layout.setVisibility(View.GONE);
        });
    }

    public void showColorPicker(Boolean withAlpha, boolean withBrithness, int startColor) {
        activity.runOnUiThread(() -> {
            this.start_color = startColor;
            colorPicker.setInitialColor(startColor);

            color_picker_main_layout.setVisibility(View.VISIBLE);

            if (withBrithness) {
                color_picker_brightnessSlide.setVisibility(View.VISIBLE);
                colorPicker.attachBrightnessSlider(color_picker_brightnessSlide);
            } else {
                color_picker_brightnessSlide.setVisibility(View.GONE);
            }

            if (withAlpha) {
                color_picker_alphaSlideBar.setVisibility(View.VISIBLE);
                colorPicker.attachAlphaSlider(color_picker_alphaSlideBar);
            } else {
                color_picker_alphaSlideBar.setVisibility(View.GONE);
            }
        });
    }

}
