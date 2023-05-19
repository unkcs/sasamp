package com.liverussia.launcher.download;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.DownloadManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.Cursor;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.liverussia.cr.R;
import com.liverussia.launcher.config.Config;
import com.liverussia.launcher.ui.activity.SplashActivity;
import com.liverussia.launcher.utils.BytesTo;
import com.liverussia.launcher.utils.UnZip;

import java.io.File;
import java.io.IOException;

public class DownloadFullCache {
    long downloadId;
    Activity activity;
    DownloadManager downloadManager;
    ProgressBar progressBar;
    TextView loadingText;
    TextView loadingPercent;
    TextView fileName;
    long lastSize;

    boolean needUpdateProgress;

    public DownloadFullCache(Activity activity) {
        this.activity = activity;

        progressBar = activity.findViewById(R.id.progressBar);
        loadingText = activity.findViewById(R.id.loadingText);
        loadingPercent = activity.findViewById(R.id.loadingPercent);
        fileName = activity.findViewById(R.id.fileName);

        downloadManager = (DownloadManager) activity.getSystemService(Context.DOWNLOAD_SERVICE);

        fileName.setText("Подключение...");

        Uri uri = Uri.parse(Config.CACHE_ZIP_URL);
        // Uri uri = Uri.parse("https://gamecache.hb.bizmrg.com/cache.zip");
        DownloadManager.Request request = new DownloadManager.Request(uri);
        request.setVisibleInDownloadsUi(true);
        request.setAllowedNetworkTypes(DownloadManager.Request.NETWORK_WIFI | DownloadManager.Request.NETWORK_MOBILE).setAllowedOverRoaming(true);
        request.setAllowedOverRoaming(true);
        request.setAllowedOverMetered(true);
        request.setNotificationVisibility(DownloadManager.Request.VISIBILITY_VISIBLE);
        request.setDestinationInExternalFilesDir(activity, Environment.DIRECTORY_DOWNLOADS, Config.FULLCACHE_ZIP_NAME);
        request.setTitle("Live Russia");
        request.setDescription("Загрузка файлов игры");

        downloadId = downloadManager.enqueue(request);
        needUpdateProgress = true;

        new Thread(() -> {
            while (needUpdateProgress) {

                Cursor c = downloadManager.query(new DownloadManager.Query().setFilterById(downloadId));

                if (c == null) {
                    onErrorDownLoading();
                    break;
                }
                if (!c.moveToFirst()) {
                    onErrorDownLoading();
                    break;
                }

                int status = c.getInt(c.getColumnIndexOrThrow(DownloadManager.COLUMN_STATUS));

                if (status == DownloadManager.STATUS_SUCCESSFUL) {
                    onComplete();
                    break;
                }
                if (status == DownloadManager.STATUS_FAILED) {
                    onErrorDownLoading();
                    break;
                }
                if (status == DownloadManager.STATUS_PENDING || status == DownloadManager.STATUS_PAUSED)
                {
                    activity.runOnUiThread(() ->
                            fileName.setText("Ожидание повторной попытки...")
                    );
                } else {
                    long totalSize = c.getLong(c.getColumnIndexOrThrow(DownloadManager.COLUMN_TOTAL_SIZE_BYTES));
                    long downloadedSize = c.getLong(c.getColumnIndexOrThrow(DownloadManager.COLUMN_BYTES_DOWNLOADED_SO_FAR));

                    long speed = (long) (downloadedSize - lastSize) / 2; // потому что слип на 2
                    if (lastSize != downloadedSize) lastSize = downloadedSize;

                    long percent = (long) (downloadedSize * 100 / totalSize);

                    // int speed = c.getColumnIndex(DownloadManager.)
                    activity.runOnUiThread(() -> {
                        progressBar.setProgress((int) percent);
                        loadingPercent.setText(String.format("%d%%", percent));

                        fileName.setText(String.format("%s из %s (%s/sec)",
                                BytesTo.convert(downloadedSize),
                                BytesTo.convert(totalSize),
                                BytesTo.convert(speed)
                        ));
                    });
                    //   progressText.setText();
                }
                c.close();

                try {
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
        }).start();

        //   activity.registerReceiver(onComplete, new IntentFilter(DownloadManager.ACTION_DOWNLOAD_COMPLETE));

        progressBar.setMax(100);
        loadingText.setText("Скачивание файлов игры...");
//        registerReceiver(onNotificationClick,
//                new IntentFilter(DownloadManager.ACTION_NOTIFICATION_CLICKED));

    }

    public void onComplete() {
        needUpdateProgress = false;

        String zipPath = activity.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS) + "/" + Config.FULLCACHE_ZIP_NAME;
        String unZipPath = activity.getExternalFilesDir(null).toString();

        activity.runOnUiThread(() -> {
            progressBar.setIndeterminate(true);
            loadingPercent.setText("");
            fileName.setText("Распаковка...");
            loadingText.setText("");
        });

        new Thread(() -> {
            try {
                UnZip.UnZipe(zipPath, unZipPath);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            activity.runOnUiThread(() -> onSuccesUnZipped());
        }).start();
    }

    public void onSuccesUnZipped() {
        // удаляем архив
        String zipPath = activity.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS) + "/" + Config.FULLCACHE_ZIP_NAME;
        File zip = new File(zipPath);
        zip.delete();

        // редиректим на мониторинг
        Intent intent = new Intent(activity, SplashActivity.class);
        intent.putExtras(activity.getIntent());
        activity.startActivity(intent);
        activity.finish();
    }

    public void onErrorDownLoading() {
        activity.runOnUiThread(() -> {
            needUpdateProgress = false;

            Toast.makeText(activity, "Ошибка при скачивании ((", Toast.LENGTH_LONG).show();
            onSuccesUnZipped();
        });
    }

    public void onExitApp() {

        downloadManager.remove(downloadId);

        // удаляем архив
        String zipPath = activity.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS) + "/" + Config.FULLCACHE_ZIP_NAME;
        File zip = new File(zipPath);
        zip.delete();
    }
}
