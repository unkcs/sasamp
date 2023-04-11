package com.liverussia.cr.gui.adapters;

import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.liverussia.cr.R;
import com.liverussia.cr.gui.tire_shop.ItemInfo;
import com.liverussia.cr.gui.tire_shop.ItemType;
import com.liverussia.cr.gui.tire_shop.TireShop;
import com.liverussia.launcher.ui.activity.StoryActivity;

import java.util.List;

import lombok.Getter;
import lombok.Setter;

public class TireShopItemsAdapter extends RecyclerView.Adapter<TireShopItemsAdapter.TireShopItemsViewHolder> {
    private final Activity activity;
    private final TireShop tireShop;

    private int selectedItem = -1;
    @Getter
    @Setter
    private List<ItemInfo> items;

    public TireShopItemsAdapter(Activity activity, List<ItemInfo> items, TireShop tireShop) {
        this.activity = activity;
        this.items = items;
        this.tireShop = tireShop;
    }

    @NonNull
    @Override
    public TireShopItemsViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(activity).inflate(R.layout.tire_shop_item, parent, false);
        return new TireShopItemsViewHolder(v);
    }

    @Override
    public void onBindViewHolder(@NonNull TireShopItemsViewHolder holder, int position) {
        ItemInfo itemInfo = this.items.get(position);
        holder.image.setImageResource(itemInfo.getResource());

        if (position == selectedItem && ItemType.CONCRETE_ITEM_IN_CATEGORY.equals(itemInfo.getType())) {
            holder.image.setImageResource(itemInfo.getResourceSelected());
        }


        holder.image.setOnClickListener(view -> {
            view.startAnimation(AnimationUtils.loadAnimation(activity,R.anim.button_click));

            new Handler().postDelayed(() -> {
                if (ItemType.MAIN_MENU_ITEM.equals(itemInfo.getType())) {
                    selectedItem = -1;
                    handleMainMenuAction(itemInfo);
                }

                if (ItemType.CONCRETE_ITEM_IN_CATEGORY.equals(itemInfo.getType())) {
                    selectedItem = position;
                    handleConcreteItemInCategoryAction(itemInfo);
                    this.notifyDataSetChanged();
                }
            }, 200);
        });
    }

    private void handleMainMenuAction(ItemInfo itemInfo) {

        ConstraintLayout dialog = activity.findViewById(R.id.dialog);

        if (ItemInfo.DISK_TYPE.equals(itemInfo)) {
            items.clear();
            items.addAll(ItemInfo.getDisks());

            ImageView goBackBtn = activity.findViewById(R.id.go_back_btn);
            goBackBtn.setVisibility(View.VISIBLE);
            ImageView closeBtn = activity.findViewById(R.id.close_btn);
            closeBtn.setVisibility(View.GONE);

            dialog.setVisibility(View.GONE);
        } else {
            dialog.setVisibility(View.VISIBLE);

            TextView dialogTitle = activity.findViewById(R.id.dialog_title);
            dialogTitle.setText(itemInfo.getItemName());

            tireShop.setDialogItemInfo(itemInfo);
            tireShop.resetBar();
        }

        notifyDataSetChanged();

    }

    private void handleConcreteItemInCategoryAction(ItemInfo itemInfo) {
        tireShop.handleConcreteItemInCategoryAction(itemInfo);
    }

    public void startStorySlider(int pos) {
        Intent intent = new Intent(activity, StoryActivity.class);
        intent.putExtra("position", pos);
        activity.startActivity(intent);
    }

    @Override
    public int getItemCount() {
        return items.size();
    }

    public static class TireShopItemsViewHolder extends RecyclerView.ViewHolder {

        ImageView image;

        public TireShopItemsViewHolder(View itemView) {
            super(itemView);

            image = itemView.findViewById(R.id.roulette_item);
        }
    }

    private void setAnimation(View view) {
        Animation anim = AnimationUtils.loadAnimation(activity, R.anim.button_click);
        view.startAnimation(anim);
    }
}