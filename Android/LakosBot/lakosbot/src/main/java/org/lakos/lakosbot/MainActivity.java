package org.lakos.lakosbot;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.ColorStateList;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.AppCompatButton;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.WindowManager;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;

import butterknife.BindColor;
import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import es.dmoral.toasty.Toasty;

import static org.lakos.lakosbot.BluetoothConnectionThread.BTC_CLOSE;
import static org.lakos.lakosbot.BluetoothConnectionThread.BTC_CREATE;
import static org.lakos.lakosbot.BluetoothConnectionThread.BTC_START;

public class MainActivity extends AppCompatActivity implements CalibrationDialog.CalibrationDialogListener {

    private static final String TAG = MainActivity.class.getSimpleName();

    // Intent request codes
    private static final int REQUEST_ENABLE_BT = 10;

    // Bound views
    @BindView(R.id.buttonActivate) AppCompatButton buttonActivate;

    // Bound resources
    @BindColor(R.color.colorActivateButtonGreen) int colorActivateButtonGreen;
    @BindColor(R.color.colorActivateButtonRed) int colorActivateButtonRed;

    // Bluetooth
    private BluetoothAdapter bluetoothAdapter;

    // Accelerometer
    private SensorManager sensorManager;
    private SensorListener sensorListener;

    // Misc
    private android.support.v7.app.ActionBar activityActionBar;
    private Menu activityMenu;

    private boolean robotControlActive = false;

    // Bound actions
    @SuppressLint("RestrictedApi")
    @OnClick(R.id.buttonActivate) public void buttonActivateOnClick() {
        robotControlActive = !robotControlActive;

        if(robotControlActive) {
            // robot control is running!
            messageSenderHandler.removeCallbacks(messageSenderRunnable);
            messageSenderHandler.post(messageSenderRunnable);

            buttonActivate.setSupportBackgroundTintList(ColorStateList.valueOf(colorActivateButtonRed));
            buttonActivate.setText("Stop robot control");
        } else {
            // robot control has stopped!
            messageSenderHandler.removeCallbacks(messageSenderRunnable);

            buttonActivate.setSupportBackgroundTintList(ColorStateList.valueOf(colorActivateButtonGreen));
            buttonActivate.setText("Start robot control");
        }
    }

    // Prepare handler and runnable for async message sending over bluetooth
    Handler messageSenderHandler;
    Runnable messageSenderRunnable = new Runnable() {
        @Override
        public void run() {
            if(sensorListener == null) {
                Log.w(TAG, "SensorListener is null!");
                return;
            }
            if(btConnectionThread == null) {
                Log.w(TAG, "BluetoothConnectionThread is null!");
                return;
            }
            if(robotControlActive) {
                float[] speeds = sensorListener.getSpeeds();

                // dirX=1 -> forward
                // dirX=0 -> backward

                byte msg[] = {
                        'L',
                        (byte) (speeds[0] >= 0 ? 1 : 0), // dirL
                        (byte) speeds[0],
                        'R',
                        (byte) (speeds[1] >= 0 ? 1 : 0), // dirR
                        (byte) speeds[1],
                };

                Log.i(TAG, "Sending message: " + Arrays.toString(msg));

                btConnectionThread.write(msg);
            }
            messageSenderHandler.postDelayed(messageSenderRunnable, 100);
        }
    };

