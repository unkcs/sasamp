package com.liverussia.cr.gui.tire_shop;

import com.liverussia.cr.R;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

@Getter
@RequiredArgsConstructor
public enum ItemInfo {

    SUSPENSION("2", "-1", R.drawable.tire_shop_suspension_button, ItemType.MAIN_MENU_ITEM),
    PREFIXES("3", "-1", R.drawable.tire_shop_prefixes_button, ItemType.MAIN_MENU_ITEM),
    WHEEL_WIDTH("4", "-1", R.drawable.tire_shop_wheel_width_button, ItemType.MAIN_MENU_ITEM),
    WHEEL_DIAMETER("5", "-1", R.drawable.tire_shop_wheel_diameter_button, ItemType.MAIN_MENU_ITEM),
    DISK_TYPE("6", "-1", R.drawable.tire_shop_disk_type_button, ItemType.MAIN_MENU_ITEM),

    DISK_1("6", "1", R.drawable.tire_shop_disk_1, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_2("6", "2", R.drawable.tire_shop_disk_2, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_3("6", "3", R.drawable.tire_shop_disk_3, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_4("6", "4", R.drawable.tire_shop_disk_4, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_5("6", "5", R.drawable.tire_shop_disk_5, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_6("6", "6", R.drawable.tire_shop_disk_6, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_7("6", "7", R.drawable.tire_shop_disk_7, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_8("6", "8", R.drawable.tire_shop_disk_8, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_9("6", "9", R.drawable.tire_shop_disk_9, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_10("6", "10", R.drawable.tire_shop_disk_10, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_11("6", "11", R.drawable.tire_shop_disk_11, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_12("6", "12", R.drawable.tire_shop_disk_12, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_13("6", "13", R.drawable.tire_shop_disk_13, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_14("6", "14", R.drawable.tire_shop_disk_14, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_15("6", "15", R.drawable.tire_shop_disk_15, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_16("6", "16", R.drawable.tire_shop_disk_16, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_17("6", "17", R.drawable.tire_shop_disk_17, ItemType.CONCRETE_ITEM_IN_CATEGORY);

    private static final String DISK_TYPE_VALUE = "6";
    private static final String MAIN_MENU_ITEM_ID = "-1";

    private final String typeId;
    private final String itemId;
    private final int resource;
    private final ItemType type;

    public static List<ItemInfo> getDisks() {
        return Arrays.stream(values())
                .filter(item -> !MAIN_MENU_ITEM_ID.equals(item.getItemId()))
                .filter(item -> DISK_TYPE_VALUE.equals(item.getTypeId()))
                .collect(Collectors.toList());

    }

    public static List<ItemInfo> getMainMenuItems() {
        return Arrays.stream(values())
                .filter(item -> MAIN_MENU_ITEM_ID.equals(item.getItemId()))
                .collect(Collectors.toList());

    }
}
