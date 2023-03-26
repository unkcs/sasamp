package com.liverussia.cr.gui.tire_shop;

import android.app.Activity;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.liverussia.cr.R;
import com.liverussia.cr.gui.adapters.TireShopItemsAdapter;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class TireShop implements View.OnClickListener {

    private static final String COST_TEXT_POSTFIX = " РУБ.";

    private Activity activity;

    private ConstraintLayout itemPanel;
    private TextView costText;
    private RecyclerView recyclerItems;
    private ImageView closeBtn;
    private ImageView goBackBtn;

    private TireShopItemsAdapter tireShopItemsAdapter;
    private Animation animation;

    private List<ItemInfo> viewPagerItems;
    private DecimalFormat costFormat;

    {
        DecimalFormatSymbols otherSymbols = new DecimalFormatSymbols(Locale.getDefault());
        otherSymbols.setGroupingSeparator('.');
        costFormat = new DecimalFormat("###,###.###", otherSymbols);
    }

    public TireShop(Activity activity) {
        this.activity = activity;

        viewPagerItems = new ArrayList<>();

        activity.runOnUiThread(() -> {
            animation = AnimationUtils.loadAnimation(activity, R.anim.button_click);

            costText = activity.findViewById(R.id.cost_text);

            goBackBtn = activity.findViewById(R.id.go_back_btn);
            goBackBtn.setOnClickListener(this);
            closeBtn = activity.findViewById(R.id.close_btn);
            closeBtn.setOnClickListener(this);

            recyclerItems = activity.findViewById(R.id.tire_items_RV);
            recyclerItems.setHasFixedSize(true);
            LinearLayoutManager layoutManager = new LinearLayoutManager(activity, LinearLayoutManager.HORIZONTAL, false);
            recyclerItems.setLayoutManager(layoutManager);

            itemPanel = activity.findViewById(R.id.item_panel);
        });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.close_btn:
                v.startAnimation(animation);
                break;
            case R.id.go_back_btn:
                v.startAnimation(animation);

                viewPagerItems.clear();
                viewPagerItems.addAll(ItemInfo.getMainMenuItems());
                tireShopItemsAdapter = new TireShopItemsAdapter(activity, viewPagerItems);
                recyclerItems.setAdapter(tireShopItemsAdapter);

                //todo почему-то npe
//                tireShopItemsAdapter.setItems(ItemInfo.getMainMenuItems());
//                tireShopItemsAdapter.notifyDataSetChanged();


                closeBtn.setVisibility(View.VISIBLE);
                goBackBtn.setVisibility(View.GONE);
                break;
            default:
                break;
        }
    }

    public void showRendering(boolean toggle, int price) {
        activity.runOnUiThread(()-> {

            if (View.VISIBLE == itemPanel.getVisibility()) {
                costText.setText(costFormat.format(price).concat(COST_TEXT_POSTFIX));
                return;
            }

            if (toggle) {
                costText.setText(costFormat.format(price).concat(COST_TEXT_POSTFIX));
                itemPanel.setVisibility(View.VISIBLE);
                closeBtn.setVisibility(View.VISIBLE);
                viewPagerItems.addAll(ItemInfo.getMainMenuItems());

                addItemsIntoViewPager();
            } else {
                itemPanel.setVisibility(View.GONE);
            }
        });
    }



    private void addItemsIntoViewPager() {
        tireShopItemsAdapter = new TireShopItemsAdapter(activity, viewPagerItems);
        recyclerItems.setAdapter(tireShopItemsAdapter);
    }

    private int getResId(String res) {
        return activity.getResources().getIdentifier(res, "drawable", activity.getPackageName());
    }
}
