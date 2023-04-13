package com.liverussia.launcher.service;

import android.app.Activity;
import android.content.Context;

public interface ActivityService {

    void showErrorMessage(String message, Activity activity);

    void showInfoMessage(String message, Activity activity);

    boolean isGameFileInstall(Activity activity, String filePath);

    void showBigMessage(String message, Activity activity);
}
