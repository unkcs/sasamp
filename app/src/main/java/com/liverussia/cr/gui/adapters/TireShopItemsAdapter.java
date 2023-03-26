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

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.liverussia.cr.R;
import com.liverussia.cr.gui.tire_shop.ItemInfo;
import com.liverussia.cr.gui.tire_shop.ItemType;
import com.liverussia.launcher.ui.activity.StoryActivity;

import java.util.List;

import lombok.Getter;
import lombok.Setter;

public class TireShopItemsAdapter extends RecyclerView.Adapter<TireShopItemsAdapter.TireShopItemsViewHolder> {
    private Activity activity;
    @Getter
    @Setter
	private List<ItemInfo> items;

	public TireShopItemsAdapter(Activity activity, List<ItemInfo> items){
		 this.activity = activity;
		 this.items = items;
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


		holder.image.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(activity,R.anim.button_click));
                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        if (ItemType.MAIN_MENU_ITEM.equals(itemInfo.getType())) {
                            handleMainMenuAction(itemInfo);
                        }
                    }
                }, 200);
            }
        });
    }

    private void handleMainMenuAction(ItemInfo itemInfo) {
        ImageView goBackBtn = activity.findViewById(R.id.go_back_btn);
        goBackBtn.setVisibility(View.VISIBLE);
        ImageView closeBtn = activity.findViewById(R.id.close_btn);
        closeBtn.setVisibility(View.GONE);


        if (ItemInfo.DISK_TYPE.equals(itemInfo)) {
            items.clear();
            items.addAll(ItemInfo.getDisks());
        }

        notifyDataSetChanged();

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