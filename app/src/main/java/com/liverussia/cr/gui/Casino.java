package com.liverussia.cr.gui;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Color;
import android.os.Build;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.google.android.material.button.MaterialButton;
import com.liverussia.cr.R;
import com.liverussia.cr.core.Samp;
import com.liverussia.cr.gui.util.Utils;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Locale;

public class Casino {
    // global
    Activity activity;
    // chip
    private TextView casino_chip_action_caption;
    private TextView casino_chip_balance_text;
    private TextView casino_chip_currency_text;
    private ConstraintLayout casino_chip_bg_2;
    private TextView casino_chip_get_count;
    private EditText casino_chip_input;
    private MaterialButton casino_chip_buy_button;
    private boolean chip_isSell;
    private View casino_chip_exit_button;
    private ConstraintLayout casino_chip_main_layout;
    private ImageView casino_chip_back_button;

    public native void Init();
    public native void ClickChipButton(int buttonID, long input, boolean sell_or_buy);

    public Casino(Activity activity){
        Init();

        this.activity = activity;

        // покупка продажа фишек
        casino_chip_action_caption = activity.findViewById(R.id.casino_chip_action_caption);
        casino_chip_balance_text = activity.findViewById(R.id.casino_chip_balance_text);
        casino_chip_currency_text = activity.findViewById(R.id.casino_chip_currency_text);
        casino_chip_bg_2 = activity.findViewById(R.id.casino_chip_bg_2);
        casino_chip_get_count = activity.findViewById(R.id.casino_chip_get_count);
        casino_chip_back_button = activity.findViewById(R.id.casino_chip_back_button);
        casino_chip_back_button.setOnClickListener(view -> {
            ClickChipButton(0, 0, !chip_isSell);
            Utils.HideLayout(casino_chip_main_layout, true);
        });
        casino_chip_input = activity.findViewById(R.id.casino_chip_input);
        casino_chip_input.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                ChipUpdateGetting();
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        casino_chip_buy_button = activity.findViewById(R.id.casino_chip_buy_button);
        casino_chip_buy_button.setOnClickListener(view -> {
            ClickChipButton(1, getChipGetCount(), !chip_isSell);
            Utils.HideLayout(casino_chip_main_layout, true);
        });
        casino_chip_exit_button = activity.findViewById(R.id.casino_chip_exit_button);
        casino_chip_exit_button.setOnClickListener(view -> {
            ClickChipButton(2, getChipGetCount(), chip_isSell);
            Utils.HideLayout(casino_chip_main_layout, true);
        });
        casino_chip_main_layout = activity.findViewById(R.id.casino_chip_main_layout);
        Utils.HideLayout(casino_chip_main_layout, false);
    }

    @SuppressLint("UseCompatLoadingForDrawables")
    void OpenChipBuySell(boolean isBuy, int balance){
        chip_isSell = isBuy;

        activity.runOnUiThread(() ->{
            if(isBuy){
                casino_chip_action_caption.setText("Купить фишки");
                casino_chip_balance_text.setText(String.format("%d руб.", balance));
                casino_chip_currency_text.setText("1 = 1.000 руб.");
                casino_chip_bg_2.setBackground(activity.getDrawable(R.drawable.casino_chip_bg_2_buy));
                casino_chip_buy_button.setText("купить");
                casino_chip_buy_button.setTextColor(Color.parseColor("#017088"));
            } else{
                casino_chip_action_caption.setText("Продать фишки");
                casino_chip_balance_text.setText(String.format("%d фишек", balance));
                casino_chip_currency_text.setText("1 = 950 руб.");
                casino_chip_bg_2.setBackground(activity.getDrawable(R.drawable.casino_chip_bg_2_sell));
                casino_chip_buy_button.setText("продать");
                casino_chip_buy_button.setTextColor(Color.parseColor("#A01618"));
            }
            casino_chip_input.setText(null);
            Utils.ShowLayout(casino_chip_main_layout, true);
        });
        ChipUpdateGetting();
    }
    Long getChipGetCount(){
        if(casino_chip_input.getText().toString().isEmpty()) return 0L;
        Long tmp;
        try {
            tmp = Long.parseLong(casino_chip_input.getText().toString());
        } catch (Exception e) {
            return 0L;
        }

        if(tmp < 0 || tmp > 2000000000) return 0L;
        return tmp;
    }
    void ChipUpdateGetting(){
        activity.runOnUiThread(() -> {
            if(chip_isSell){
                casino_chip_get_count.setText(String.format("К оплате: %s руб.", Samp.formatter.format(getChipGetCount()* 1000L)));
            }else {
                casino_chip_get_count.setText(String.format("К получению: %s руб.", Samp.formatter.format(getChipGetCount()* 950L)));
            }
        });
    }
}
