package com.liverussia.cr.gui.tire_shop;

import com.liverussia.cr.R;

import org.apache.commons.lang3.StringUtils;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

@Getter
@RequiredArgsConstructor
public enum ItemInfo {

    SUSPENSION(2, -1, -1, R.drawable.tire_shop_suspension_button, R.drawable.tire_shop_suspension_button, ItemType.MAIN_MENU_ITEM),
    PREFIXES(3, -1, -1, R.drawable.tire_shop_prefixes_button, R.drawable.tire_shop_prefixes_button, ItemType.MAIN_MENU_ITEM),
    WHEEL_WIDTH(4, -1, -1, R.drawable.tire_shop_wheel_width_button, R.drawable.tire_shop_wheel_width_button, ItemType.MAIN_MENU_ITEM),
    WHEEL_DIAMETER(5, -1, -1, R.drawable.tire_shop_wheel_diameter_button, R.drawable.tire_shop_wheel_diameter_button, ItemType.MAIN_MENU_ITEM),
    DISK_TYPE(6, -1, -1, R.drawable.tire_shop_disk_type_button, R.drawable.tire_shop_disk_type_button, ItemType.MAIN_MENU_ITEM),

    DISK_1(6, 1, 1082, R.drawable.tire_shop_disk_1, R.drawable.tire_shop_disk_1_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_2(6, 2, 1085, R.drawable.tire_shop_disk_2, R.drawable.tire_shop_disk_2_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_3(6, 3, 1096, R.drawable.tire_shop_disk_3, R.drawable.tire_shop_disk_3_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_4(6, 4, 1097, R.drawable.tire_shop_disk_4, R.drawable.tire_shop_disk_4_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_5(6, 5, 1098, R.drawable.tire_shop_disk_5, R.drawable.tire_shop_disk_5_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_6(6, 6, 1080, R.drawable.tire_shop_disk_6, R.drawable.tire_shop_disk_6_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_7(6, 7, 1077, R.drawable.tire_shop_disk_7, R.drawable.tire_shop_disk_7_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_8(6, 8, 1083, R.drawable.tire_shop_disk_8, R.drawable.tire_shop_disk_8_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_9(6, 9, 1078, R.drawable.tire_shop_disk_9, R.drawable.tire_shop_disk_9_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_10(6, 10, 1076, R.drawable.tire_shop_disk_10, R.drawable.tire_shop_disk_10_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_11(6, 11, 1084, R.drawable.tire_shop_disk_11, R.drawable.tire_shop_disk_11_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_12(6, 12, 1073, R.drawable.tire_shop_disk_12, R.drawable.tire_shop_disk_12_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_13(6, 13, 1025, R.drawable.tire_shop_disk_13, R.drawable.tire_shop_disk_13_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_14(6, 14, 1079, R.drawable.tire_shop_disk_14, R.drawable.tire_shop_disk_14_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_15(6, 15, 1075, R.drawable.tire_shop_disk_15, R.drawable.tire_shop_disk_15_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_16(6, 16, 1074, R.drawable.tire_shop_disk_16, R.drawable.tire_shop_disk_16_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY),
    DISK_17(6, 17, 1081, R.drawable.tire_shop_disk_17, R.drawable.tire_shop_disk_17_selected, ItemType.CONCRETE_ITEM_IN_CATEGORY);

    private static final int DISK_TYPE_VALUE = 6;
    private static final int MAIN_MENU_ITEM_ID = -1;

    private final int typeId;
    private final int itemId;
    private final int clientItemId;
    private final int resource;
    private final int resourceSelected;
    private final ItemType type;

    public static List<ItemInfo> getDisks() {
        return Arrays.stream(values())
                .filter(item -> MAIN_MENU_ITEM_ID != item.getItemId())
                .filter(item -> DISK_TYPE_VALUE == item.getTypeId())
                .collect(Collectors.toList());

    }

    public static List<ItemInfo> getMainMenuItems() {
        return Arrays.stream(values())
                .filter(item -> MAIN_MENU_ITEM_ID == item.getItemId())
                .collect(Collectors.toList());

    }
}
