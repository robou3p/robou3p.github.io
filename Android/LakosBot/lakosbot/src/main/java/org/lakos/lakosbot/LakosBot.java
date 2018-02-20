package org.lakos.lakosbot;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class LakosBot extends Activity implements SensorEventListener {
	
	long startTime = 0;

    //runs without a timer by reposting this handler at the end of the runnable
    Handler timerHandler = new Handler();
	Runnable timerRunnable = new Runnable() {

        @Override
        public void run() {
            long millis = System.currentTimeMillis() - startTime;
            int seconds = (int) (millis / 1000);
            int minutes = seconds / 60;
            seconds = seconds % 60;
            sendMessage(new byte[] { 76 });
			sendMessage(new byte[] { dirL });  
			sendMessage(new byte[] { (byte) ((int) Math.abs(speedL)) });
			sendMessage(new byte[] { 82 });
			sendMessage(new byte[] { dirR });
			sendMessage(new byte[] { (byte) ((int) Math.abs(speedR)) });
            timerHandler.postDelayed(this, 100);
        }
    };
	
	// Debugging
	private static final String TAG = "LakosBot";

	// Message types sent from the BluetoothService Handler
	public static final int MESSAGE_STATE_CHANGE = 1;
	public static final int MESSAGE_READ = 2;
	public static final int MESSAGE_WRITE = 3;
	public static final int MESSAGE_DEVICE_NAME = 4;
	public static final int MESSAGE_TOAST = 5;

	// Key names received from the BluetoothService Handler
	public static final String DEVICE_NAME = "device_name";
	public static final String TOAST = "toast";

	// Intent request codes
	private static final int REQUEST_CONNECT_DEVICE = 1;
	private static final int REQUEST_ENABLE_BT = 2;

	// Calibration states
	private static final int CALIBRATION_FINISHED = 1;
	private static final int CALIBRATION_NEUTRAL = 2;
	private static final int CALIBRATION_FORWARD = 3;
	private static final int CALIBRATION_REVERSE = 4;
	private static final int CALIBRATION_LEFT = 5;
	private static final int CALIBRATION_RIGHT = 6;

	private int mCalibrationState = CALIBRATION_FINISHED;

	// Layout Views
	private TextView mTitle;

	private ListView mMessageView;
	private Button mStartButton;
	private Button mStopButton;
	private Button mCalibrateButton;

	// Sensor manager (for accelerometers)
	private SensorManager mSensorManager;
	// Start/stop state
	private boolean mStarted = false;
	// Name of the connected device
	private String mConnectedDeviceName = null;
	// Array adapter for the accelerometer thread
	private ArrayAdapter<String> mMessageArrayAdapter;
	// String buffer for outgoing messages
	private StringBuffer mOutStringBuffer;
	// Local Bluetooth adapter
	private BluetoothAdapter mBluetoothAdapter = null;
	// Member object for the chat services
	private BluetoothService mBluetoothService = null;

	// Control magic: vectors
	Vector currentVector = new Vector(0, 0, 0);
	Vector neutralVector = new Vector(0, 0, 0);
	Vector forwardVector = new Vector(0, 0, 0);
	Vector reverseVector = new Vector(0, 0, 0);
	Vector leftVector = new Vector(0, 0, 0);
	Vector rightVector = new Vector(0, 0, 0);

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// Set up the window layout
		requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		setContentView(R.layout.lakosbot);
		getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE,
				R.layout.custom_title);

		// Set up the custom title
		mTitle = (TextView) findViewById(R.id.title_left_text);
		mTitle.setText(R.string.app_name);
		mTitle = (TextView) findViewById(R.id.title_right_text);

		// Get local Bluetooth adapter
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		// If the adapter is null, then Bluetooth is not supported
		if (mBluetoothAdapter == null) {
			Toast.makeText(this, "Bluetooth is not available",
					Toast.LENGTH_LONG).show();
			finish();
			return;
		}
	}

	@Override
	public void onStart() {
		super.onStart();
		// If BT is not on, request that it be enabled.
		if (!mBluetoothAdapter.isEnabled()) {
			Intent enableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
			// Otherwise, setup the chat session
		} else {
			if (mBluetoothService == null)
				// Initialize the BluetoothService
				mBluetoothService = new BluetoothService(this, mHandler);

			// Initialize the buffer for outgoing messages
			mOutStringBuffer = new StringBuffer("");
		}

		mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
		// add listener. The listener will be HelloAndroid (this) class
		mSensorManager.registerListener(this,
				mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
				SensorManager.SENSOR_DELAY_NORMAL);

		/*
		 * More sensor speeds (taken from api docs) SENSOR_DELAY_FASTEST get
		 * sensor data as fast as possible SENSOR_DELAY_GAME rate suitable for
		 * games SENSOR_DELAY_NORMAL rate (default) suitable for screen
		 * orientation changes
		 */

		setupUI();
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	}

	private void setupUI() {
		mMessageArrayAdapter = new ArrayAdapter<String>(this, R.layout.message);
		mMessageView = (ListView) findViewById(R.id.messages);
		mMessageView.setAdapter(mMessageArrayAdapter);
		mMessageArrayAdapter.add("Welcome!");

		mStartButton = (Button) findViewById(R.id.buttonStart);
		mStartButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mStarted = true;
				timerHandler.postDelayed(timerRunnable, 0);
			}
		});

		mStopButton = (Button) findViewById(R.id.buttonStop);
		mStopButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mStarted = false;
				byte[] message = new byte[] { 76, 1, 0, 82, 1, 0 };
				sendMessage(message);
				timerHandler.removeCallbacks(timerRunnable);
			}
		});

		mCalibrateButton = (Button) findViewById(R.id.buttonCalibrate);
		mCalibrateButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				switch (mCalibrationState) {
				case CALIBRATION_FINISHED:
					mMessageArrayAdapter
							.add("Position NEUTRAL and press Calibrate.");
					mCalibrationState = CALIBRATION_NEUTRAL;
					break;
				case CALIBRATION_NEUTRAL:
					neutralVector.setValue(currentVector);
					mMessageArrayAdapter
							.add("Position FORWARD and press Calibrate.");
					mCalibrationState = CALIBRATION_FORWARD;
					break;
				case CALIBRATION_FORWARD:
					forwardVector.setValue(currentVector
							.subtract(neutralVector));
					mMessageArrayAdapter
							.add("Position REVERSE and press Calibrate.");
					mCalibrationState = CALIBRATION_REVERSE;
					break;
				case CALIBRATION_REVERSE:
					reverseVector.setValue(currentVector
							.subtract(neutralVector));
					mMessageArrayAdapter
							.add("Position LEFT and press Calibrate.");
					mCalibrationState = CALIBRATION_LEFT;
					break;
				case CALIBRATION_LEFT:
					leftVector.setValue(currentVector.subtract(neutralVector));
					mMessageArrayAdapter
							.add("Position RIGHT and press Calibrate.");
					mCalibrationState = CALIBRATION_RIGHT;
					break;
				case CALIBRATION_RIGHT:
					rightVector.setValue(currentVector.subtract(neutralVector));
					mMessageArrayAdapter.add("Calibration finished.");
					mCalibrationState = CALIBRATION_FINISHED;
					break;
				}
			}
		});
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {

	}

	byte dirL = 0;
	byte dirR = 0;
	float speedL = 0;
	float speedR = 0;
	
	@Override
	public void onSensorChanged(SensorEvent event) {

		// check sensor type
		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {

			// assign directions
			float x = event.values[0];
			float y = event.values[1];
			float z = event.values[2];
			currentVector.setValue(x, y, z);

			if (mStarted) {
				float forward = currentVector.getComponent(forwardVector);
				float reverse = currentVector.getComponent(reverseVector);
				float left = currentVector.getComponent(leftVector);
				float right = currentVector.getComponent(rightVector);

				float speed = 20 * (forward - reverse);
				float speedValue = (float) Math.pow(Math.abs(speed), 1.20f);
				if (speed < 0)
					speedValue *= -0.9;

				float steer = 20 * (right - left);
				float steerValue = (float) Math.pow(Math.abs(steer), 1.20f);
				if (steer < 0)
					steerValue *= -1;

				speedR = speedValue - steerValue/10 - steerValue * speedValue / 100;
				if (speedR > 255)
					speedR = 255;
				else if (speedR < -255)
					speedR = -255;

				speedL = speedValue + steerValue/10 + steerValue * speedValue / 100;
				if (speedL > 255)
					speedL = 255;
				else if (speedL < -255)
					speedL = -255;

				dirL = speedL >= 0 ? (byte) 1 : (byte) 0;
				dirR = speedR >= 0 ? (byte) 1 : (byte) 0;
				// display
				mMessageArrayAdapter.clear();
				mMessageArrayAdapter.add(String.format("%05.2f %05.2f %05.2f",
						x, y, z));
				mMessageArrayAdapter.add(String.format(
						"Left: %03d %03d Right: %03d %03d", (int) dirL, (int) speedL,
						(int) dirR, (int) speedR));

				/*sendMessage(new byte[] { 76, (byte) dirL,
						(byte) ((int) Math.abs(speedL)), 82, (byte) dirR,
						(byte) ((int) Math.abs(speedR)) });*/
				/*
				sendMessage(new byte[] { 76 });
				sendMessage(new byte[] { dirL });
				sendMessage(new byte[] { (byte) ((int) Math.abs(speedL)) });
				sendMessage(new byte[] { 82 });
				sendMessage(new byte[] { dirR });
				sendMessage(new byte[] { (byte) ((int) Math.abs(speedR)) });
				*/
			}
		}
	}

	@Override
	public synchronized void onResume() {
		super.onResume();
		// Performing this check in onResume() covers the case in which BT was
		// not enabled during onStart(), so we were paused to enable it...
		// onResume() will be called when ACTION_REQUEST_ENABLE activity
		// returns.
		if (mBluetoothService != null) {
			// Only if the state is STATE_NONE, do we know that we haven't
			// started already
			if (mBluetoothService.getState() == BluetoothService.STATE_NONE) {
				// Start the Bluetooth chat services
				mBluetoothService.start();
			}
		}
	}

	@Override
	public synchronized void onPause() {
		super.onPause();
		timerHandler.removeCallbacks(timerRunnable);
	}

	@Override
	public void onStop() {
		super.onStop();

	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		// Stop the Bluetooth chat services
		if (mBluetoothService != null)
			mBluetoothService.stop();
		mSensorManager.unregisterListener(this);
		mBluetoothAdapter.disable();
	}

	private void ensureDiscoverable() {
		if (mBluetoothAdapter.getScanMode() != BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
			Intent discoverableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
			discoverableIntent.putExtra(
					BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
			startActivity(discoverableIntent);
		}
	}

	private void sendMessage(byte[] message) {
		// Check that we're actually connected before trying anything
		if (mBluetoothService.getState() != BluetoothService.STATE_CONNECTED) {
			// TODO: notify the user?
			return;
		}

		mBluetoothService.write(message);
		mOutStringBuffer.setLength(0);
	}

	private void sendMessage(String message) {
		// Check that we're actually connected before trying anything
		if (mBluetoothService.getState() != BluetoothService.STATE_CONNECTED) {
			// TODO: notify the user?
			return;
		}

		mBluetoothService.writeString(message);
		mOutStringBuffer.setLength(0);
	}
	
	// The Handler that gets information back from the BluetoothService
	private final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MESSAGE_STATE_CHANGE:
				switch (msg.arg1) {
				case BluetoothService.STATE_CONNECTED:
					mTitle.setText(R.string.title_connected_to);
					break;
				case BluetoothService.STATE_CONNECTING:
					mTitle.setText(R.string.title_connecting);
					break;
				case BluetoothService.STATE_LISTEN:
				case BluetoothService.STATE_NONE:
					mTitle.setText(R.string.title_not_connected);
					break;
				}
				break;
			case MESSAGE_DEVICE_NAME:
				// save the connected device's name
				mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
				Toast.makeText(getApplicationContext(),
						"Connected to " + mConnectedDeviceName,
						Toast.LENGTH_SHORT).show();
				break;
			case MESSAGE_TOAST:
				Toast.makeText(getApplicationContext(),
						msg.getData().getString(TOAST), Toast.LENGTH_SHORT)
						.show();
				break;
			}
		}
	};

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) {
		case REQUEST_CONNECT_DEVICE:
			// When DeviceList returns with a device to connect
			if (resultCode == Activity.RESULT_OK) {
				// Get the device MAC address
				String address = data.getExtras().getString(
						DeviceList.EXTRA_DEVICE_ADDRESS);
				// Get the BLuetoothDevice object
				BluetoothDevice device = mBluetoothAdapter
						.getRemoteDevice(address);
				// Attempt to connect to the device
				mBluetoothService.connect(device);
			}
			break;
		case REQUEST_ENABLE_BT:
			// When the request to enable Bluetooth returns
			if (resultCode == Activity.RESULT_OK) {
				mBluetoothService = new BluetoothService(this, mHandler);
				mOutStringBuffer = new StringBuffer("");
			} else {
				// User did not enable Bluetooth or an error occured
				Log.d(TAG, "BT not enabled");
				Toast.makeText(this, R.string.bt_not_enabled_leaving,
						Toast.LENGTH_SHORT).show();
				finish();
			}
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.option_menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.scan:
			// Launch the DeviceList to see devices and do scan
			Intent serverIntent = new Intent(this, DeviceList.class);
			startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
			return true;
		case R.id.discoverable:
			// Ensure this device is discoverable by others
			ensureDiscoverable();
			return true;
		}
		return false;
	}
}