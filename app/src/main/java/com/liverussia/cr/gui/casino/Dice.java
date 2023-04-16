package com.liverussia.cr.gui.casino;

import static com.liverussia.cr.core.Samp.activity;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.liverussia.cr.R;
import com.liverussia.cr.core.Samp;
import com.nvidia.devtech.NvEventQueueActivity;

public class Dice {
    ConstraintLayout casino_dice_main_layout;
    TextView casino_dice_table_num;
    TextView casino_dice_crup_name;
    TextView casino_dice_time;
    TextView dice_text_bet;
    TextView casino_dice_text_bank;
    TextView dice_text_balance;
    TextView dice_text_player_name1;
    TextView dice_text_player_ready1;
    TextView dice_text_player_name2;
    TextView dice_text_player_ready2;
    TextView dice_text_player_name3;
    TextView dice_text_player_ready3;
    TextView dice_text_player_name4;
    TextView dice_text_player_ready4;
    TextView dice_text_player_name5;
    TextView dice_text_player_ready5;
    Button dice_butt_bet;
    Button dice_butt_dice;
    Button dice_butt_exit;
    ConstraintLayout dialog_layout;
    private boolean isTempHiden;

    private final int CASINO_DICE_BUTTON_BET = 0;
    private final int CASINO_DICE_BUTTON_DICE = 1;
    private final int CASINO_DICE_BUTTON_EXIT = 2;

    public Dice() {
        activity.runOnUiThread(() -> {
            casino_dice_main_layout = activity.findViewById(R.id.casino_dice_main_layout);

            casino_dice_table_num = activity.findViewById(R.id.casino_dice_table_num);
            dice_text_bet = activity.findViewById(R.id.dice_text_bet);
            casino_dice_text_bank = activity.findViewById(R.id.casino_dice_text_bank);
            dice_text_balance = activity.findViewById(R.id.dice_text_balance);

            dice_text_player_name1 = activity.findViewById(R.id.dice_text_player_name1);
            dice_text_player_ready1 = activity.findViewById(R.id.dice_text_player_ready1);

            dice_text_player_name2 = activity.findViewById(R.id.dice_text_player_name2);
            dice_text_player_ready2 = activity.findViewById(R.id.dice_text_player_ready2);

            dice_text_player_name3 = activity.findViewById(R.id.dice_text_player_name3);
            dice_text_player_ready3 = activity.findViewById(R.id.dice_text_player_ready3);

            dice_text_player_name4 = activity.findViewById(R.id.dice_text_player_name4);
            dice_text_player_ready4 = activity.findViewById(R.id.dice_text_player_ready4);

            dice_text_player_name5 = activity.findViewById(R.id.dice_text_player_name5);
            dice_text_player_ready5 = activity.findViewById(R.id.dice_text_player_ready5);

            casino_dice_crup_name = activity.findViewById(R.id.casino_dice_crup_name);
            casino_dice_time = activity.findViewById(R.id.casino_dice_time);

            dice_butt_bet = activity.findViewById(R.id.dice_butt_bet);
            dice_butt_dice = activity.findViewById(R.id.dice_butt_dice);
            dice_butt_exit = activity.findViewById(R.id.dice_butt_exit);

            dice_butt_bet.setOnClickListener(view -> {
                NvEventQueueActivity.getInstance().SendCasinoButt(CASINO_DICE_BUTTON_BET);
            });
            dice_butt_dice.setOnClickListener(view -> {
                NvEventQueueActivity.getInstance().SendCasinoButt(CASINO_DICE_BUTTON_DICE);
            });
            dice_butt_exit.setOnClickListener(view -> {
                NvEventQueueActivity.getInstance().SendCasinoButt(CASINO_DICE_BUTTON_EXIT);
            });

            casino_dice_main_layout.setVisibility(View.VISIBLE);
        });
    }

    void tempToggle(boolean toggle) {
        isTempHiden = !toggle;
        if (toggle) {
            activity.runOnUiThread(() -> casino_dice_main_layout.setVisibility(View.VISIBLE));
        } else {
            activity.runOnUiThread(() -> casino_dice_main_layout.setVisibility(View.GONE));
        }
    }

    void destroy() {
        activity.runOnUiThread(()-> {
            casino_dice_main_layout.setVisibility(View.GONE);
        });
    }


    @SuppressLint("DefaultLocale")
    void update(int tableID, int tableBet, int tableBank, int money,
              String player1name, int player1stat,
              String player2name, int player2stat,
              String player3name, int player3stat,
              String player4name, int player4stat,
              String player5name, int player5stat,
              int time, String crupName, int crupId) {
        activity.runOnUiThread(() ->
        {
            if (time > 1) {
                casino_dice_time.setText(String.format("До конца раунда: %d", time));
                casino_dice_time.setVisibility(View.VISIBLE);
            } else {
                casino_dice_time.setVisibility(View.GONE);
            }
            if(crupId != Samp.INVALID_PLAYER_ID)
                casino_dice_crup_name.setText(String.format("Крупье: %s[%d]", crupName, crupId));
            else
                casino_dice_crup_name.setText("Крупье: --");

            casino_dice_table_num.setText("Стол " + tableID);
            dice_text_bet.setText(Samp.formatter.format(tableBet) + " руб.");
            casino_dice_text_bank.setText(Samp.formatter.format(tableBank) + " руб.");
            dice_text_balance.setText(Samp.formatter.format(money) + " руб.");

            dice_text_player_name1.setText(player1name);
            if (player1stat == 0) {
                dice_text_player_ready1.setText("--");
            } else if (player1stat == 1) {
                dice_text_player_ready1.setText("++");
            } else {
                dice_text_player_ready1.setText(String.format("%d", player1stat));
            }
            dice_text_player_name2.setText(player2name);
            if (player2stat == 0) {
                dice_text_player_ready2.setText("--");
            } else if (player2stat == 1) {
                dice_text_player_ready2.setText("++");
            } else {
                dice_text_player_ready2.setText(String.format("%d", player2stat));
            }
            dice_text_player_name3.setText(player3name);
            if (player3stat == 0) {
                dice_text_player_ready3.setText("--");
            } else if (player3stat == 1) {
                dice_text_player_ready3.setText("++");
            } else {
                dice_text_player_ready3.setText(String.format("%d", player3stat));
            }
            dice_text_player_name4.setText(player4name);
            if (player4stat == 0) {
                dice_text_player_ready4.setText("--");
            } else if (player4stat == 1) {
                dice_text_player_ready4.setText("++");
            } else {
                dice_text_player_ready4.setText(String.format("%d", player4stat));
            }
            dice_text_player_name5.setText(player5name);
            if (player5stat == 0) {
                dice_text_player_ready5.setText("--");
            } else if (player5stat == 1) {
                dice_text_player_ready5.setText("++");
            } else {
                dice_text_player_ready5.setText(String.format("%d", player5stat));
            }
            if (!isTempHiden) {
                casino_dice_main_layout.setVisibility(View.VISIBLE);
            }
        });

    }
}


