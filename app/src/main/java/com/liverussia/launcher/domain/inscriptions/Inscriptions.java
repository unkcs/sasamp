package com.liverussia.launcher.domain.inscriptions;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

@Getter
@RequiredArgsConstructor
public enum Inscriptions {
    BALANCE("Баланс: %s LC"),
    DUELS_TOP_1("1. %s"),
    DUELS_TOP_2("2. %s"),
    DUELS_TOP_3("3. %s");

    private final String text;

    public static String createBalanceInscription(String balance) {
        return String.format(
                BALANCE.getText(),
                balance
        );
    }

    public static String createDuelsTop1Inscription(String nickname) {
        return String.format(
                DUELS_TOP_1.getText(),
                nickname
        );
    }

    public static String createDuelsTop2Inscription(String nickname) {
        return String.format(
                DUELS_TOP_2.getText(),
                nickname
        );
    }

    public static String createDuelsTop3Inscription(String nickname) {
        return String.format(
                DUELS_TOP_3.getText(),
                nickname
        );
    }
}
