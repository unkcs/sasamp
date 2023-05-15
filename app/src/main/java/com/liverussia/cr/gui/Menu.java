package com.liverussia.cr.gui;

import android.annotation.SuppressLint;
import android.app.Activity;

import com.liverussia.cr.R;
import com.liverussia.cr.gui.adapters.DialogMenuAdapter;
import com.liverussia.cr.gui.models.DataDialogMenu;
import com.liverussia.cr.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;

import android.content.Context;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

public class Menu {
    private View mRootView;
    private Activity activity;
    private LinearLayout menu_layout;
    private TextView menuTitle;
    private final Animation anim;
    private int index = -1;
    private final ArrayList<DataDialogMenu> dataDialogMenuArrayList = new ArrayList<>();

    @SuppressLint("InflateParams")
    public Menu(Activity aactivity) {
        activity = aactivity;
        anim = AnimationUtils.loadAnimation(aactivity, R.anim.button_click);
        menu_layout = aactivity.findViewById(R.id.main_menu_layout_new_layout);
        aactivity.findViewById(R.id.br_menu_close_new).setOnClickListener(view -> {
            close();
        });
        this.mRootView = ((LayoutInflater) aactivity.getSystemService(Context.LAYOUT_INFLATER_SERVICE)).inflate(R.layout.menu_action_dialog, null, false);
        Utils.HideLayout(menu_layout,false);
    }

    public void Update(boolean z) {
        RecyclerView recyclerView = activity.findViewById(R.id.br_rec_view_menu);
        /*if (this.index == -1) {
            TransitionManager.beginDelayedTransition(recyclerView);
        }*/
        this.index = -1;
        this.menuTitle = activity.findViewById(R.id.br_menu_title);
        if (!z) {
            setMenu();
            this.menuTitle.setText("Действия");
            setDataInRecyclerView((dataDialogMenu, view) -> {
                index = dataDialogMenu.getId();
                view.startAnimation(anim);
                new Handler().postDelayed(() -> {
                    try {
                        NvEventQueueActivity.getInstance().sendRPC(1, String.valueOf(index).getBytes("windows-1251"), index);
                      //  Toast.makeText(activity, String.valueOf(index), Toast.LENGTH_SHORT).show();
                        close();
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                }, 300);
            }, this.dataDialogMenuArrayList, recyclerView, mRootView, 4);
        }
    }

    public void ShowMenu()
    {
        Update(false);
        Utils.ShowLayout(menu_layout, true);
    }

    private void setMenu() {
        this.dataDialogMenuArrayList.clear();
        this.dataDialogMenuArrayList.add(new DataDialogMenu(1, R.drawable.menu_icon_gps, "Навигатор"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(2, R.drawable.menu_icon_mm, "Меню"));
       // this.dataDialogMenuArrayList.add(new DataDialogMenu(3, R.drawable.br_menu_chat, "Общение"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(3, R.drawable.menu_icon_inv, "Инвентарь"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(4, R.drawable.menu_icon_anim, "Анимации"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(5, R.drawable.br_menu_ruble, "Донат"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(6, R.drawable.menu_icon_car, "Автомобили"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(7, R.drawable.menu_report_icon, "Жалоба"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(8, R.drawable.menu_promocode_icon, "Промокод"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(9, R.drawable.players_icon, "Игроки"));
        this.dataDialogMenuArrayList.add(new DataDialogMenu(10, R.drawable.menu_icon_family, "Семья"));
    }

    private void setDataInRecyclerView(DialogMenuAdapter.OnUserClickListener onUserClickListener, ArrayList<DataDialogMenu> arrayList, RecyclerView recyclerView, final View view, int i) {
        DialogMenuAdapter dialogMenuAdapter = new DialogMenuAdapter(arrayList, onUserClickListener);
        recyclerView.setLayoutManager(new GridLayoutManager(view.getContext(), i) {
            @Override
            public boolean checkLayoutParams(RecyclerView.LayoutParams layoutParams) {
                float f = 30.0f / view.getResources().getDisplayMetrics().density;
                int i2 = (int) f;
                layoutParams.setMarginStart(i2);
                layoutParams.setMarginEnd(i2);
                layoutParams.setMargins(0, i2, 0, 0);
                layoutParams.width = (int) (((float) (getWidth() / getSpanCount())) - f);
                return true;
            }
        });
        recyclerView.setAdapter(dialogMenuAdapter);
    }

    public void close() {
        Utils.HideLayout(menu_layout, true);
      //  NvEventQueueActivity.getInstance().togglePlayer(0);
    }
}