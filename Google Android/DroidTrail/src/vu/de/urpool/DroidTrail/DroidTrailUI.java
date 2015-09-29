package vu.de.urpool.DroidTrail;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.Vibrator;
import android.util.Log;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ContextMenu.ContextMenuInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class DroidTrailUI extends Activity implements View.OnCreateContextMenuListener {
	private static final String LOG_TAG = "DROID_TRAIL_UI";
	private IDroidTrailService mDroidTrailService;
    private DroidTrailServiceConnection mDroidTrailServiceConnection;
    final Handler mHandler = new Handler();
    private boolean mRestoreSavedInstanceState = false;
    public static final int CONVERT_TRAIL = Menu.FIRST;
    public static final int DELETE_TRAIL = Menu.FIRST + 1;
    boolean mEnableOptionsMenu = true;
    EditText mTrailName;
    EditText mMinTime;
    EditText mMinDistance;
    Button mStartLogging;
    Button mStopLogging;
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.droid_trail);
        
        mTrailName = (EditText) findViewById( R.id.trailName );
        mTrailName.setText("MyTrail");
        mMinTime = (EditText) findViewById( R.id.minTime );
        mMinDistance = (EditText) findViewById( R.id.minDistance );
        
        mStartLogging = (Button) findViewById( R.id.startLogging );
        mStartLogging.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
            	startService();
            	if (startLogging()) {
            		mTrailName.setEnabled(false);
            		mMinTime.setEnabled(false);
            		mMinDistance.setEnabled(false);
	            	mStartLogging.setEnabled(false);
	          	    mStopLogging.setEnabled(true);
	          	    mEnableOptionsMenu = false;
            	} else {
            		stopService();
            	}
     		}
        });

        mStopLogging = (Button) findViewById( R.id.stopLogging );
        mStopLogging.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
            	stopLogging();
            	stopService();
            	mTrailName.setEnabled(true);
            	mMinTime.setEnabled(true);
        		mMinDistance.setEnabled(true);
            	mStartLogging.setEnabled(true);
        	    mStopLogging.setEnabled(false);
        	    mEnableOptionsMenu = true;
     		}
        });
        
        mTrailName.setOnCreateContextMenuListener(this);
        
        bindService();
        
        if(savedInstanceState != null && savedInstanceState.getString("TrailName") != null) {
        	mRestoreSavedInstanceState = true;     	
        } else {
        	SharedPreferences settings = getSharedPreferences("Settings", 0);
        	mTrailName.setText(settings.getString("trailName", mTrailName.getText().toString()));
        }
    }
    
    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {
    	savedInstanceState.putString("TrailName", mTrailName.getText().toString());
    	savedInstanceState.putString("MinTime", mMinTime.getText().toString());
    	savedInstanceState.putString("MinDistance", mMinDistance.getText().toString());
    	super.onSaveInstanceState(savedInstanceState);
    }
    
    @Override
    public void onRestoreInstanceState(Bundle savedInstanceState) {
    	super.onRestoreInstanceState(savedInstanceState);
    	mTrailName.setText(savedInstanceState.getString("TrailName"));
    	mMinTime.setText(savedInstanceState.getString("MinTime"));
    	mMinDistance.setText(savedInstanceState.getString("MinDistance"));
    }
    
    private final Runnable mInitUI = new Runnable() {
        public void run() {
	    	boolean isLogging = false;
			try {
				isLogging = mDroidTrailService.isLogging();
				if(!mRestoreSavedInstanceState) {
					mMinTime.setText(String.valueOf(mDroidTrailService.getMinTime() / 1000));
					mMinDistance.setText(String.valueOf( (long)mDroidTrailService.getMinDistance() ));					
				}
			} catch (RemoteException e) {
				Log.e( LOG_TAG, "Oops" );
			}
			mTrailName.setEnabled(!isLogging);
			mMinTime.setEnabled(!isLogging);
    		mMinDistance.setEnabled(!isLogging);
	    	mStartLogging.setEnabled(!isLogging);
	    	mStopLogging.setEnabled(isLogging);
	    	mEnableOptionsMenu = !isLogging;
        }
    };
    
    @Override
    protected void onDestroy() {
    	unbindService();
    	super.onDestroy();
    }
        
    private void startService() {
  	    Intent intent = new Intent();
  	    intent.setClassName( "vu.de.urpool.DroidTrail", "vu.de.urpool.DroidTrail.DroidTrailService" );
  	    startService( intent );  	    
    }
    
    private void stopService() {   	
  	    Intent intent = new Intent();
  	    intent.setClassName( "vu.de.urpool.DroidTrail", "vu.de.urpool.DroidTrail.DroidTrailService" );
  	    stopService( intent );
    }
    
    private void bindService() {  	
    	if( mDroidTrailServiceConnection == null ) {
    		Intent intent = new Intent();
      	    intent.setClassName( "vu.de.urpool.DroidTrail", "vu.de.urpool.DroidTrail.DroidTrailService" );
			mDroidTrailServiceConnection = new DroidTrailServiceConnection();		    
			bindService(intent, mDroidTrailServiceConnection, Context.BIND_AUTO_CREATE);
			Log.d( LOG_TAG, "bound Service" );
		}
    }
    
    private void unbindService() {
    	if( mDroidTrailServiceConnection != null ) {
    	    unbindService( mDroidTrailServiceConnection );
    	    mDroidTrailServiceConnection = null;
    		Log.d( LOG_TAG, "unbound Service" );
    	}
    }
    
    private boolean startLogging() {
    	SharedPreferences settings = getSharedPreferences("Settings", 0);
		SharedPreferences.Editor editor = settings.edit();
		editor.putString("trailName", mTrailName.getText().toString());
		editor.commit();
    	try {
    		mDroidTrailService.setMinTime(Long.valueOf(mMinTime.getText().toString()) * 1000);
    		mDroidTrailService.setMinDistance(Float.valueOf(mMinDistance.getText().toString()));
			if (!mDroidTrailService.startLogging(mTrailName.getText().toString() + ".dat")) {
				Vibrator vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
				vibrator.vibrate(100);
				return false;
			}
		} catch (RemoteException e) {
			Log.e( LOG_TAG, "Oops" );
			return false;
		} catch (NumberFormatException e) {
			Log.e( LOG_TAG, "Oops" );
			return false;
		}		
		return true;
		
    }
    
    private void stopLogging() {
    	try {
			mDroidTrailService.stopLogging();
		} catch (RemoteException e) {
			Log.e( LOG_TAG, "Oops" );
		}		
    }
    
    private boolean convertDroidTrail(IDroidTrailConverter droidTrailConverter) {
    	File inputDir = getDir("Trails", Context.MODE_PRIVATE);
        File inputFile = new File(inputDir, mTrailName.getText().toString() + ".dat");
        if (!inputFile.exists()) {
        	return false;
        }
        File outputDir = Environment.getExternalStorageDirectory();
        File outputFile = new File(outputDir, mTrailName.getText().toString() + ".kml");
		if (!outputFile.exists()) {
			try {
				outputFile.createNewFile();
			} catch (IOException ex) {
				return false;
			}
		}
		
		DataInputStream input;
		DataOutputStream output;
		try {
			input = new DataInputStream( new FileInputStream(inputFile) );
			output = new DataOutputStream( new FileOutputStream(outputFile) );
			
			droidTrailConverter.writeHeader(output, mTrailName.getText().toString());
			for(int i = 0; i < inputFile.length() / 16; i++) {
				droidTrailConverter.writeCoordinates(output, input.readDouble(), input.readDouble());
			}
			droidTrailConverter.writeFooter(output);
			
			input.close();
			output.close();	
		} catch (FileNotFoundException e) {
			Log.e( LOG_TAG, "Oops" );
			return false;
		} catch (IOException e) {
			Log.e( LOG_TAG, "Oops" );
			return false;
		}

		return true;
    }
    
    @Override
    public void onCreateContextMenu(ContextMenu menu, View view, ContextMenuInfo menuInfo) {
        menu.clear();
        menu.setHeaderTitle(R.string.loggedTrails);
        File dir = getDir("Trails", Context.MODE_PRIVATE);
		if (dir.isDirectory()) {
			if(dir.list().length > 0) {
				String[] files = dir.list();
				Arrays.sort(files, String.CASE_INSENSITIVE_ORDER);
				int id = 1;
				for(String file : files) {
					menu.add(0, id++, 0, file.substring(0, file.lastIndexOf(".dat")));
				}				
			} else {
				menu.add(0, 0, 0, R.string.noLoggedTrails);
			}
		}
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case 0:
            	return true;
            default:
            	mTrailName.setText(item.getTitle());
            	return true;
        }
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // onCreateOptionsMenu is only called once
        return super.onCreateOptionsMenu(menu);
    }
    
    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
    	menu.clear();
    	if(mEnableOptionsMenu) {
        	MenuItem converterItem = menu.add(0, CONVERT_TRAIL, 0, R.string.trailConverter);
        	converterItem.setIcon(R.drawable.star);
        	MenuItem deleterItem = menu.add(0, DELETE_TRAIL, 0, R.string.deleteTrail);
        	deleterItem.setIcon(R.drawable.star);        	
        }
    	return true;
    }
	
	@Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        	case CONVERT_TRAIL:        		
        		IDroidTrailConverter droidTrailConverter = new GoogleEarthConverter();
            	String message;
        		if( convertDroidTrail(droidTrailConverter) ) {
            		message = String.format(getResources().getText(R.string.trailConverterPopupOK).toString(), mTrailName.getText() + ".kml");
            	} else {
            		message = String.format(getResources().getText(R.string.trailConverterPopupError).toString(), mTrailName.getText());
            	}
            	Toast.makeText(getApplicationContext(),
        			message,
        			Toast.LENGTH_SHORT).show();
        		return true;
        	case DELETE_TRAIL:
        		File dir = getDir("Trails", Context.MODE_PRIVATE);
                File file = new File(dir, mTrailName.getText().toString() + ".dat");
                if (file.exists()) {
                	file.delete();
                }
                Toast.makeText(getApplicationContext(),
                	String.format(getResources().getText(R.string.trailDeleterPopup).toString(), mTrailName.getText()),
                	Toast.LENGTH_SHORT).show();
        		return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
    class DroidTrailServiceConnection implements ServiceConnection {	
    	public void onServiceConnected(ComponentName name, IBinder service ) {
        	mDroidTrailService = IDroidTrailService.Stub.asInterface( (IBinder) service );
        	mHandler.post(mInitUI);
        	Log.d( LOG_TAG, "onServiceConnected" );
        }

        public void onServiceDisconnected(ComponentName name) {
        	mDroidTrailService = null;
        	Log.d( LOG_TAG, "onServiceDisconnected" );
        }
    };
}