package com.liverussia.cr.gui.styling;

import android.app.Activity;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.liverussia.cr.R;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

public class Styling {

    private Activity activity;

    private ConstraintLayout mainLayout;

    private Animation animation;

    private TextView moneyText;

    private LinearLayout exitLayout;
    private ImageView exitImage;

    private LinearLayout stockLayout;
    private ImageView stockImage;

    native void toStock();
    native void exitClick();

    public Styling(Activity activity, int money)
    {
        this.activity = activity;

        this.activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                animation = AnimationUtils.loadAnimation(activity, R.anim.button_click);

                moneyText = (TextView)activity.findViewById(R.id.styling_money);
                DecimalFormatSymbols dfs = new DecimalFormatSymbols();
                dfs.setGroupingSeparator(' ');
                DecimalFormat df = new DecimalFormat("###,###,###", dfs);
                moneyText.setText(df.format(money));

                exitLayout = activity.findViewById(R.id.styling_exit);
                exitImage = activity.findViewById(R.id.styling_exit_image);
                exitLayout.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        exitImage.startAnimation(animation);
                        exitClick();
                        toStock();
                    }
                });

                stockLayout = activity.findViewById(R.id.styling_stock);
                stockImage = activity.findViewById(R.id.styling_stock_image);
                stockLayout.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        stockImage.startAnimation(animation);
                        toStock();
                    }
                });

                mainLayout = activity.findViewById(R.id.main_layout);
                mainLayout.setVisibility(View.VISIBLE);
            }
        });
    }

}
