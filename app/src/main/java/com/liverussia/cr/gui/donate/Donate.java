package com.liverussia.cr.gui.donate;

import android.app.Activity;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.net.Uri;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.GridLayoutManager;

import com.factor.bouncy.BouncyRecyclerView;
import com.google.android.material.button.MaterialButton;
import com.liverussia.cr.R;
import com.liverussia.cr.core.Samp;
import com.liverussia.launcher.config.Config;
import com.liverussia.launcher.domain.enums.NativeStorageElements;
import com.liverussia.launcher.domain.enums.StorageElements;
import com.liverussia.launcher.domain.messages.ErrorMessage;
import com.liverussia.launcher.service.ActivityService;
import com.liverussia.launcher.service.impl.ActivityServiceImpl;
import com.liverussia.launcher.storage.NativeStorage;
import com.liverussia.launcher.storage.Storage;
import com.liverussia.launcher.ui.dialogs.ReCaptchaDialog;
import com.liverussia.launcher.utils.Validator;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Donate {
    ConstraintLayout donate_main_layout;
    ConstraintLayout donate_buy_lc_layout;

    ConstraintLayout donate_category_buy_all;
    ConstraintLayout donate_category_my_item;
    ConstraintLayout donate_category_buy_cars;
    ConstraintLayout donate_category_buy_skins;
    ConstraintLayout donate_category_buy_acs;
    ConstraintLayout donate_category_buy_other;
    ConstraintLayout donate_category_buy_vip;

    BouncyRecyclerView donate_recycle;
    DonateAdapter donateAdapter;
    TextView donate_balance_text;
    ImageView donate_exit_button;
    MaterialButton donate_check_butt;
    MaterialButton donate_deposit_butt;
    MaterialButton donate_history_butt;
    MaterialButton donate_change_butt;
    MaterialButton donate_buy_lc_buy_btn;
    MaterialButton donate_buy_lc_buy_via_qiwi_btn;
    MaterialButton donate_byu_lc_cancel_btn;
    EditText donate_buy_lc_email_input;
    EditText donate_buy_lc_sum_input;
    CheckBox iAmNotRobotCheckBox;


    int active_category_type = -1;
    View active_caterory_view = null;
    String captchaToken;
    ActivityService activityService;

    static final int CATEGORY_ALL = 0;
    static final int CATEGORY_CARS = 1;
    static final int CATEGORY_SKINS = 2;
    static final int CATEGORY_ACS = 3;
    static final int CATEGORY_VIP = 4;
    static final int CATEGORY_OTHER = 5;

    Activity activity;
    final List<DonateItem> allItems = new ArrayList<>();

    native void native_hide();

    static final int ACTION_TYPE_CLICK_CHECK = 0;
    static final int ACTION_TYPE_CLICK_HISTORY = 1;
    static final int ACTION_TYPE_CLICK_CONVERT = 2;
    static final int ACTION_TYPE_CLICK_ITEM = 3;
    native void sendAction(int type);

    static final int DONATE_OTHER_HOUSE_SLOT = 0;
    static final int DONATE_OTHER_BIZ_SLOT = 1;
    static final int DONATE_OTHER_WARN = 2;
    static final int DONATE_OTHER_LICENSES = 3;
    static final int DONATE_OTHER_CHANGE_FAM_NAME = 4;
    static final int DONATE_OTHER_CHANGE_SIM = 5;
    static final int DONATE_OTHER_VOEN = 6;
    static final int DONATE_OTHER_ZAKON = 7;
    static final int DONATE_OTHER_MED = 8;
    static final int DONATE_OTHER_CHANGE_NICK = 9;
    static final int DONATE_OTHER_CHANGE_SEX = 10;
    static final int DONATE_OTHER_VEH_SLOT = 11;
    static final int DONATE_OTHER_VEH_FAMILY_SLOT = 12;
    static final int DONATE_OTHER_GIVE_FAMILY_SLOT = 13;
    static final int DONATE_OTHER_ROLLERS = 14;
    static final String CONTACTS_VIEW_ACTION = "android.intent.action.VIEW";

    static final int BUTTTON_BUY = 0;
    static final int BUTTTON_INFO = 1;
    native void sendClickItem(int actionType, int buttonId, int itemType, int itemId);

    public Donate(Activity activity, int balance)
    {
        this.activity = activity;

        activity.runOnUiThread(() ->
        {
            activityService = new ActivityServiceImpl();
            registerAllItems();
            initBuyLcDialog();

            donate_change_butt = activity.findViewById(R.id.donate_change_butt);
            donate_change_butt.setOnClickListener(view -> {
                sendAction(ACTION_TYPE_CLICK_CONVERT);
            });

            donate_history_butt = activity.findViewById(R.id.donate_history_butt);
            donate_history_butt.setOnClickListener(view -> {
                sendAction(ACTION_TYPE_CLICK_HISTORY);
            });

            donate_check_butt = activity.findViewById(R.id.donate_check_butt);
            donate_check_butt.setOnClickListener(view -> {
                sendAction(ACTION_TYPE_CLICK_CHECK);
            });

            // Сортировка
            String[] sort = {"Сначала новые", "Сначала дорогие", "Сначала дешевые"};
            Spinner spinner = activity.findViewById(R.id.donate_sort);

            // Создаем адаптер ArrayAdapter с помощью массива строк и стандартной разметки элемета spinner
            ArrayAdapter<String> adapter = new ArrayAdapter(activity, android.R.layout.simple_spinner_item, sort);

            // Определяем разметку для использования при выборе элемента
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

            // Применяем адаптер к элементу spinner
            spinner.setAdapter(adapter);
            spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                 //   Log.d("safd", "item = " + i);
                    if( i == 2 )
                    {
                        //donateAdapter.items.sort( new DonateComparators.DonateComparatorByHighPrice());
                        Collections.sort(donateAdapter.items, new DonateComparators.DonateComparatorByPrice());
                    }
                    if( i == 1 )
                    {
                        Collections.sort(donateAdapter.items, new DonateComparators.DonateComparatorByPrice().reversed());
                    }
                    if( i == 0)
                    {
                        setActiveCategory(active_category_type, active_caterory_view);
                    }
                    donateAdapter.notifyDataSetChanged();
                }

                @Override
                public void onNothingSelected(AdapterView<?> adapterView) {

                }
            });

            // категории
            donate_category_buy_all = activity.findViewById(R.id.donate_category_buy_all);
            donate_category_buy_all.setOnClickListener(view -> {
                donateAdapter = new DonateAdapter(activity, allItems, this);
                donate_recycle.setAdapter(donateAdapter);
                setActiveCategory(CATEGORY_ALL, view);
            });

            donate_category_my_item = activity.findViewById(R.id.donate_category_my_item);
            donate_category_my_item.setOnClickListener(view -> {
                // donate_recycle.setAdapter( new DonateAdapter(activity, allItems) );
                setActiveCategory(-1, view);
            });

            donate_category_buy_cars = activity.findViewById(R.id.donate_category_buy_cars);
            donate_category_buy_cars.setOnClickListener(view -> {
                setActiveCategory(CATEGORY_CARS, view);
            });

            donate_category_buy_skins = activity.findViewById(R.id.donate_category_buy_skins);
            donate_category_buy_skins.setOnClickListener(view -> {
                setActiveCategory(CATEGORY_SKINS, view);
            });

            donate_category_buy_acs = activity.findViewById(R.id.donate_category_buy_acs);
            donate_category_buy_acs.setOnClickListener(view -> {
                setActiveCategory(CATEGORY_ACS, view);
            });

            donate_category_buy_vip = activity.findViewById(R.id.donate_category_buy_vip);
            donate_category_buy_vip.setOnClickListener(view -> {
                setActiveCategory(CATEGORY_VIP, view);
            });

            donate_category_buy_other = activity.findViewById(R.id.donate_category_buy_other);
            donate_category_buy_other.setOnClickListener(view -> {
                setActiveCategory(CATEGORY_OTHER, view);
            });

            donate_balance_text = activity.findViewById(R.id.donate_balance_text);
            donate_balance_text.setText( String.format("Баланс: %s LC", Samp.formatter.format(balance)) );

            donate_exit_button = activity.findViewById(R.id.donate_exit_button);
            donate_exit_button.setOnClickListener(view -> {
                hide();
            });

            //
            donate_recycle = activity.findViewById(R.id.donate_recycle);
            donate_recycle.setLayoutManager(new GridLayoutManager(activity, 3));
            setActiveCategory(CATEGORY_CARS, donate_category_buy_cars);
          //  donateAdapter = new DonateAdapter(activity, allItems, this);
           // donate_recycle.setAdapter(donateAdapter);

            donate_main_layout = activity.findViewById(R.id.donate_main_layout);
            donate_main_layout.setVisibility(View.VISIBLE);
        });
    }

    private void initBuyLcDialog() {

        donate_buy_lc_layout = activity.findViewById(R.id.donate_buy_lc_layout);
        donate_buy_lc_layout.setVisibility(View.GONE);

        donate_deposit_butt = activity.findViewById(R.id.donate_deposit_butt);
        donate_deposit_butt.setOnClickListener(view -> {
            donate_buy_lc_layout.setVisibility(View.VISIBLE);
        });

        donate_buy_lc_buy_btn = activity.findViewById(R.id.donate_buy_lc_buy_btn);
        donate_buy_lc_buy_btn.setOnClickListener(view -> {
            doBuyBtnAction();
        });

        donate_buy_lc_buy_via_qiwi_btn = activity.findViewById(R.id.donate_buy_lc_buy_via_qiwi_btn);
        donate_buy_lc_buy_via_qiwi_btn.setOnClickListener(view -> {
            doQiwiBuyBtnAction();
        });

        donate_byu_lc_cancel_btn = activity.findViewById(R.id.donate_byu_lc_cancel_btn);
        donate_byu_lc_cancel_btn.setOnClickListener(view -> {
            donate_buy_lc_layout.setVisibility(View.GONE);
        });

        donate_buy_lc_email_input = activity.findViewById(R.id.donate_buy_lc_email_input);

        donate_buy_lc_sum_input = activity.findViewById(R.id.donate_buy_lc_sum_input);

        iAmNotRobotCheckBox = activity.findViewById(R.id.donate_buy_lc_i_am_not_robot_checkbox);
        iAmNotRobotCheckBox.setOnClickListener(view -> {
            try {
                performIAmNotRobotCheckBoxAction();
            } catch (Exception ignored) {

            }
        });
    }

    private void doQiwiBuyBtnAction() {
        if (!isValidBuyDialogInputs()) {
            return;
        }

        activity.startActivity(new Intent(CONTACTS_VIEW_ACTION, Uri.parse(createUri())));
    }

    private String createUri() {
        String nickname = NativeStorage.getClientProperty(NativeStorageElements.NICKNAME, activity);
        String email = donate_buy_lc_email_input.getText().toString();
        String serverId = NativeStorage.getClientProperty(NativeStorageElements.SERVER, activity);
        String serverName = Storage.getProperty(StorageElements.SERVER_NAME, activity);
        String sum = donate_buy_lc_sum_input.getText().toString();
        String captcha = captchaToken;

        return Config.createBillingUri(serverId, serverName, sum, nickname, email, captcha);
    }

    private void performIAmNotRobotCheckBoxAction() {
        if (iAmNotRobotCheckBox.isChecked()) {
            iAmNotRobotCheckBox.setChecked(false);
            ReCaptchaDialog reCaptchaDialog = new ReCaptchaDialog(activity);
            reCaptchaDialog.setOnDialogCloseListener(this::performCaptchaSuccessAction);
            reCaptchaDialog.createDialog();
        } else {
            activityService.showErrorMessage(ErrorMessage.CAPTCHA_NOT_PASSED.getText(), activity);
        }
    }

    public void performCaptchaSuccessAction(String token) {
        captchaToken = token;
        iAmNotRobotCheckBox.setChecked(true);
    }

    //todo мб сделать когда разберусь почему киви не рисуется в webview
    private void doBuyBtnAction() {
        if (!isValidBuyDialogInputs()) {
            return;
        }


    }

    private boolean isValidBuyDialogInputs() {
        return Validator.isValidEmail(donate_buy_lc_email_input.getText().toString(), activity)
                && Validator.isValidDonateSum(donate_buy_lc_sum_input.getText().toString(), activity)
                && Validator.isValidCaptchaToken(captchaToken, activity);
    }

    void updateBalance(int balance)
    {
        activity.runOnUiThread(()-> {
            donate_balance_text.setText( String.format("Баланс: %s LC", Samp.formatter.format(balance)) );
        });
    }

    void hide()
    {
        activity.runOnUiThread(()-> {
            donate_main_layout.setVisibility(View.GONE);
        });
        native_hide();
    }

    int getResId(String res)
    {
        return activity.getResources().getIdentifier(res, "drawable", activity.getPackageName());
    }

    void setActiveCategory(int category, View view)
    {
        active_category_type = category;
        active_caterory_view = view;

        resetAllCategoryTint();
        view.setBackgroundTintList( ColorStateList.valueOf(Color.parseColor("#ffff00")) );

        List<DonateItem> tempList = new ArrayList<>();
        for (int i = 0; i < allItems.size(); i++)
        {
            if( allItems.get(i).category == category || category == 0 )
                tempList.add( allItems.get(i) );
        }
        donateAdapter = new DonateAdapter(activity, tempList, this);
        donate_recycle.setAdapter( donateAdapter );
    }

    void resetAllCategoryTint()
    {
        donate_category_buy_other.setBackgroundTintList(null);
        donate_category_buy_vip.setBackgroundTintList(null);
        donate_category_my_item.setBackgroundTintList(null);
        donate_category_buy_all.setBackgroundTintList(null);
        donate_category_buy_cars.setBackgroundTintList(null);
        donate_category_buy_skins.setBackgroundTintList(null);
        donate_category_buy_acs.setBackgroundTintList(null);
    }

    void registerAllItems()
    {
        // ДОБАВЛЯТЬ В НАЧАЛО КАТЕГОРИИ ЧТОБЫ ВОРКАЛА СОРТИРОВКА ПО НОВИЗНЕ!!!!!!!!!111111111111111111

        // skins
        allItems.add( new DonateItem("Флеш", CATEGORY_SKINS, 10000, getResId("skin_186"), 186) );
        allItems.add( new DonateItem("Сорвиголова", CATEGORY_SKINS, 7500, getResId("skin_206"), 206) );
        allItems.add( new DonateItem("Детстроук", CATEGORY_SKINS, 15000, getResId("skin_221"), 221) );
        allItems.add( new DonateItem("Мияги", CATEGORY_SKINS, 2500, getResId("skin_299"), 299) );
        allItems.add( new DonateItem("Пивозавр", CATEGORY_SKINS, 500, getResId("skin_138"), 138) );
        allItems.add( new DonateItem("Каха", CATEGORY_SKINS, 1500, getResId("skin_158"), 158) );
        allItems.add( new DonateItem("Джейсон", CATEGORY_SKINS, 1500, getResId("skin_95"), 95) );
        allItems.add( new DonateItem("Серго", CATEGORY_SKINS, 1500, getResId("skin_170"), 170) );
        allItems.add( new DonateItem("Доминик Торетто", CATEGORY_SKINS, 1500, getResId("skin_168"), 168) );
        allItems.add( new DonateItem("Ведьма", CATEGORY_SKINS, 1500, getResId("skin_77"), 77) );
        allItems.add( new DonateItem("Иван Золо", CATEGORY_SKINS, 2500, getResId("skin_114"), 114) );
        allItems.add( new DonateItem("Милохин", CATEGORY_SKINS, 2500, getResId("skin_115"), 115) );
        allItems.add( new DonateItem("Давидыч", CATEGORY_SKINS, 2500, getResId("skin_119"), 119) );
        allItems.add( new DonateItem("Дед Мороз", CATEGORY_SKINS, 2500, getResId("skin_121"), 121) );
        allItems.add( new DonateItem("Хабиб", CATEGORY_SKINS, 2500, getResId("skin_1"), 1) );
        allItems.add( new DonateItem("FACE", CATEGORY_SKINS, 2500, getResId("skin_154"), 154) );
        allItems.add( new DonateItem("Снегурочка", CATEGORY_SKINS, 2500, getResId("skin_40"), 40) );
        allItems.add( new DonateItem("Литвин", CATEGORY_SKINS, 2500, getResId("skin_159"), 159) );
        allItems.add( new DonateItem("Крик", CATEGORY_SKINS, 2500, getResId("skin_94"), 94) );
        allItems.add( new DonateItem("Харли", CATEGORY_SKINS, 2500, getResId("skin_87"), 87) );
        allItems.add( new DonateItem("Крюгер", CATEGORY_SKINS, 2500, getResId("skin_227"), 227) );
        allItems.add( new DonateItem("Пабло", CATEGORY_SKINS, 2500, getResId("skin_228"), 228) );
        allItems.add( new DonateItem("Агент 007", CATEGORY_SKINS, 2500, getResId("skin_267"), 267) );
        allItems.add( new DonateItem("Моргенштерн", CATEGORY_SKINS, 2500, getResId("skin_294"), 294) );
        allItems.add( new DonateItem("Кизару", CATEGORY_SKINS, 2500, getResId("skin_296"), 296) );
        allItems.add( new DonateItem("Аниме тян", CATEGORY_SKINS, 3000, getResId("skin_139"), 139) );
        allItems.add( new DonateItem("Пеннивайз", CATEGORY_SKINS, 3600, getResId("skin_264"), 264) );
        allItems.add( new DonateItem("Квин", CATEGORY_SKINS, 3600, getResId("skin_226"), 226) );
        allItems.add( new DonateItem("Палпатин", CATEGORY_SKINS, 3600, getResId("skin_137"), 137) );
        allItems.add( new DonateItem("Бустер", CATEGORY_SKINS, 5000, getResId("skin_171"), 171) );
        allItems.add( new DonateItem("Оксимирон", CATEGORY_SKINS, 5000, getResId("skin_297"), 297) );
        allItems.add( new DonateItem("Димитреску", CATEGORY_SKINS, 6000, getResId("skin_298"), 298) );
        allItems.add( new DonateItem("Человек Паук", CATEGORY_SKINS, 20000, getResId("skin_272"), 272) );
        allItems.add( new DonateItem("Шрек", CATEGORY_SKINS, 20000, getResId("skin_97"), 97) );
        allItems.add( new DonateItem("Админ", CATEGORY_SKINS, 30000, getResId("skin_184"), 184) );
        allItems.add( new DonateItem("Скелетон", CATEGORY_SKINS, 10000, getResId("skin_273"), 273) );
        allItems.add( new DonateItem("Северус", CATEGORY_SKINS, 1500, getResId("skin_220"), 220) );
        allItems.add( new DonateItem("Бодров", CATEGORY_SKINS, 2500, getResId("skin_208"), 208) );
        allItems.add( new DonateItem("Дафна", CATEGORY_SKINS, 2000, getResId("skin_238"), 238) );
        allItems.add( new DonateItem("Месси", CATEGORY_SKINS, 2500, getResId("skin_161"), 161) );
        allItems.add( new DonateItem("Kaneki Aogiri", CATEGORY_SKINS, 7500, getResId("skin_147"), 147) );
        allItems.add( new DonateItem("Seidou Takizawa", CATEGORY_SKINS, 7500, getResId("skin_140"), 140) );
        allItems.add( new DonateItem("Грут", CATEGORY_SKINS, 50000, getResId("skin_14"), 14) );

        // cars
        allItems.add( new DonateItem("BMW M5 E39", CATEGORY_CARS, 750, getResId("auc_veh_496"), 496) );
        allItems.add( new DonateItem("Dodge Demon SRT", CATEGORY_CARS, 1750, getResId("auc_veh_503"), 503) );
        allItems.add( new DonateItem("Cadillac Escadale", CATEGORY_CARS, 5000, getResId("auc_veh_504"), 504) );
        allItems.add( new DonateItem("Porche 911 Carrera S", CATEGORY_CARS, 5100, getResId("auc_veh_506"), 506) );
        allItems.add( new DonateItem("Volvo 242 DL", CATEGORY_CARS, 120, getResId("auc_veh_536"), 536) );
        allItems.add( new DonateItem("Alfa Romeo Guilia", CATEGORY_CARS, 900, getResId("auc_veh_550"), 550) );
        allItems.add( new DonateItem("Mercedes-Benz a65 AMG", CATEGORY_CARS, 1325, getResId("auc_veh_565"), 565) );
        allItems.add( new DonateItem("Tesla Cubertruck", CATEGORY_CARS, 3500, getResId("auc_veh_576"), 576) );
        allItems.add( new DonateItem("VAZ 2115", CATEGORY_CARS, 60, getResId("auc_veh_585"), 585) );
        allItems.add( new DonateItem("Ford Mustang GT", CATEGORY_CARS, 3640, getResId("auc_veh_600"), 600) );
        allItems.add( new DonateItem("Mercedes Benz GLE 350", CATEGORY_CARS, 9000, getResId("auc_veh_466"), 466) );
        allItems.add( new DonateItem("Ducati SuperSport S", CATEGORY_CARS, 750, getResId("auc_veh_461"), 461) );
        allItems.add( new DonateItem("Yamaha FZ-10", CATEGORY_CARS, 1000, getResId("auc_veh_581"), 581) );
        allItems.add( new DonateItem("BMW S 1000 RR", CATEGORY_CARS, 1250, getResId("auc_veh_521"), 521) );
        allItems.add( new DonateItem("Ducati XDiavel S", CATEGORY_CARS, 1750, getResId("auc_veh_463"), 463) );
        allItems.add( new DonateItem("Aprilia-sxv", CATEGORY_CARS, 500, getResId("auc_veh_468"), 468) );
        allItems.add( new DonateItem("Bugatti Veyron", CATEGORY_CARS, 55000, getResId("auc_veh_411"), 411) );
        allItems.add( new DonateItem("Lamborghini Urus", CATEGORY_CARS, 8250, getResId("auc_veh_579"), 579) );
        allItems.add( new DonateItem("Lamborghini Aventador", CATEGORY_CARS, 8500, getResId("auc_veh_451"), 451) );
        allItems.add( new DonateItem("Rolls Royce Wraith", CATEGORY_CARS, 12500, getResId("auc_veh_477"), 477) );
        allItems.add( new DonateItem("Mercedes-Benz G63-6x6", CATEGORY_CARS, 12500, getResId("auc_veh_489"), 489) );
        allItems.add( new DonateItem("VAZ 1111", CATEGORY_CARS, 12, getResId("auc_veh_589"), 589) );
        allItems.add( new DonateItem("LADA 2107", CATEGORY_CARS, 47, getResId("auc_veh_517"), 517) );
        allItems.add( new DonateItem("NIVA 2121", CATEGORY_CARS, 65, getResId("auc_veh_474"), 474) );
        allItems.add( new DonateItem("VAZ 2109", CATEGORY_CARS, 87, getResId("auc_veh_420"), 420) );
        allItems.add( new DonateItem("LADA 2114", CATEGORY_CARS, 93, getResId("auc_veh_438"), 438) );
        allItems.add( new DonateItem("Mercedes-Benz E230", CATEGORY_CARS, 105, getResId("auc_veh_475"), 475) );
        allItems.add( new DonateItem("Subaru Legacy", CATEGORY_CARS, 125, getResId("auc_veh_526"), 526) );
        allItems.add( new DonateItem("Lada Vesta", CATEGORY_CARS, 170, getResId("auc_veh_527"), 527) );
        allItems.add( new DonateItem("BMW 3 Series E30", CATEGORY_CARS, 245, getResId("auc_veh_401"), 401) );
        allItems.add( new DonateItem("BMW 5 Series E34", CATEGORY_CARS, 265, getResId("auc_veh_421"), 421) );
        allItems.add( new DonateItem("Mercedes-Benz E420", CATEGORY_CARS, 275, getResId("auc_veh_604"), 604) );
        allItems.add( new DonateItem("Nissan Silvia S14", CATEGORY_CARS, 300, getResId("auc_veh_494"), 494) );
        allItems.add( new DonateItem("BMW E39", CATEGORY_CARS, 330, getResId("auc_veh_412"), 412) );
        allItems.add( new DonateItem("Toyota Mark 2", CATEGORY_CARS, 365, getResId("auc_veh_562"), 562) );
        allItems.add( new DonateItem("Skoda Octavia", CATEGORY_CARS, 395, getResId("auc_veh_479"), 479) );
        allItems.add( new DonateItem("Opel Insignia", CATEGORY_CARS, 440, getResId("auc_veh_492"), 492) );
        allItems.add( new DonateItem("Nissan Sentra", CATEGORY_CARS, 450, getResId("auc_veh_400"), 400) );
        allItems.add( new DonateItem("Volkswagen Scirocco", CATEGORY_CARS, 480, getResId("auc_veh_549"), 549) );
        allItems.add( new DonateItem("Mitsubishi Lancer IX", CATEGORY_CARS, 570, getResId("auc_veh_404"), 404) );
        allItems.add( new DonateItem("Honda Civic Type R", CATEGORY_CARS, 612, getResId("auc_veh_402"), 402) );
        allItems.add( new DonateItem("Volkswagen Golf", CATEGORY_CARS, 635, getResId("auc_veh_436"), 436) );
        allItems.add( new DonateItem("BMW E60", CATEGORY_CARS, 675, getResId("auc_veh_405"), 405) );
        allItems.add( new DonateItem("Subaru Impreza", CATEGORY_CARS, 750, getResId("auc_veh_560"), 560) );
        allItems.add( new DonateItem("Audi A6", CATEGORY_CARS, 800, getResId("auc_veh_507"), 507) );
        allItems.add( new DonateItem("Mercedes Benz s600", CATEGORY_CARS, 870, getResId("auc_veh_426"), 426) );
        allItems.add( new DonateItem("Subaru Impreza WRX STI", CATEGORY_CARS, 950, getResId("auc_veh_502"), 502) );
        allItems.add( new DonateItem("BMW X5M E70", CATEGORY_CARS, 1250, getResId("auc_veh_458"), 458) );
        allItems.add( new DonateItem("Toyota Camry 3.5", CATEGORY_CARS, 1650, getResId("auc_veh_542"), 542) );
        allItems.add( new DonateItem("Lexus LFA", CATEGORY_CARS, 1825, getResId("auc_veh_480"), 480) );
        allItems.add( new DonateItem("Volvo XC90", CATEGORY_CARS, 2100, getResId("auc_veh_470"), 470) );
        allItems.add( new DonateItem("Mercedes Benz C63 AMG", CATEGORY_CARS, 2100, getResId("auc_veh_566"), 566) );
        allItems.add( new DonateItem("Jeep Grand Cherokee", CATEGORY_CARS, 2250, getResId("auc_veh_505"), 505) );
        allItems.add( new DonateItem("Nissan GT-R", CATEGORY_CARS, 2550, getResId("auc_veh_410"), 410) );
        allItems.add( new DonateItem("Audi RS6", CATEGORY_CARS, 2850, getResId("auc_veh_559"), 559) );
        allItems.add( new DonateItem("Range Rover Sport", CATEGORY_CARS, 3100, getResId("auc_veh_534"), 534) );
        allItems.add( new DonateItem("Bentley Continental Gt", CATEGORY_CARS, 3750, getResId("auc_veh_419"), 419) );
        allItems.add( new DonateItem("Audi R8", CATEGORY_CARS, 4100, getResId("auc_veh_587"), 587) );
        allItems.add( new DonateItem("Porsche Panamera S", CATEGORY_CARS, 4200, getResId("auc_veh_429"), 429) );
        allItems.add( new DonateItem("Mercedes Maybach S650", CATEGORY_CARS, 4500, getResId("auc_veh_491"), 491) );
        allItems.add( new DonateItem("Mercedes-Benz g63", CATEGORY_CARS, 5000, getResId("auc_veh_543"), 543) );
        allItems.add( new DonateItem("Mercedes-Benz GT63s", CATEGORY_CARS, 5250, getResId("auc_veh_546"), 546) );
        allItems.add( new DonateItem("BMW M5", CATEGORY_CARS, 5725, getResId("auc_veh_551"), 551) );
        allItems.add( new DonateItem("Audi RS7", CATEGORY_CARS, 5850, getResId("auc_veh_547"), 547) );
        allItems.add( new DonateItem("Lamborghini Huracan", CATEGORY_CARS, 7000, getResId("auc_veh_415"), 415) );
        allItems.add( new DonateItem("Ferrari La Ferrari", CATEGORY_CARS, 7500, getResId("auc_veh_541"), 541) );
        allItems.add( new DonateItem("BMW M8", CATEGORY_CARS, 8250, getResId("auc_veh_603"), 603) );
        allItems.add( new DonateItem("Квадроцикл", CATEGORY_CARS, 370, getResId("auc_veh_424"), 424) );
        allItems.add( new DonateItem("Путник", CATEGORY_CARS, 750, getResId("auc_veh_586"), 586) );
        allItems.add( new DonateItem("Каваски", CATEGORY_CARS, 2350, getResId("auc_veh_522"), 522) );
        allItems.add( new DonateItem("Снегоход", CATEGORY_CARS, 2500, getResId("auc_veh_471"), 471) );
        allItems.add( new DonateItem("Bandito", CATEGORY_CARS, 2500, getResId("auc_veh_568"), 568) );
        allItems.add( new DonateItem("Картер", CATEGORY_CARS, 2500, getResId("auc_veh_571"), 571) );
        allItems.add( new DonateItem("Monster", CATEGORY_CARS, 10000, getResId("auc_veh_444"), 444) );
        allItems.add( new DonateItem("УАЗ - Патриот", CATEGORY_CARS, 500, getResId("auc_veh_540"), 540) );
        allItems.add( new DonateItem("LADA 2101", CATEGORY_CARS, 50, getResId("auc_veh_518"), 518) );
        allItems.add( new DonateItem("LADA Priora", CATEGORY_CARS, 300, getResId("auc_veh_467"), 467) );
        allItems.add( new DonateItem("Rolls-Royce Cullinan", CATEGORY_CARS, 12500, getResId("auc_veh_558"), 558) );


        //vip
        allItems.add( new DonateItem("Silver VIP\n( 7 дней )", CATEGORY_VIP, 100, getResId("silver_vip"), 0) );
        allItems.add( new DonateItem("Silver VIP\n( 15 дней )", CATEGORY_VIP, 170, getResId("silver_vip"), 1) );
        allItems.add( new DonateItem("Silver VIP\n( 30 дней )", CATEGORY_VIP, 300, getResId("silver_vip"), 2) );
        allItems.add( new DonateItem("Gold VIP\n( 7 дней )", CATEGORY_VIP, 215, getResId("gold_vip"), 3) );
        allItems.add( new DonateItem("Gold VIP\n( 15 дней )", CATEGORY_VIP, 370, getResId("gold_vip"), 4) );
        allItems.add( new DonateItem("Gold VIP\n( 30 дней )", CATEGORY_VIP, 650, getResId("gold_vip"), 5) );
        allItems.add( new DonateItem("Platinum VIP\n( 7 дней )", CATEGORY_VIP, 335, getResId("platinum_vip"), 6) );
        allItems.add( new DonateItem("Platinum VIP\n( 15 дней )", CATEGORY_VIP, 570, getResId("platinum_vip"), 7) );
        allItems.add( new DonateItem("Platinum VIP\n( 30 дней )", CATEGORY_VIP, 1000, getResId("platinum_vip"), 8) );

        //acs
        allItems.add( new DonateItem("Ворон", CATEGORY_ACS, 500, getResId("inv_voron"), 5767) );
        allItems.add( new DonateItem("Тыква", CATEGORY_ACS, 500, getResId("inv_tikva"), 5770) );
        allItems.add( new DonateItem("Мишка", CATEGORY_ACS, 500, getResId("inv_mishka"), 5811) );
        allItems.add( new DonateItem("Пёсель", CATEGORY_ACS, 500, getResId("inv_pesel"), 5916) );
        allItems.add( new DonateItem("Пакет", CATEGORY_ACS, 500, getResId("inv_packet"), 5873) );
        allItems.add( new DonateItem("Гуманоид", CATEGORY_ACS, 700, getResId("inv_nlo"), 5874) );
        allItems.add( new DonateItem("Скейт", CATEGORY_ACS, 1000, getResId("inv_skeyt"), 14283) );
        allItems.add( new DonateItem("Посох", CATEGORY_ACS, 1500, getResId("inv_posox"), 14393) );
        allItems.add( new DonateItem("Кирка", CATEGORY_ACS, 1500, getResId("inv_pickaxe"), 14394) );
        allItems.add( new DonateItem("Молот Тора", CATEGORY_ACS, 2500, getResId("inv_tor"), 14289) );
        allItems.add( new DonateItem("Крылья", CATEGORY_ACS, 3000, getResId("inv_wings"), 17979) );

        // other
        allItems.add( new DonateItem("Ролики ( анимация ходьбы )", CATEGORY_OTHER, 2000, getResId("inv_rollers"), DONATE_OTHER_ROLLERS) );
        allItems.add( new DonateItem("Слот для дома\n(От 1000)", CATEGORY_OTHER, 1000, getResId("house_slot"), DONATE_OTHER_HOUSE_SLOT) );
        allItems.add( new DonateItem("Слот для Бизнеса", CATEGORY_OTHER, 5000, getResId("biz_slot"), DONATE_OTHER_BIZ_SLOT) );
        allItems.add( new DonateItem("Снять варн", CATEGORY_OTHER, 100, getResId("donate_warn"), DONATE_OTHER_WARN) );
        allItems.add( new DonateItem("Все лицензии", CATEGORY_OTHER, 150, getResId("licenses_pack"), DONATE_OTHER_LICENSES) );
        allItems.add( new DonateItem("Изменить имя семьи", CATEGORY_OTHER, 100, getResId("donate_change_family_name"), DONATE_OTHER_CHANGE_FAM_NAME) );
        allItems.add( new DonateItem("4-х значный номер", CATEGORY_OTHER, 70, getResId("donate_sim"), DONATE_OTHER_CHANGE_SIM) );
        allItems.add( new DonateItem("Военный билет", CATEGORY_OTHER, 100, getResId("donate_voen"), DONATE_OTHER_VOEN) );
        allItems.add( new DonateItem("Законопослушность\n( + 10 )", CATEGORY_OTHER, 5, getResId("donate_zakon"), DONATE_OTHER_ZAKON) );
        allItems.add( new DonateItem("Мед. карта", CATEGORY_OTHER, 100, getResId("donate_medcard"), DONATE_OTHER_MED) );
        allItems.add( new DonateItem("Смена ника", CATEGORY_OTHER, 50, getResId("donate_changenick"), DONATE_OTHER_CHANGE_NICK) );
        allItems.add( new DonateItem("Смена пола", CATEGORY_OTHER, 100, getResId("donate_change_sex"), DONATE_OTHER_CHANGE_SEX) );
        allItems.add( new DonateItem("Слот для авто", CATEGORY_OTHER, 100, getResId("donate_veh_slot"), DONATE_OTHER_VEH_SLOT) );
        allItems.add( new DonateItem("Слот для семейного авто", CATEGORY_OTHER, 200, getResId("donate_family_car_slot"), DONATE_OTHER_VEH_FAMILY_SLOT) );
        allItems.add( new DonateItem("Передача семьи", CATEGORY_OTHER, 1000, getResId("donate_give_family"), DONATE_OTHER_GIVE_FAMILY_SLOT) );
    }

    public void clickInfo(DonateItem item) {
        sendClickItem(ACTION_TYPE_CLICK_ITEM, BUTTTON_INFO, item.category, item.value);
    }

    public void clickBuy(DonateItem item) {
        sendClickItem(ACTION_TYPE_CLICK_ITEM, BUTTTON_BUY, item.category, item.value);
    }
}
