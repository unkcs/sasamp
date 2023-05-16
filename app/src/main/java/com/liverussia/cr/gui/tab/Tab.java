package com.liverussia.cr.gui.tab;

import static com.liverussia.cr.core.Samp.activity;

import android.app.Activity;
import android.content.Context;
import android.os.Build;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.view.animation.AnimationUtils;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.RequiresApi;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.liverussia.cr.R;
import com.liverussia.cr.gui.util.LinearLayoutManagerWrapper;
import com.liverussia.cr.gui.util.Utils;
import com.nvidia.devtech.NvEventQueueActivity;

public class Tab {
  //  public native void initTab();

    public native void onTabClose();
    public ImageView mClearImg;
    public ConstraintLayout tab_main;
    public RecyclerView mPlayersList;
    public EditText mSearch;
    public ImageView mSearchImg;
    private TabAdapter mTabAdapter;

    public Tab() {
        activity.runOnUiThread(() -> {
            tab_main = activity.findViewById(R.id.tab_main);

            mSearch = activity.findViewById(R.id.search_view);

            mSearchImg = activity.findViewById(R.id.icon_search_view);
            mClearImg = activity.findViewById(R.id.icon_clear_search_text);

            mClearImg.setOnClickListener(view -> clearEditText());
            mSearchImg.setOnClickListener(view -> {
                clearEditText();
            });
            mPlayersList = activity.findViewById(R.id.players_list);

            mPlayersList.setLayoutManager(new LinearLayoutManagerWrapper(activity));
            mTabAdapter = new TabAdapter();
            mPlayersList.setAdapter(mTabAdapter);
            mSearch.setText("");
            setVisibleIconInSearchView("");

            ((Button) activity.findViewById(R.id.players_list_close)).setOnClickListener(view -> {
                //  view.startAnimation(AnimationUtils.loadAnimation(activity, R.anim.button_click));
                close();
            });

            mSearch.addTextChangedListener(new TextWatcher() {
                @Override
                public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                }

                @Override
                public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                }

                @RequiresApi(api = Build.VERSION_CODES.N)
                @Override
                public void afterTextChanged(Editable editable) {
                    mTabAdapter.updateSearch(editable.toString());
                    setVisibleIconInSearchView(editable.toString());
                }
            });


            Utils.ShowLayout(this.tab_main, true);
        });
    }

    public void clearEditText() {
        mSearch.setText("");
        setVisibleIconInSearchView("");
    }

    public void close() {
        ((InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(mSearch.getWindowToken(), 0);

        this.mPlayersList.getRecycledViewPool().clear();
        Utils.HideLayout(tab_main, true);
        onTabClose();
    }

    public void setStat(int id, int color, String name, int score, int ping) {
       // Log.d("afd", "Update");

        activity.runOnUiThread( () -> {
            mTabAdapter.addItem( new PlayerData(id, color, name, score, ping) );
        });
    }

    public void setVisibleIconInSearchView(String str) {
        activity.runOnUiThread( () ->
        {
            if (str.isEmpty()) {
                this.mSearchImg.setVisibility(View.VISIBLE);
                this.mClearImg.setVisibility(View.INVISIBLE);
                return;
            }
            this.mSearchImg.setVisibility(View.INVISIBLE);
            this.mClearImg.setVisibility(View.VISIBLE);
        });
    }

}
