package com.liverussia.launcher.activity;

import android.Manifest;
import android.content.Intent;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.liverussia.cr.R;

import java.util.Timer;
import java.util.TimerTask;

import java.util.ArrayList;
import java.util.List;

import com.liverussia.cr.core.DownloadUtils;
import com.liverussia.launcher.dto.response.LatestVersionInfoDto;
import com.liverussia.launcher.enums.DownloadType;
import com.liverussia.launcher.error.apiException.ErrorContainer;
import com.liverussia.launcher.model.News;

import com.liverussia.launcher.model.Servers;
import com.liverussia.launcher.other.NetworkService;
import com.liverussia.launcher.other.Lists;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

import static com.liverussia.launcher.config.Config.LIVE_RUSSIA_RESOURCE_SERVER_URI;

public class SplashActivity extends AppCompatActivity {

	private final static String IS_AFTER_LOADING_KEY = "isAfterLoading";
	private final static int EXIT_SUCCESS_STATUS = 0;
	private final static int LAST_VERSION_WITHOUT_NEED_PERMS = 23;

	public static ArrayList<Servers> slist;

	private NetworkService sNetworkService;

	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

		getWindow().getDecorView().setSystemUiVisibility(
				View.SYSTEM_UI_FLAG_IMMERSIVE
						| View.SYSTEM_UI_FLAG_LAYOUT_STABLE
						| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
						| View.SYSTEM_UI_FLAG_FULLSCREEN
						| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);

		Lists.SERVERS = new ArrayList<>();
		Lists.NEWS = new ArrayList<>();

		Retrofit retrofit = new Retrofit.Builder()
				.baseUrl(LIVE_RUSSIA_RESOURCE_SERVER_URI)
				.addConverterFactory(GsonConverterFactory.create())
				.build();

		sNetworkService = retrofit.create(NetworkService.class);

		loadServers();
		loadNews();
		startApp();
	}

	private void startApp() {
		if (Build.VERSION.SDK_INT >= LAST_VERSION_WITHOUT_NEED_PERMS) {
			if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED
					|| checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED || checkSelfPermission(Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_DENIED) {
				//requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, 1000);
				startActivity(new Intent(this, PolicyActivity.class));
				finish();
			} else {
				checkVersionAndStartLauncher();
			}
		} else {
			checkVersionAndStartLauncher();
		}
	}

	private void loadNews() {
		Call<List<News>> newsCall = sNetworkService.getNews();

		newsCall.enqueue(new Callback<List<News>>() {
			@Override
			public void onResponse(Call<List<News>> call, Response<List<News>> response) {
				if (response.isSuccessful()) {
					List<News> news = response.body();

					for (News story : news) {
						Lists.NEWS.add(new News(story.getImageUrl(), story.getTitle(), story.getUrl()));
					}
				}
			}

			@Override
			public void onFailure(Call<List<News>> call, Throwable t) {
			}
		});
	}

	private void loadServers() {
		Call<List<Servers>> serversCall = sNetworkService.getServers();

		serversCall.enqueue(new Callback<List<Servers>>() {
			@Override
			public void onResponse(Call<List<Servers>> call, Response<List<Servers>> response) {
				if (response.isSuccessful()) {
					List<Servers> servers = response.body();

					for (Servers server : servers) {
						Lists.SERVERS.add(new Servers(server.getColor(), server.getServerID(), server.getDopname(), server.getname(), server.getOnline(), server.getmaxOnline(), server.getColorl()));
					}
				}
			}

			@Override
			public void onFailure(Call<List<Servers>> call, Throwable t) {
				Toast.makeText(getApplicationContext(), ErrorContainer.SERVER_CONNECT_ERROR.getMessage(), Toast.LENGTH_SHORT).show();
			}
		});
	}
	
	private void checkVersionAndStartLauncher() {
		Call<LatestVersionInfoDto> latestVersionInfoCall = sNetworkService.getLatestVersionInfoDto();

		latestVersionInfoCall.enqueue(new Callback<LatestVersionInfoDto>() {
			@Override
			public void onResponse(Call<LatestVersionInfoDto> call, Response<LatestVersionInfoDto> response) {
				if (!response.isSuccessful()) {
					finish();
					System.exit(0);
				}

				checkVersion(response.body());
			}

			@Override
			public void onFailure(Call<LatestVersionInfoDto> call, Throwable t) {
				finish();
				System.exit(0);
			}
		});
    }

	private void startLauncher() {
		Intent intent = new Intent(this, MainActivity.class);

		if (getIntent().getExtras() != null && getIntent().getExtras().getBoolean(IS_AFTER_LOADING_KEY)){
			intent.putExtras(getIntent());
			intent.putExtra(IS_AFTER_LOADING_KEY, true);
		}

		startActivity(intent);
		finish();
	}

	private void checkVersion(LatestVersionInfoDto latestVersionInfo) {

		String currentVersion = getCurrentVersion();
		String latestVersion = latestVersionInfo.getVersion();

		if (currentVersion.equals(latestVersion)) {
			startLauncher();
			return;
		}

		DownloadUtils.setType(DownloadType.UPDATE_APK);
		DownloadUtils.LATEST_APK_INFO = latestVersionInfo;
		startActivity(new Intent(this, LoaderActivity.class));
	}

	private String getCurrentVersion(){
		PackageManager pm = this.getPackageManager();

		try {
			PackageInfo pInfo = pm.getPackageInfo(this.getPackageName(),0);
			return pInfo.versionName;
		} catch (PackageManager.NameNotFoundException e1) {
			e1.printStackTrace();
		}

		finish();
		System.exit(EXIT_SUCCESS_STATUS);

		return null;
	}

	public static boolean isOnline(Context context) {
		ConnectivityManager cm =
				(ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo netInfo = cm.getActiveNetworkInfo();

		return netInfo != null && netInfo.isConnectedOrConnecting();
	}
}