    // Prepare a broadcast listener for bluetooth connection state changes
    BroadcastReceiver btStateBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(BTC_CLOSE.equals(intent.getAction())) {
                setActionBarSubtitle("Not connected");
                activityMenu.getItem(0).setTitle("Connect to robot...");
            }
            else if(BTC_CREATE.equals(intent.getAction())) {
                setActionBarSubtitle("Connecting...");
            }
            else if(BTC_START.equals(intent.getAction())) {
                setActionBarSubtitle("Connected to: " + intent.getStringExtra("name"));
                activityMenu.getItem(0).setTitle("Disconnect from " + intent.getStringExtra("name"));
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Bind views
        ButterKnife.bind(this);

        // Set up the custom title
        activityActionBar = getSupportActionBar();
        if(activityActionBar == null) {
            Log.w(TAG, "Activity's Action bar is null!");
        } else {
            activityActionBar.setTitle("LakosBot");
            setActionBarSubtitle("Not connected");
        }

        // Initialize and ask for bluetooth
        // Initialize accelerometer sensor
        initBluetooth();
        initAccelerometer();

        // Initialize a handler with seperate thread that will be sending messages to robot
        messageSenderHandler = new Handler();

        // Keep screen awake
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onResume() {
        super.onResume();
        IntentFilter f = new IntentFilter();
        f.addAction(BTC_CREATE);
        f.addAction(BTC_CLOSE);
        f.addAction(BTC_START);
        LocalBroadcastManager.getInstance(this).registerReceiver(btStateBroadcastReceiver, f);
        initUI();
    }

    @Override
    protected void onPause() {
        super.onPause();
        LocalBroadcastManager.getInstance(this).unregisterReceiver(btStateBroadcastReceiver);
        messageSenderHandler.removeCallbacks(messageSenderRunnable);
        stopBluetoothDiscovery();
        robotControlActive = false;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        sensorManager.unregisterListener(sensorListener);
        btConnectionThread.cancel();
        bluetoothAdapter.disable();
    }

    private void setActionBarSubtitle(String subtitle) {
        if(activityActionBar != null) {
            activityActionBar.setSubtitle(subtitle);
        }
    }

    private void initBluetooth() {
        // Get local Bluetooth adapter
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // If the adapter is null, then Bluetooth is not supported
        if (bluetoothAdapter == null) {
            Toasty.error(this, "Bluetooth is not available!").show();
            finish();
        }

        // If BT is not on, request that it be enabled.
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(
                    BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        }
    }

    private void initAccelerometer() {
        // Initialize and register sensor listener
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        sensorListener = new SensorListener();
        sensorManager.registerListener(sensorListener, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
    }

    @SuppressLint("RestrictedApi")
    private void initUI() {
        // Initialize button
        if(robotControlActive) {
            buttonActivate.setSupportBackgroundTintList(ColorStateList.valueOf(colorActivateButtonRed));
            buttonActivate.setText("Stop robot control");
        } else {
            buttonActivate.setSupportBackgroundTintList(ColorStateList.valueOf(colorActivateButtonGreen));
            buttonActivate.setText("Start robot control");
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                if (resultCode == Activity.RESULT_OK) {
                    Toasty.normal(this, "Bluetooth enabled. Use \"Connect to robot...\" from the menu to start.", Toast.LENGTH_LONG).show();
                } else {
                    // User did not enable Bluetooth or an error occured
                    Toasty.error(this, "Bluetooth was not enabled!").show();
                    finish();
                }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        activityMenu = menu;
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.connect:
                if(bluetoothAdapter == null || !bluetoothAdapter.isEnabled()) {
                    initBluetooth();
                } else {
                    if(btConnectionThread == null || !btConnectionThread.isConnected()) {
                        showBluetoothDevicesDialog();
                    } else {
                        btConnectionThread.cancel();
                    }
                }
                return true;
            case R.id.calibrate:
                startPhoneCalibration();
                return true;
        }
        return false;
    }

    private List<BluetoothDevice> deviceList;
    private BluetoothDeviceListAdapter deviceListAdapter;
    private BluetoothConnectionThread btConnectionThread;

    private void showBluetoothDevicesDialog() {
        startBluetoothDiscovery();
        // Get list of devices to display
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        deviceList = new ArrayList<>(pairedDevices.size());
        if (pairedDevices.size() > 0) {
            // There are paired devices. Get the name and address of each paired device.
            for (BluetoothDevice device : pairedDevices) {
                deviceList.add(device);
                Log.d(TAG, String.format("Paired device: [%s] %s", device.getAddress(), device.getName()));
            }
        }

        deviceListAdapter = new BluetoothDeviceListAdapter(this, deviceList);

        // Prepare and show a dialog
        AlertDialog.Builder dialogBuilder = new AlertDialog.Builder(this);

        dialogBuilder
                .setTitle("Select a device")
                .setIcon(R.drawable.ic_bluetooth_searching_black);

        dialogBuilder.setSingleChoiceItems(deviceListAdapter, -1, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                deviceListAdapter.setSelectedPosition(which);
            }
        });

        dialogBuilder
                .setPositiveButton("Connect", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        int pos = deviceListAdapter.getSelectedPosition();
                        // connect to selected item
                        if(pos >= 0 && pos < deviceList.size()) {
                            BluetoothDevice d = deviceListAdapter.getItem(pos);
                            connectToBluetoothDevice(d);
                        } else {
                            Toasty.warning(MainActivity.this, "No device selected.").show();
                        }
                    }
                })
                .setNegativeButton("Cancel", null)
                .setOnDismissListener(new DialogInterface.OnDismissListener() {
                    @Override
                    public void onDismiss(DialogInterface dialog) {
                        stopBluetoothDiscovery();
                    }
                });

        Toasty.info(this, "Newly found devices will appear at the bottom of the list.").show();
        dialogBuilder.show();
    }

    private void startBluetoothDiscovery() {
        if(!bluetoothAdapter.isDiscovering()) {
            if(bluetoothAdapter.startDiscovery()) {
                Log.d(TAG, "Device discovery started.");
            } else {
                Log.w(TAG, "Device discovery failed to start.");
            }
        } else {
            Log.v(TAG, "Device discovery already running!");
        }

    }

    private void stopBluetoothDiscovery() {
        if(bluetoothAdapter.isDiscovering()) {
            bluetoothAdapter.cancelDiscovery();
            Log.d(TAG, "Device discovery stopped.");
        }
        Log.v(TAG, "Device discovery was not running.");
    }

    private void connectToBluetoothDevice(final BluetoothDevice device) {
        // if there's an existing thread, cancel it.
        if(btConnectionThread != null && btConnectionThread.isAlive()) btConnectionThread.cancel();

        // start a new thread with the selected device
        if(btConnectionThread == null || !btConnectionThread.isAlive()) {
            btConnectionThread = new BluetoothConnectionThread(this, bluetoothAdapter, device);
            btConnectionThread.start();
        } else {
            Toasty.info(this, "Connection already established.").show();
        }
    }

    private void startPhoneCalibration() {
        // disable robot control while calibrating ...
        robotControlActive = false;

        // display calibration dialog
        CalibrationDialog dialog = new CalibrationDialog();
        dialog.show(this, getSupportFragmentManager());
    }

    @Override
    public void calibrationDialogResult(Vector[] vectors) {
        Log.d(TAG, "Calibration vectors received...");
        for(int i=0; i<5; i++) {
            if(vectors[i] == null) {
                Log.d(TAG, "Calibration was not completed!");
                Toasty.warning(this, "Calibration was cancelled!").show();
                return;
            }
        }
        Log.d(TAG, "Calibration vectors applied to sensor listener!");
        sensorListener.setVectors(vectors[0], vectors[1], vectors[2], vectors[3], vectors[4]);
    }
}
