package com.liverussia.launcher.async.task;

import android.content.Context;
import android.os.PowerManager;
import android.view.View;
import android.view.WindowManager;
import android.widget.ProgressBar;

import androidx.annotation.UiThread;
import androidx.annotation.WorkerThread;

import com.google.firebase.crashlytics.FirebaseCrashlytics;
import com.liverussia.launcher.utils.MainUtils;
import com.liverussia.launcher.ui.activity.LoaderActivity;
import com.liverussia.launcher.async.domain.AsyncTaskResult;
import com.liverussia.launcher.async.listener.OnAsyncCriticalErrorListener;
import com.liverussia.launcher.async.listener.OnAsyncSuccessListener;
import com.liverussia.launcher.async.dto.response.FileInfo;
import com.liverussia.launcher.async.dto.response.GameFileInfoDto;
import com.liverussia.launcher.async.dto.response.LatestVersionInfoDto;
import com.liverussia.launcher.domain.enums.DownloadType;
import com.liverussia.launcher.error.apiException.ApiException;
import com.liverussia.launcher.error.apiException.ErrorContainer;
import com.liverussia.launcher.service.ActivityService;
import com.liverussia.launcher.service.impl.ActivityServiceImpl;
import com.techyourchance.threadposter.BackgroundThreadPoster;
import com.techyourchance.threadposter.UiThreadPoster;

import org.apache.commons.lang3.StringUtils;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.http.converter.json.MappingJackson2HttpMessageConverter;
import org.springframework.web.client.RestTemplate;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import java.net.SocketException;
import java.net.URI;
import java.net.URL;
import java.net.UnknownHostException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.concurrent.atomic.AtomicLong;

import javax.net.ssl.SSLException;

import static com.liverussia.launcher.config.Config.FILE_INFO_URL;

public class DownloadTask implements Listener<TaskStatus> {

    private final LoaderActivity loaderActivity;
    private final ProgressBar progressBar;
    private final ActivityService activityService;

    private OnAsyncSuccessListener onAsyncSuccessListener;
    private OnAsyncCriticalErrorListener onAsyncCriticalErrorListener;

    private PowerManager.WakeLock mWakeLock;

    private long fileLengthFull;
    private AtomicLong fileLengthMin;
    private long total;
    private Set<FileInfo> files;

    private final UiThreadPoster uiThreadPoster;
    private final BackgroundThreadPoster backgroundThreadPoster;

    public DownloadTask(LoaderActivity loaderActivity, ProgressBar progressBar) {
        this.loaderActivity = loaderActivity;
        this.progressBar = progressBar;
    }

    {
        activityService = new ActivityServiceImpl();
        uiThreadPoster = new UiThreadPoster();
        backgroundThreadPoster = new BackgroundThreadPoster();
    }

    public void download() {
        configureProgressBar();
        backgroundThreadPoster.post(this::downloadGameFiles);
    }

    public void reloadCache() {
        configureProgressBar();
        backgroundThreadPoster.post(this::reloadGameFiles);
    }

    public void updateApk() {
        configureProgressBar();
        backgroundThreadPoster.post(this::downloadApk);
    }

