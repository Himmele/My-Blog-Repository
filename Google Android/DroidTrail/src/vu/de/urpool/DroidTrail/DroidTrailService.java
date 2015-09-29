package vu.de.urpool.DroidTrail;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import java.io.*;

public class DroidTrailService extends Service {

	private static final String LOG_TAG = "DROID_TRAIL_SERVICE";
	private static final int DROID_TRAIL_OK = 1;
	private static final int DROID_TRAIL_ERROR = 2;
	private LocationManager mLocationManager;
	private int mLocationProviderAccurary = Criteria.ACCURACY_FINE;
	private NotificationManager mNM;
	private DataOutputStream mLogFile;
	private long mMinTime = 60000; // 60 sec
	private float mMinDistance = 100; // 100 meters
	private String mLogFileName;
	private DroidTrailState mDroidTrailState = DroidTrailState.OFF;

	@Override
    public void onCreate() {
    	super.onCreate();
    	
    	mLocationManager = (LocationManager)getSystemService(Context.LOCATION_SERVICE);
    	mNM = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
    	
    	SharedPreferences settings = getSharedPreferences("Settings", 0);
    	mMinTime = settings.getLong("minTime", mMinTime);
        mMinDistance = settings.getFloat("minDistance", mMinDistance);
    }
	
	@Override
    public void onStart(Intent intent, int startId) {
    	super.onStart( intent, startId );
    }

    @Override
	public void onDestroy() {
    	super.onDestroy();
    }
    
    private void showNotification(int id, String text) {
    	clearNotification();
    	Notification notification = null;
    	switch(id) {
    	case DROID_TRAIL_OK:
    		if (Criteria.ACCURACY_FINE == mLocationProviderAccurary) {
    			notification = new Notification(R.drawable.droid_trail_ok_good_accuracy, null, System.currentTimeMillis());
    		} else {
    			notification = new Notification(R.drawable.droid_trail_ok_bad_accuracy, null, System.currentTimeMillis());
    		}    		
    		break;
    	case DROID_TRAIL_ERROR:
    		notification = new Notification(R.drawable.droid_trail_error, null, System.currentTimeMillis());
    		break;
    	}    	 
		PendingIntent contentIntent = PendingIntent.getActivity(this, 0, new Intent(this, DroidTrailUI.class), 0);
		notification.flags |= (Notification.FLAG_NO_CLEAR | Notification.FLAG_ONGOING_EVENT);
		notification.setLatestEventInfo(this, getText(R.string.appName), text, contentIntent);
		mNM.notify(1, notification);
    }
    
    private void clearNotification() {
    	mNM.cancel(1);
    }

    private final IDroidTrailService.Stub mBinder = new IDroidTrailService.Stub() {	
    	public boolean startLogging(String logFileName) {
    		if(mDroidTrailState != DroidTrailState.OFF) {
    			return false;
    		}
    		if(logFileName == null) {
    			return false;
    		}
    		
    		mLogFileName = logFileName;
    		Criteria providerCriteria = new Criteria();
    		providerCriteria.setAccuracy(Criteria.ACCURACY_FINE);
    		String providerName = mLocationManager.getBestProvider(providerCriteria, true);
    		if (providerName == null) {
    			return false;
    		}
    		mLocationProviderAccurary = mLocationManager.getProvider(providerName).getAccuracy();
        	
        	try {
        		File dir = getDir("Trails", Context.MODE_PRIVATE);
                File file = new File(dir, mLogFileName);
        		if( !file.exists() ) {
        			file.createNewFile();   			
        		}
        		mLogFile = new DataOutputStream( new FileOutputStream(file, true) );
    		} catch (FileNotFoundException ex) {
    			Log.e( LOG_TAG, "Oops" );
    			return false;
    		} catch (IOException e) {
    			Log.e( LOG_TAG, "Oops" );
    			return false;
    		}
    		
    		mLocationManager.requestLocationUpdates(providerName, 
                mMinTime, 
                mMinDistance, 
                mLocationListener);
    		
    		mDroidTrailState = DroidTrailState.ON;        	
    		showNotification(DROID_TRAIL_OK, "");
    		
        	return true;
    	}
    	
    	public void stopLogging() {
    		if (mDroidTrailState == DroidTrailState.ON) {
	    		mLocationManager.removeUpdates(mLocationListener);
	    		clearNotification();
	    		try {	
					mLogFile.close();
				} catch (IOException e) {
					Log.e( LOG_TAG, "Oops" );
				} finally {
					mDroidTrailState = DroidTrailState.OFF;					
				}
    		}
    	}    	
    	
    	public boolean isLogging() {
    		return mDroidTrailState == DroidTrailState.ON;
    	}
    	
    	public void setMinTime(long minTime) {
    		mMinTime = minTime;
    		SharedPreferences settings = getSharedPreferences("Settings", 0);
    		SharedPreferences.Editor editor = settings.edit();
    		editor.putLong("minTime", mMinTime);
    		editor.commit();
    	}
    	
    	public long getMinTime() {    		
    		SharedPreferences settings = getSharedPreferences("Settings", 0);
    		return settings.getLong("minTime", mMinTime);
    	}
    	
    	public void setMinDistance(float minDistance) {
    		mMinDistance = minDistance;
    		SharedPreferences settings = getSharedPreferences("Settings", 0);
    		SharedPreferences.Editor editor = settings.edit();
    		editor.putFloat("minDistance", mMinDistance);
    		editor.commit();
    	}
    	
    	public float getMinDistance() {    		
    		SharedPreferences settings = getSharedPreferences("Settings", 0);
    		return settings.getFloat("minDistance", mMinDistance);
    	}
    	
    	public String getLogFileName() {
    		return mLogFileName;
    	}
    };

	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}
	
	LocationListener mLocationListener = new LocationListener(){		
    	public void onLocationChanged(Location location) {
            if (location != null) {            	
            	try {
					mLogFile.writeDouble(location.getLongitude());
					mLogFile.writeDouble(location.getLatitude());					
				} catch (IOException e) {
					Log.e( LOG_TAG, "Oops" );
				}
            }
        }

		public void onProviderDisabled(String provider) {
			showNotification(DROID_TRAIL_ERROR, "");
		}

		public void onProviderEnabled(String provider) {
			showNotification(DROID_TRAIL_OK, "");
		}

		public void onStatusChanged(String provider, int status, Bundle extras) {
			switch(status) {
			case LocationProvider.AVAILABLE:
				showNotification(DROID_TRAIL_OK, "");
				break;
			case LocationProvider.OUT_OF_SERVICE:
			case LocationProvider.TEMPORARILY_UNAVAILABLE:
				showNotification(DROID_TRAIL_ERROR, "");
				break;
			}
		}
    };
}