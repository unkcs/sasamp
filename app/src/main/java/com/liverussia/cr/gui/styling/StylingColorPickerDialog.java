package com.liverussia.cr.gui.styling;

import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;

import com.google.android.material.dialog.MaterialAlertDialogBuilder;
import com.liverussia.cr.R;
import com.skydoves.colorpickerview.ColorEnvelope;
import com.skydoves.colorpickerview.ColorPickerView;
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener;

public class StylingColorPickerDialog extends MaterialAlertDialogBuilder implements View.OnClickListener {

    private final Styling styling;

    private AlertDialog dialog;
    private Animation animation;

    private ColorPickerView colorPickerView;

    private int type;

    public RGB RGB1;

    public StylingColorPickerDialog(Styling styling, int type) {
        super(styling.activity);
        this.styling = styling;
        this.type = type;
        createDialog();
    }

    private void createDialog() {
        View view = LayoutInflater.from(styling.activity).inflate(R.layout.item_color_picker, null);
        setView(view);
        initViewElements(view);
        dialog = show();
        dialog.getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));

        setOnDialogCloseListener();
        animation = AnimationUtils.loadAnimation(getContext(), R.anim.button_click);
    }

    class RGB {
        double r;
        double g;
        double b;
    }

    public RGB colorConverter(Long hexValue)
    {
        RGB rgbColor = new RGB();
        rgbColor.r = (hexValue & 0xFF0000) >> 16;
        rgbColor.g = (hexValue & 0xFF00) >> 8;
        rgbColor.b = (hexValue & 0xFF);

        return rgbColor;
    }

    private void initViewElements(View view) {

        colorPickerView = view.findViewById(R.id.color_picker_rgb);
        colorPickerView.selectCenter();

        colorPickerView.setColorListener(new ColorEnvelopeListener() {
            @Override
            public void onColorSelected(ColorEnvelope colorEnvelope, boolean z) {
                RGB1 = new RGB();
                RGB1 = colorConverter(Long.decode("#" + colorEnvelope.getHexCode()));
                styling.onChangeColor(
                        type,
                        colorEnvelope.getArgb()[1],
                        colorEnvelope.getArgb()[2],
                        colorEnvelope.getArgb()[3],
                        colorEnvelope.getArgb()[0]
                );
                Log.d("x1y2z", " " + RGB1.r + " " + RGB1.g + " " + RGB1.b);
            }
        });


        TextView saveButton = view.findViewById(R.id.color_picker_save_btn);
        saveButton.setOnClickListener(this);

        TextView cancelButton = view.findViewById(R.id.color_picker_cancel_btn);
        cancelButton.setOnClickListener(this);
    }

    private void setOnDialogCloseListener() {
        dialog.setOnDismissListener(alertDialog -> {

        });
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.color_picker_save_btn:
                view.startAnimation(animation);
                styling.sendChoosedColor(type, (int)RGB1.r, (int)RGB1.g, (int)RGB1.b);
                dialog.dismiss();
                break;
            case R.id.color_picker_cancel_btn:
                view.startAnimation(animation);
                dialog.dismiss();
                break;
            default:
                break;
        }
    }
}
