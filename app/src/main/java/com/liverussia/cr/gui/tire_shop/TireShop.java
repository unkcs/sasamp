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

    private ConstraintLayout tireShopHud;
    private TextView costText;
    private RecyclerView recyclerItems;
    private ImageView closeBtn;
    private ImageView goBackBtn;
    private ImageView stockBtn;

    private TireShopItemsAdapter tireShopItemsAdapter;
    private Animation animation;

    private List<ItemInfo> viewPagerItems;
    private DecimalFormat costFormat;

    {
        DecimalFormatSymbols otherSymbols = new DecimalFormatSymbols(Locale.getDefault());
        otherSymbols.setGroupingSeparator('.');
        costFormat = new DecimalFormat("###,###.###", otherSymbols);
    }

    native void sendClickItem(int buttonId, float value);

    native void chooseDisk(int diskId);

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
            stockBtn = activity.findViewById(R.id.stock_btn);
            stockBtn.setOnClickListener(this);

            recyclerItems = activity.findViewById(R.id.tire_items_RV);
            recyclerItems.setHasFixedSize(true);
            LinearLayoutManager layoutManager = new LinearLayoutManager(activity, LinearLayoutManager.HORIZONTAL, false);
            recyclerItems.setLayoutManager(layoutManager);

            tireShopHud = activity.findViewById(R.id.tire_shop_hud);
        });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.close_btn:
                v.startAnimation(animation);
                sendClickItem(7, 0);
                break;
            case R.id.go_back_btn:
                v.startAnimation(animation);
                doGoBackButtonAction();
                break;
            case R.id.stock_btn:
                v.startAnimation(animation);
            default:
                break;
        }
    }

    private void doGoBackButtonAction() {
        viewPagerItems.clear();
        viewPagerItems.addAll(ItemInfo.getMainMenuItems());
        tireShopItemsAdapter = new TireShopItemsAdapter(activity, viewPagerItems, this);
        recyclerItems.setAdapter(tireShopItemsAdapter);

        //todo почему-то npe
//                tireShopItemsAdapter.setItems(ItemInfo.getMainMenuItems());
//                tireShopItemsAdapter.notifyDataSetChanged();

        closeBtn.setVisibility(View.VISIBLE);
        goBackBtn.setVisibility(View.GONE);
    }

    public void handleConcreteItemInCategoryAction(ItemInfo itemInfo) {
        if (ItemInfo.DISK_TYPE.getTypeId() == itemInfo.getTypeId()) {
            chooseDisk(itemInfo.getClientItemId());
        }
    }

    public void showRendering(boolean toggle, int price) {
        activity.runOnUiThread(()-> {
            if (toggle) {
                if (View.VISIBLE == tireShopHud.getVisibility()) {
                    costText.setText(costFormat.format(price).concat(COST_TEXT_POSTFIX));
                    return;
                }

                costText.setText(costFormat.format(price).concat(COST_TEXT_POSTFIX));
                tireShopHud.setVisibility(View.VISIBLE);
                closeBtn.setVisibility(View.VISIBLE);
                viewPagerItems.addAll(ItemInfo.getMainMenuItems());

                addItemsIntoViewPager();
            } else {
                tireShopHud.setVisibility(View.GONE);
            }
        });
    }

    private void addItemsIntoViewPager() {
        tireShopItemsAdapter = new TireShopItemsAdapter(activity, viewPagerItems, this);
        recyclerItems.setAdapter(tireShopItemsAdapter);
    }

    private int getResId(String res) {
        return activity.getResources().getIdentifier(res, "drawable", activity.getPackageName());
    }
}
