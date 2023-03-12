package com.liverussia.cr.gui;

import android.app.Activity;
import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.liverussia.cr.R;
import com.liverussia.launcher.domain.inscriptions.Inscriptions;

import java.util.ArrayList;
import java.util.List;

public class DuelsHud {

    private static final String KILL_LIST_ICON_RESOURCE_NAME = "kill_list_icon_weapon_";

    Activity activity;
    KillListAdapter adapter;
    RecyclerView kill_list;

    ConstraintLayout duels_kills_left_layout;
    TextView duels_kills_left_text;

    private ConstraintLayout topPanel;
    private ConstraintLayout statePanel;

    private TextView top1NameTextView;
    private TextView top2NameTextView;
    private TextView top3NameTextView;

    private TextView stateKillsCountTextView;
    private TextView stateDeathsCountTextView;

    native void init();

    public DuelsHud(Activity activity) {
        init();
        this.activity = activity;

        kill_list = activity.findViewById(R.id.kill_list);

        adapter = new KillListAdapter(activity);
        kill_list.setAdapter(adapter);

        //
        duels_kills_left_layout = activity.findViewById(R.id.duels_kills_left_layout);
        duels_kills_left_text = activity.findViewById(R.id.duels_kills_left_text);

        topPanel = activity.findViewById(R.id.top_panel);
        statePanel = activity.findViewById(R.id.state_panel);

        top1NameTextView = activity.findViewById(R.id.top1_name_text);
        top2NameTextView = activity.findViewById(R.id.top2_name_text);
        top3NameTextView = activity.findViewById(R.id.top3_name_text);

        stateKillsCountTextView = activity.findViewById(R.id.state_kills_count_text);
        stateDeathsCountTextView = activity.findViewById(R.id.state_deaths_count_text);
    }

    void showKillsLeft(boolean show, int kills, int needKills) {
        if(show) {
            activity.runOnUiThread(() -> {
                duels_kills_left_layout.setVisibility(View.VISIBLE);
                duels_kills_left_text.setText( String.format("%d/%d", kills, needKills) );
            });
        }
        else {
            activity.runOnUiThread( () -> {
                duels_kills_left_layout.setVisibility(View.GONE);
            });
        }
    }

    void addItem(String killertext, String deathtext, int gun, int team) {
        activity.runOnUiThread(() -> {
            if(kill_list.getVisibility() == View.GONE) {
                kill_list.setVisibility(View.VISIBLE);
            }

            adapter.addItem(killertext, deathtext, gun, team);
        });
    }

    void addTop(String top1Name, String top2Name, String top3Name, boolean show) {
        if(show) {
            activity.runOnUiThread(() -> {
                topPanel.setVisibility(View.VISIBLE);
                top1NameTextView.setText(Inscriptions.createDuelsTop1Inscription(top1Name));
                top2NameTextView.setText(Inscriptions.createDuelsTop2Inscription(top2Name));
                top3NameTextView.setText(Inscriptions.createDuelsTop3Inscription(top3Name));
            });
        } else {
            activity.runOnUiThread( () -> {
                topPanel.setVisibility(View.GONE);
            });
        }
    }

    void addStatistic(int killsCount, int deathsCount, boolean show) {
        if(show) {
            activity.runOnUiThread(() -> {
                statePanel.setVisibility(View.VISIBLE);
                stateKillsCountTextView.setText(String.valueOf(killsCount));
                stateDeathsCountTextView.setText(String.valueOf(deathsCount));
            });
        } else {
            activity.runOnUiThread( () -> {
                statePanel.setVisibility(View.GONE);
            });
        }
    }

    void clearKillList() {
        activity.runOnUiThread( () -> kill_list.setVisibility(View.GONE) );
        adapter.clearItems();
    }

    public class KillListAdapter  extends RecyclerView.Adapter<KillListAdapter.ViewHolder> {

        final LayoutInflater inflater;
        List<String> killertext;
        List<String> deathtext;
        List<Integer> gun;
        List<Integer> teamColor;

        KillListAdapter(Context context) {
            killertext = new ArrayList<>();
            deathtext = new ArrayList<>();
            gun = new ArrayList<>();
            teamColor = new ArrayList<>();

            this.inflater = LayoutInflater.from(context);
        }
        @Override
        public KillListAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {

            View view = inflater.inflate(R.layout.itemkilllist, parent, false);

            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
            if(killertext.get(position) == null) return;

            holder.kill_list_killer_textview.setText(killertext.get(position));
            holder.kill_list_death_textview.setText(deathtext.get(position));
            holder.kill_list_bg.setBackgroundTintList(ColorStateList.valueOf(teamColor.get(position)));

            String gunId = String.valueOf(gun.get(position));
            String resourceName = KILL_LIST_ICON_RESOURCE_NAME.concat(gunId);

            holder.kill_list_gun_icon.setBackgroundResource(getResId(resourceName));
        }

        public int getResId(String res) {
            return activity.getResources().getIdentifier(res, "drawable", activity.getPackageName());
        }

        @Override
        public int getItemCount() {
            return killertext.size();
        }

        public class ViewHolder extends RecyclerView.ViewHolder {
            final TextView kill_list_killer_textview;
            final TextView kill_list_death_textview;
            final ImageView kill_list_gun_icon;
            final ConstraintLayout kill_list_bg;

            ViewHolder(View view){
                super(view);
                kill_list_bg = view.findViewById(R.id.kill_list_bg);
                kill_list_killer_textview = view.findViewById(R.id.kill_list_killer_textview);
                kill_list_death_textview = view.findViewById(R.id.kill_list_death_textview);
                kill_list_gun_icon = view.findViewById(R.id.kill_list_gun_icon);
            }
        }

        void clearItems() {
            this.killertext.clear();
            this.deathtext.clear();
            this.gun.clear();
            this.teamColor.clear();
        }

        void addItem(String killer, String death, int gun, int team) {
            activity.runOnUiThread(() -> {
                if(this.killertext != null && this.killertext.size() > 2){
                    this.killertext.remove(0);
                    this.deathtext.remove(0);
                    this.gun.remove(0);
                    this.teamColor.remove(0);
                    notifyItemRemoved(0);
                    notifyItemRangeChanged(0, getItemCount());
                }
                this.killertext.add(killer);
                this.deathtext.add(death);
                this.gun.add(gun);
                switch (team) {
                    case 1: {
                        this.teamColor.add(Color.parseColor("#e53935"));
                        break;
                    }
                    case 2: {
                        this.teamColor.add(Color.parseColor("#165A7A"));
                        break;
                    }
                    default: {
                        this.teamColor.add(Color.parseColor("#FF282A2C"));
                    }
                }
                notifyItemInserted(this.killertext.size()-1);

            });

        }
    }
}
