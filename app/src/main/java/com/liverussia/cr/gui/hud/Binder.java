package com.liverussia.cr.gui.hud;

import static com.liverussia.cr.core.Samp.activity;

import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.ColorInt;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.FragmentActivity;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.factor.bouncy.BouncyRecyclerView;
import com.google.android.material.button.MaterialButton;
import com.liverussia.cr.R;
import com.liverussia.cr.core.Samp;

import org.jetbrains.annotations.NotNull;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import eltos.simpledialogfragment.SimpleDialog;
import eltos.simpledialogfragment.color.SimpleColorDialog;

public class Binder {
    BouncyRecyclerView binder_recycle;
    public BinderAdapter adapter;
   // List<BinderItem> list;
    ConstraintLayout binder_add_layout;
    public static int lastItemId;

    public int lastSelectId;

    public Binder() {
        binder_recycle = activity.findViewById(R.id.binder_recycle);

        LinearLayoutManager mLayoutManager = new GridLayoutManager(activity, 3);
        binder_recycle.setLayoutManager(mLayoutManager);
        binder_recycle.setOrientation(LinearLayoutManager.VERTICAL);
        adapter = new BinderAdapter();
        binder_recycle.setAdapter(adapter);

        binder_add_layout = activity.findViewById(R.id.binder_add_layout);

        ImageView binder_add_butt = activity.findViewById(R.id.binder_add_butt);
        binder_add_butt.setOnClickListener(view -> {
            activity.runOnUiThread(()-> {
                EditText binder_add_edittext = activity.findViewById(R.id.binder_add_edittext);
                binder_add_edittext.setText("");

                binder_add_layout.setVisibility(View.VISIBLE);

            });
        });

        MaterialButton binder_add_ok_butt = activity.findViewById(R.id.binder_add_ok_butt);
        binder_add_ok_butt.setOnClickListener(view -> {
            EditText binder_add_edittext = activity.findViewById(R.id.binder_add_edittext);

            if(binder_add_edittext.getText().length() > 400) {
                Toast.makeText(activity, "Не больше 400 символов!", Toast.LENGTH_SHORT).show();
                return;
            }
            adapter.addItem( new BinderItem(binder_add_edittext.getText().toString(), 0xff009688) );

            binder_add_layout.setVisibility(View.GONE);
        });

        ImageView binder_exit_butt = activity.findViewById(R.id.binder_exit_butt);
        binder_exit_butt.setOnClickListener(view -> {
            toggleMainLayout(false);
        });
    }

    public static void toggleMainLayout(boolean toggle) {
        activity.runOnUiThread(()-> {
            ConstraintLayout binder_main_layout = activity.findViewById(R.id.binder_main_layout);

            if(toggle)
                binder_main_layout.setVisibility(View.VISIBLE);
            else
                binder_main_layout.setVisibility(View.GONE);
        });
    }

    public static class BinderAdapter extends BouncyRecyclerView.Adapter<BinderAdapter.ViewHolder>  {
        final String path = activity.getExternalFilesDir(null).toString() + "/binder.dat";
        private final LayoutInflater inflater;
        public List<BinderItem>  list = new ArrayList<>();

        BinderAdapter() {
            loadData();
           // this.list = list;
            this.inflater = LayoutInflater.from(activity);
        }

        @NotNull
        @Override
        public BinderAdapter.ViewHolder onCreateViewHolder(@NotNull ViewGroup parent, int viewType) {

            View view = inflater.inflate(R.layout.binder_item, parent, false);

            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(@NonNull ViewHolder holder, int pos) {
            holder.binder_text.setText( list.get(pos).text );
            holder.binder_bubble.setColorFilter( list.get(pos).color );
        }


        @Override
        public int getItemCount() {
            return list.size();
        }

        @Override
        public void onItemMoved(int fromPosition, int toPosition) {
            //example of handling reorder
            BinderItem item = list.remove(fromPosition);
            list.add(toPosition, item);
            notifyItemMoved(fromPosition, toPosition);

            Toast.makeText(activity, "Перемещено", Toast.LENGTH_SHORT).show();

            saveData();
        }

        @Override
        public void onItemReleased(@NonNull RecyclerView.ViewHolder viewHolder) {

        }

        @Override
        public void onItemSelected(@Nullable RecyclerView.ViewHolder viewHolder) {

        }

        public void changeColor(int color) {
            list.get(lastItemId).color = color;
            notifyItemChanged(lastItemId);
            saveData();
        }

        @Override
        public void onItemSwipedToEnd(@NonNull RecyclerView.ViewHolder viewHolder, int i) {
            lastItemId = i;

            SimpleColorDialog.build()
                    .title("Выберите цвет")
                    .allowCustom(true)
                    .colorPreset(0xff009688)
                    .show((FragmentActivity) activity, "BINDERCOLOR");
        }

        @Override
        public void onItemSwipedToStart(@NonNull RecyclerView.ViewHolder viewHolder, int i) {
            list.remove(i);
            notifyItemRemoved(i);
            Toast.makeText(activity, "Удалено", Toast.LENGTH_SHORT).show();
            saveData();
        }

        public class ViewHolder extends RecyclerView.ViewHolder {
            final TextView binder_text;
            final ImageView binder_bubble;
            ViewHolder(View view){
                super(view);
                binder_text = view.findViewById(R.id.binder_text);
                binder_bubble = view.findViewById(R.id.binder_bubble);

                view.setOnClickListener(view1 -> {
                    toggleMainLayout(false);

                    int pos = getBindingAdapterPosition();

                    String[] messages = list.get(pos).text.split(";");

                    for (String message : messages) {
                        try {
                            Chat.SendChatMessage(message.getBytes("windows-1251"));
                        } catch (UnsupportedEncodingException e) {
                            throw new RuntimeException(e);
                        }
                    }
                    Chat.chat_input.getText().clear();

                });
            }
        }

        public void saveData() {

            try(ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(path)))
            {
                oos.writeObject(list);
                System.out.println("File has been written");
            }
            catch(Exception ex){

                System.out.println(ex.getMessage());
            }
        }

        public void loadData() {

            try(ObjectInputStream ois = new ObjectInputStream(new FileInputStream(path)))
            {

                list = (ArrayList<BinderItem>)ois.readObject();
            }
            catch(Exception ex){

                System.out.println(ex.getMessage());
            }
        }

        public void addItem(BinderItem item) {
            list.add(item);
            notifyItemInserted(getItemCount());
            saveData();
        }
    }
}