    @WorkerThread
    private void downloadApk() {
        LatestVersionInfoDto apkInfo = MainUtils.LATEST_APK_INFO;
        FileInfo fileInfo = buildFileInfoForApkFile(apkInfo);

        List<FileInfo> filesInfo = new ArrayList<>();
        filesInfo.add(fileInfo);

        fileLengthFull = apkInfo.getSize();
        fileLengthMin = new AtomicLong(0);
        total = 0;

        files = new HashSet<>(filesInfo);

        try {
            downloadAndSaveFiles(filesInfo);
        } catch (ApiException e) {
            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(e));
            });
        } catch (Exception e) {
            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(new ApiException(ErrorContainer.DOWNLOAD_FILES_ERROR)));
            });
        }
    }

    private FileInfo buildFileInfoForApkFile(LatestVersionInfoDto apkInfo) {
        FileInfo fileInfo = new FileInfo();
        fileInfo.setLink(apkInfo.getLink());
        fileInfo.setSize(apkInfo.getSize());
        fileInfo.setPath(apkInfo.getPath());

        return fileInfo;
    }

    @WorkerThread
    private void reloadGameFiles() {
        List<FileInfo> filesInfo = MainUtils.FILES_TO_RELOAD;
        fileLengthFull = calculateSize(filesInfo);
        fileLengthMin = new AtomicLong(0);
        total = 0;
        files = new HashSet<>(filesInfo);

        try {
            downloadAndSaveFiles(filesInfo);
        } catch (ApiException e) {
            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(e));
            });
        } catch (Exception e) {
            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(new ApiException(ErrorContainer.DOWNLOAD_FILES_ERROR)));
            });
        }
    }

    @WorkerThread
    private void downloadGameFiles() {
        try {
            GameFileInfoDto gameFilesInfo = getGameFilesInfo();

            fileLengthFull = gameFilesInfo.getAllFilesSize();
            fileLengthMin = new AtomicLong(0);
            total = 0;

            files = new HashSet<>(gameFilesInfo.getFiles());

            downloadAndSaveFiles(gameFilesInfo.getFiles());
        } catch (ApiException e) {
            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(e));
            });
        } catch (Exception e) {
            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(new ApiException(ErrorContainer.DOWNLOAD_FILES_ERROR)));
            });
        }
    }

    @WorkerThread
    private void downloadAndSaveFilesAfterFailed(List<FileInfo> gameFilesInfo) {
        try {
            Optional.ofNullable(gameFilesInfo)
                    .orElse(Collections.emptyList())
                    .forEach(this::downloadAndSaveFile);

            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(TaskStatus.SUCCESS));
            });
        } catch (ApiException e) {
            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(e));
            });
        } catch (Exception e) {
            uiThreadPoster.post(() -> {
                onAsyncFinished(new AsyncTaskResult<>(new ApiException(ErrorContainer.DOWNLOAD_FILES_ERROR)));
            });
        }
    }

    private long calculateSize(List<FileInfo> filesInfo) {
        return Optional.ofNullable(filesInfo)
                .orElse(Collections.emptyList())
                .stream()
                .mapToLong(FileInfo::getSize)
                .sum();
    }

    private GameFileInfoDto getGameFilesInfo() {
        URI url = URI.create(FILE_INFO_URL);

        RestTemplate restTemplate = new RestTemplate();
        restTemplate.getMessageConverters().add(new MappingJackson2HttpMessageConverter());
        ResponseEntity<GameFileInfoDto> response = restTemplate.exchange(url, HttpMethod.GET, HttpEntity.EMPTY, GameFileInfoDto.class);

        return response.getBody();
    }

    private void downloadAndSaveFiles(List<FileInfo> gameFilesInfo) {
        Optional.ofNullable(gameFilesInfo)
                .orElse(Collections.emptyList())
                .forEach(this::downloadAndSaveFile);

        uiThreadPoster.post(() -> {
            onAsyncFinished(new AsyncTaskResult<>(TaskStatus.SUCCESS));
        });

    }

    private void downloadAndSaveFile(FileInfo fileInfo) {
        InputStream input = null;
        OutputStream output = null;
        HttpURLConnection connection = null;
        long fileLoadedSize = 0;
        File file = null;

        try {
            URL url = new URL(fileInfo.getLink());
            connection = (HttpURLConnection) url.openConnection();
            connection.connect();

            if (connection.getResponseCode() != HttpURLConnection.HTTP_OK) {
                throw new ApiException(ErrorContainer.DOWNLOAD_FILES_ERROR);
            }

            long fileLength = connection.getContentLengthLong();
            input = connection.getInputStream();

            file = new File(loaderActivity.getExternalFilesDir(null).toString()
                    .concat("/")
                    .concat(fileInfo.getPath().replace("files/", ""))
            );

            if (file.exists()) {
                file.delete();
            }

            file.getParentFile().mkdirs();

            output = new FileOutputStream(loaderActivity
                    .getExternalFilesDir(null).toString()
                    .concat("/")
                    .concat(fileInfo.getPath().replace("files/", ""))
            );

            byte[] data = new byte[4096];
            int count;

            while ((count = input.read(data)) != -1) {
                total += count;
                fileLoadedSize += count;

                if (fileLength > 0) {
                    uiThreadPoster.post(() -> {
                        if (fileLengthFull == 0) {
                            fileLengthFull += 1;
                        }
                        publishProgress((int) (total * 100 / fileLengthFull));
                    });
                }

                fileLengthMin.set(total);

                output.write(data, 0, count);
            }

            files.remove(fileInfo);
        } catch (ApiException e) {
           apiExceptionAction(file, e);
        } catch (UnknownHostException | SocketException | SSLException | ProtocolException e) {
            internetDisableAction(file, fileLoadedSize);
        } catch (IOException e) {
            noSpaceLeftOnDeviceAction(file);
        } catch (Exception e) {
            unknownErrorAction(file, e);
        } finally {
            afterLoadingFileCompleteAction(output, input, connection);
        }
    }

    private void afterLoadingFileCompleteAction(OutputStream output, InputStream input, HttpURLConnection connection) {
        try {
            if (output != null)
                output.close();
            if (input != null)
                input.close();
        } catch (IOException ignored) {
        }

        if (connection != null){
            connection.disconnect();
        }
    }

    private void apiExceptionAction(File file, ApiException e) {
        if (file != null && file.exists()) {
            file.delete();
        }

        throw new ApiException(e);
    }

    private void unknownErrorAction(File file, Exception e) {
        if (file != null && file.exists()) {
            file.delete();
        }

        FirebaseCrashlytics.getInstance().recordException(e);
        throw new ApiException(ErrorContainer.DOWNLOAD_FILES_ERROR);
    }

    private void noSpaceLeftOnDeviceAction(File file) {
        if (file != null && file.exists()) {
            file.delete();
        }

        throw new ApiException(ErrorContainer.NO_SPACE_LEFT_ON_DEVICE);
    }

    private void internetDisableAction(File file, long fileLoadedSize) {
        boolean isDeleted = false;

        if (file != null && file.exists()) {
            isDeleted = file.delete();
        }

        if (isDeleted) {
            total -= fileLoadedSize;
        }

        throw new ApiException(ErrorContainer.INTERNET_WAS_DISABLE);
    }

    @UiThread
    private void publishProgress(int progress) {
        if (DownloadType.LOAD_ALL_CACHE.equals(MainUtils.getType()) || DownloadType.RELOAD_OR_ADD_PART_OF_CACHE.equals(MainUtils.getType())) {
            loaderActivity.getLoading().setText("Загрузка файлов игры...");
            loaderActivity.getLoadingPercent().setText(progress + "%");
            loaderActivity.getFileName().setText(formatFileSize(fileLengthMin.longValue())+" из "+formatFileSize(fileLengthFull));
        }

        if (DownloadType.UPDATE_APK.equals(MainUtils.getType())) {
            loaderActivity.getLoading().setText("Обновление...");
            loaderActivity.getLoadingPercent().setText(progress + "%");
            loaderActivity.getFileName().setText(formatFileSize(fileLengthMin.longValue())+" из "+formatFileSize(fileLengthFull));
        }

        progressBar.setProgress(progress);
    }

    @Override
    @UiThread
    public void onAsyncFinished(AsyncTaskResult<TaskStatus> result) {

        if (isInternetDisableException(result.getException())) {
            loaderActivity.getRepeatLoadButton().setVisibility(View.VISIBLE);
            loaderActivity.getLoading().setText("Ошибка соединения...");
            return;
        }

        loaderActivity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        if (mWakeLock.isHeld())
            mWakeLock.release();
        loaderActivity.getLoading().setVisibility(View.INVISIBLE);
        loaderActivity.getLoadingPercent().setVisibility(View.INVISIBLE);
        progressBar.setVisibility(View.INVISIBLE);

        if (result.getException() != null) {
            ApiException apiException = result.getException();
            showErrorMessage(apiException);
            onAsyncErrorDo();
        } else if (onAsyncSuccessListener != null) {
            onAsyncSuccessListener.onSuccess();
        }
    }

    private boolean isInternetDisableException(ApiException exception) {
        return Optional.ofNullable(exception)
                .map(ApiException::getError)
                .filter(ErrorContainer.INTERNET_WAS_DISABLE::equals)
                .isPresent();
    }

    private void showErrorMessage(ApiException exception) {
        String errorMessage = Optional.ofNullable(exception)
                .map(ApiException::getError)
                .map(ErrorContainer::getMessage)
                .orElse(StringUtils.EMPTY);

        activityService.showErrorMessage(errorMessage, loaderActivity);
    }

    private void onAsyncErrorDo() {
        if (onAsyncCriticalErrorListener != null) {
            onAsyncCriticalErrorListener.onCriticalError();
        }
    }


    public void setOnAsyncSuccessListener(OnAsyncSuccessListener onClickListener) {
        this.onAsyncSuccessListener = onClickListener;
    }

    public void setOnAsyncCriticalErrorListener(OnAsyncCriticalErrorListener onAsyncCriticalErrorListener) {
        this.onAsyncCriticalErrorListener = onAsyncCriticalErrorListener;
    }


    public static String formatFileSize(long size) {
        String hrSize = null;

        double b = size;
        double k = size/1024.0;
        double m = ((size/1024.0)/1024.0);
        double g = (((size/1024.0)/1024.0)/1024.0);
        double t = ((((size/1024.0)/1024.0)/1024.0)/1024.0);

        DecimalFormat dec = new DecimalFormat("0.00");

        if ( t>1 ) {
            hrSize = dec.format(t).concat(" TB");
        } else if ( g>1 ) {
            hrSize = dec.format(g).concat(" GB");
        } else if ( m>1 ) {
            hrSize = dec.format(m).concat(" MB");
        } else if ( k>1 ) {
            hrSize = dec.format(k).concat(" KB");
        } else {
            hrSize = dec.format(b).concat(" Bytes");
        }

        return hrSize;
    }

    private void configureProgressBar() {
        loaderActivity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        PowerManager pm = (PowerManager) loaderActivity.getSystemService(Context.POWER_SERVICE);
        mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, getClass().getName());
        mWakeLock.acquire();
        progressBar.setVisibility(View.VISIBLE);
        progressBar.setIndeterminate(false);
        progressBar.setMax(100);
        loaderActivity.getLoading().setText("Загрузка файлов игры...");
    }

    public void repeatLoad() {
        loaderActivity.getRepeatLoadButton().setVisibility(View.INVISIBLE);
        backgroundThreadPoster.post(() -> downloadAndSaveFilesAfterFailed(new ArrayList<>(files)));
    }
}
