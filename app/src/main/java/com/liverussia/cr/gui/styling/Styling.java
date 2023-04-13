package com.liverussia.cr.gui.styling;

import android.app.Activity;
import android.content.DialogInterface;
import android.graphics.Color;
import android.util.Log;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.liverussia.cr.R;
import com.liverussia.launcher.ui.dialogs.EnterNicknameDialog;
import com.skydoves.colorpickerview.ColorEnvelope;
import com.skydoves.colorpickerview.ColorPickerDialog;
import com.skydoves.colorpickerview.ColorPickerView;
import com.skydoves.colorpickerview.flag.FlagView;
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

public class Styling {

    public Activity activity;

    private ConstraintLayout mainLayout;

    private Animation animation;

    private TextView moneyText;
    private TextView howMuchText;

    private LinearLayout exitLayout;
    private ImageView exitImage;

    private LinearLayout stockLayout;
    private ImageView stockImage;

    private ConstraintLayout neonLayout;
    ConstraintLayout wheel_layout;
    private ConstraintLayout lightsLayout;
    private ConstraintLayout tonerLayout;
    private ConstraintLayout colorLayout;
    private ImageView styling_buy_butt;
    private ImageView arrowRight;
    private ImageView arrowLeft;

    native void toStock();
    native void exitClick();

    native void sendChoosedColor(int type, int r, int g, int b);

    native void sendBuy();

    native void sendClickedCameraArrow(int rightorleft);

    native void onChangeColor(int type, int r, int g, int b, int a);

    public Styling(Activity activity, int money, int total, int price1, int price2, int price3, int price4)
    {
        this.activity = activity;

        this.activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                animation = AnimationUtils.loadAnimation(activity, R.anim.button_click);

                wheel_layout = activity.findViewById(R.id.wheel_layout);
                neonLayout = activity.findViewById(R.id.neon_layout);
                lightsLayout = activity.findViewById(R.id.lights_layout);
                tonerLayout = activity.findViewById(R.id.toner_layout);
                colorLayout = activity.findViewById(R.id.color_layout);

                moneyText = (TextView)activity.findViewById(R.id.styling_money);

                exitLayout = activity.findViewById(R.id.styling_exit);
                exitImage = activity.findViewById(R.id.styling_exit_image);
                exitLayout.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        exitImage.startAnimation(animation);
                        mainLayout.setVisibility(View.GONE);
                        exitClick();
                        toStock();
                    }
                });

                stockLayout = activity.findViewById(R.id.styling_stock);
                stockImage = activity.findViewById(R.id.styling_stock_image);
                stockLayout.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        view.startAnimation(animation);
                        stockImage.startAnimation(animation);
                        toStock();
                    }
                });

                styling_buy_butt = activity.findViewById(R.id.styling_buy_butt);
                howMuchText = activity.findViewById(R.id.styling_howmuch);

                styling_buy_butt.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        view.startAnimation(animation);
                        sendBuy();
                    }
                });

                arrowRight = activity.findViewById(R.id.styling_arrow_right);
                arrowRight.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        view.startAnimation(animation);
                        sendClickedCameraArrow(1);
                    }
                });

                arrowLeft = activity.findViewById(R.id.styling_arrow_left);
                arrowLeft.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        view.startAnimation(animation);
                        sendClickedCameraArrow(0);
                    }
                });

                neonLayout.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        view.startAnimation(animation);
                        showColorPicker(0);
                    }
                });
                lightsLayout.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        view.startAnimation(animation);
                        showColorPicker(1);
                    }
                });

                tonerLayout.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        view.startAnimation(animation);
                        showColorPicker(2);
                        // Toast.makeText(activity, "Скоро...", Toast.LENGTH_SHORT).show();
                    }
                });

                colorLayout.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        view.startAnimation(animation);
                        showColorPicker(3);
                      //  Toast.makeText(activity, "Скоро...", Toast.LENGTH_SHORT).show();
                    }
                });

                wheel_layout.setOnClickListener(view -> {
                    view.startAnimation(animation);
                    showColorPicker(4);
                });

                updateEverything(money, total);

                mainLayout = activity.findViewById(R.id.main_layout);
                mainLayout.setVisibility(View.VISIBLE);
            }
        });
    }

    void updateEverything(int money, int total) {
        DecimalFormatSymbols dfs = new DecimalFormatSymbols();
        dfs.setGroupingSeparator(' ');
        DecimalFormat df = new DecimalFormat("###,###,###", dfs);
        moneyText.setText(df.format(money));

        DecimalFormatSymbols dfs1 = new DecimalFormatSymbols();
        dfs1.setGroupingSeparator('.');
        DecimalFormat df1 = new DecimalFormat("###,###,###", dfs1);
        howMuchText.setText(df1.format(total));
    }

    void showColorPicker(int type)
    {
        new StylingColorPickerDialog(this, type);
        /*ColorPickerDialog.Builder builder = new ColorPickerDialog.Builder(activity)
                .setTitle("Выбор цвета")
                .setPreferenceName("MyColorPickerDialog")
                .setPositiveButton("Сохранить",
                        new ColorEnvelopeListener() {
                            @Override
                            public void onColorSelected(ColorEnvelope envelope, boolean fromUser) {
                                int[] rgb = new int[3];
                                rgb[0] = Color.red(envelope.getColor());
                                rgb[1] = Color.green(envelope.getColor());
                                rgb[2] = Color.blue(envelope.getColor());
                                Log.d("x1y2z", " " + rgb[0] + rgb[1] + rgb[2]);
                                sendChoosedColor(type, rgb[0], rgb[1], rgb[2]);
                            }
                        })
                .setNegativeButton("Отменить",
                        new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                dialogInterface.dismiss();
                            }
                        })
                .attachAlphaSlideBar(true) // the default value is true.
                .attachBrightnessSlideBar(true)  // the default value is true.
                .setBottomSpace(12);// set a bottom space between the last slidebar and buttons.//

        builder.show(); // shows the dialog*/
    }

}
