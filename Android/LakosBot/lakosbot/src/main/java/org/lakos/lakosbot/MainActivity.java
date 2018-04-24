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
import android.content.pm.ActivityInfo;
import android.content.res.ColorStateList;
import android.content.res.Configuration;
import android.graphics.drawable.Drawable;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.AppCompatButton;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.jmedeisis.bugstick.Joystick;
import com.jmedeisis.bugstick.JoystickListener;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;
import java.util.Set;

import butterknife.BindColor;
import butterknife.BindDrawable;
import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import es.dmoral.toasty.Toasty;
import me.toptas.fancyshowcase.FancyShowCaseQueue;
import me.toptas.fancyshowcase.FancyShowCaseView;
import me.toptas.fancyshowcase.FocusShape;

import static org.lakos.lakosbot.BluetoothConnectionThread.BTC_CLOSE;
import static org.lakos.lakosbot.BluetoothConnectionThread.BTC_CREATE;
import static org.lakos.lakosbot.BluetoothConnectionThread.BTC_START;

public class MainActivity extends AppCompatActivity implements CalibrationDialog.CalibrationDialogListener {

    private static final String TAG = MainActivity.class.getSimpleName();

    // Intent request codes
    private static final int REQUEST_ENABLE_BT = 10;

    // Bound views
    @BindView(R.id.buttonActivate) AppCompatButton buttonActivate;
    @BindView(R.id.textViewTest) TextView textViewTest;
    //@BindView(R.id.compassView) CompassView compassView;
    @BindView(R.id.joystick) Joystick joystick;
    @BindView(R.id.joystickButton) Button joystickButton;

    // Bound resources
    @BindColor(R.color.colorActivateButtonGreen) int colorActivateButtonGreen;
    @BindColor(R.color.colorActivateButtonRed) int colorActivateButtonRed;
    @BindColor(R.color.colorShowcaseBackground) int colorShowcaseBackground;
    @BindColor(R.color.colorShowcaseBorder) int colorShowcaseBorder;

    @BindDrawable(R.drawable.joystick_button_disabled) Drawable drawableJoystickButtonDisabled;
    @BindDrawable(R.drawable.joystick_button_enabled) Drawable drawableJoystickButtonEnabled;

    // Bluetooth
    private BluetoothAdapter bluetoothAdapter;

    // Accelerometer
    private SensorManager sensorManager;
    private SensorListener sensorListener;
    Sensor accelerometer;
    Sensor magnetometer;

    // Misc
    private ActionBar activityActionBar;
    private Menu activityMenu;

    private boolean robotControlActive = false;

    // Bound actions
    @OnClick(R.id.buttonActivate) public void buttonActivateOnClick() {
        robotControlActive = !robotControlActive;

        buttonActivateUpdate();
    }

    @SuppressLint("RestrictedApi")
    private void buttonActivateUpdate() {
        if(robotControlActive) {
            // robot control with accelerometer is running!
            messageSenderHandler.removeCallbacks(messageSenderRunnable);
            messageSenderHandler.post(messageSenderRunnable);

            buttonActivate.setSupportBackgroundTintList(ColorStateList.valueOf(colorActivateButtonRed));
            buttonActivate.setText("Stop robot control");

            // disable joystick manual control
            joystick.setEnabled(false);
            joystick.setJoystickButtonPosition(-0.75f, -0.25f); // test
            joystickButton.setBackground(drawableJoystickButtonDisabled);
        } else {
            // robot control with accelerometer has stopped!
            messageSenderHandler.removeCallbacks(messageSenderRunnable);

            buttonActivate.setSupportBackgroundTintList(ColorStateList.valueOf(colorActivateButtonGreen));
            buttonActivate.setText("Start robot control");

            // enable joystick manual control
            joystick.setEnabled(true);
            joystickButton.setBackground(drawableJoystickButtonEnabled);
        }
    }

    // Prepare handler and runnable for async message sending over bluetooth
    Handler messageSenderHandler;
    Runnable messageSenderRunnable = new Runnable() {
        @Override
        public void run() {
            if(sensorListener == null) {
                Log.w(TAG, "SensorListener is null!");
                robotControlActive = false;
                initUI();
                return;
            }

            byte[] data = sensorListener.getDirsAndSpeeds();

            sendData(data);

            messageSenderHandler.postDelayed(messageSenderRunnable, 100);
        }
    };

    /**
     * @param data should have 4 elements, in order: directionLeft, speedLeft, directionR, speedR
     */
    public void sendData(byte[] data) {
        String dirLstr = data[0] == 0 ? "back" : "forw";
        String dirRstr = data[2] == 0 ? "back" : "forw";
        textViewTest.setText(String.format(Locale.ENGLISH, "L: %3d %s\nD: %3d %s", data[1], dirLstr, data[3], dirRstr));

        if(btConnectionThread != null && robotControlActive) {
            byte msg[] = {
                    'L',
                    data[0], // dirL
                    data[1], // speedL
                    'R',
                    data[2], // dirR
                    data[3], // speedR
            };
            Log.i(TAG, "Sending message: " + Arrays.toString(msg));
            btConnectionThread.write(msg);
        } else {
            Log.w(TAG, "BluetoothConnectionThread is null!");
        }
    }

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

        // Bind views defined with @Bind* annotations
        ButterKnife.bind(this);

        // dodajanje senzorjev
        //wrapper();
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        magnetometer = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);

        // Set up the custom title
        activityActionBar = getSupportActionBar();
        if(activityActionBar == null) {
            Log.w(TAG, "Activity's Action bar is null!");
        } else {
            activityActionBar.setTitle("LakosBot");
            setActionBarSubtitle("Not connected");
        }

        textViewTest.setText("L:\nR:");

        // Initialize and ask for bluetooth
        // Initialize accelerometer sensor
        //initBluetooth();

        initAccelerometer();

        initMagnetometer();

        initJoystick();

        initShowcase();

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
        SensorConfigurationLoader.loadCalibration(this, sensorListener);

        // accelerometer, magnetometer
        sensorManager.registerListener(sensorListener, accelerometer, SensorManager.SENSOR_DELAY_UI);
        sensorManager.registerListener(sensorListener, magnetometer, SensorManager.SENSOR_DELAY_UI);
    }

    @Override
    protected void onPause() {
        super.onPause();
        LocalBroadcastManager.getInstance(this).unregisterReceiver(btStateBroadcastReceiver);
        messageSenderHandler.removeCallbacks(messageSenderRunnable);
        stopBluetoothDiscovery();
        sensorManager.unregisterListener(sensorListener);
        robotControlActive = false;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        sensorManager.unregisterListener(sensorListener);
        if(btConnectionThread != null) btConnectionThread.cancel();
        if(bluetoothAdapter != null) bluetoothAdapter.disable();
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
            Toasty.error(this, "Bluetooth is not available on this device.").show();
            //finish();
            return;
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

    private void initMagnetometer() {
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        //sensorListener = new SensorListener();
        //sensorManager.registerListener(sensorListener, sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD), SensorManager.SENSOR_DELAY_NORMAL);
    }

    private void initUI() {
        // Initialize button
        buttonActivateUpdate();
    }

    private void initJoystick() {
        joystick.setEnabled(false);
        joystick.setTouchSlop(0);
        joystick.setJoystickListener(new JoystickListener() {

            float wheelL, wheelR = 0;
            float offset;

            @Override
            public void onDown() {
                // ..
            }

            @Override
            public void onDrag(float degrees, float offset) {
                this.offset = offset;
                /*
                 degrees are values from (-180) to (180) - starting with 0 on the right and ending
                 with 180 on the left. Upper half is positive numbers, lower half is negative numb.

                 offset are values from (0) to (1) - distance from center position of the joystick
                 button
                 */
                final int BACKWARD = 0, FORWARD = 1;
                final int LEFT = 0, RIGHT = 1;
                final int MODE1 = 0, MODE2 = 1;

                int direction;
                if(degrees < 0) direction = BACKWARD;
                else direction = FORWARD;
                degrees = Math.abs(degrees);

                int firstWheel;
                if(degrees > 90) firstWheel = LEFT;
                else firstWheel = RIGHT;

                if(degrees > 90) degrees = degrees - ( (degrees - 90) * 2 );
                float normalizedDegrees = degrees / 45; // [0, 2]

                int mode = MODE2;
                if(mode == MODE1) {
                    // MODE 1 (full right makes left wheel full forward and right wheel full backward - rotation in place)
                    if (firstWheel == LEFT) {
                        wheelL = -1 + normalizedDegrees;
                        wheelR = +1;
                    } else {
                        wheelL = +1;
                        wheelR = -1 + normalizedDegrees;
                    }
                } else if(mode == MODE2) {
                    // MODE 2 (full right makes left wheel full forward and right wheel with no movement - rotation around stopped wheel)
                    if (firstWheel == LEFT) {
                        wheelL = normalizedDegrees / 2;
                        wheelR = +1;
                    } else {
                        wheelL = +1;
                        wheelR = normalizedDegrees / 2;
                    }
                } else {
                    wheelL = wheelR = 0;
                }

                if(direction == FORWARD) {
                    ;
                } else {
                    wheelL *= -1;
                    wheelR *= -1;
                }

                byte wheelLDirection = (byte) (wheelL >= 0 ? 1 : 0);
                byte wheelRDirection = (byte) (wheelR >= 0 ? 1 : 0);

                byte wheelLFinal = (byte) Math.abs(Math.round(wheelL*100*Math.pow(offset, 1.6)));
                byte wheelRFinal = (byte) Math.abs(Math.round(wheelR*100*Math.pow(offset, 1.6)));

                byte[] data = {wheelLDirection, wheelLFinal, wheelRDirection, wheelRFinal};

                sendData(data);

            }

            @Override
            public void onUp() {
                wheelL = wheelR = 0;
                if(btConnectionThread != null) {
                    byte msg[] = {
                            'L',
                            0, // dirL
                            0, // speedL
                            'R',
                            0, // dirR
                            0, // speedR
                    };
                    Log.i(TAG, "Sending message: " + Arrays.toString(msg));
                    btConnectionThread.write(msg);
                } else {
                    Log.w(TAG, "BluetoothConnectionThread is null!");
                }
                //textViewTest.setText(String.format(Locale.ENGLISH, "L:%d\nD:%d", Math.round(wheelL*100*Math.pow(offset, 2)), Math.round(wheelR*100*Math.pow(offset,2 ))));
            }
        });
    }

    private void initShowcase() {

        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);

        int width = metrics.widthPixels;
        int height = metrics.heightPixels;

        FancyShowCaseView sc1 = new FancyShowCaseView.Builder(this)
                .focusRectAtPosition(200, 100, 800, 250)
                .backgroundColor(colorShowcaseBackground)
                .focusBorderColor(colorShowcaseBorder)
                .focusBorderSize(10)
                .customView(R.layout.showcase_1_bt, null)
                .showOnce("sc1")
                .build();

        FancyShowCaseView sc2 = new FancyShowCaseView.Builder(this)
                .focusCircleAtPosition(width - 75, 75, 100)
                .backgroundColor(colorShowcaseBackground)
                .focusBorderColor(colorShowcaseBorder)
                .focusBorderSize(10)
                .customView(R.layout.showcase_2_menu, null)
                .showOnce("sc2")
                .build();

        FancyShowCaseView sc3 = new FancyShowCaseView.Builder(this)
                .focusOn(joystick)
                .backgroundColor(colorShowcaseBackground)
                .focusBorderColor(colorShowcaseBorder)
                .focusBorderSize(10)
                .customView(R.layout.showcase_3_joystick, null)
                .showOnce("sc3")
                .build();

        FancyShowCaseView sc4 = new FancyShowCaseView.Builder(this)
                .focusOn(buttonActivate)
                .focusShape(FocusShape.ROUNDED_RECTANGLE)
                .backgroundColor(colorShowcaseBackground)
                .focusBorderColor(colorShowcaseBorder)
                .focusBorderSize(10)
                .customView(R.layout.showcase_4_sensor, null)
                .showOnce("sc4")
                .build();

        FancyShowCaseQueue fq = new FancyShowCaseQueue()
                .add(sc1)
                .add(sc2)
                .add(sc3)
                .add(sc4);

        fq.show();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                if (resultCode == Activity.RESULT_OK) {
                    Toasty.normal(this, "Bluetooth je vključen. V meniju izberi \"Poveži se z robotom...\" za začetek.", Toast.LENGTH_LONG).show();
                } else {
                    // User did not enable Bluetooth or an error occured
                    Toasty.error(this, "Bluetooth ni bil vključen!").show();
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
            case R.id.rotate:
                if(getRequestedOrientation() != ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE)
                    setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
                else
                    setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
                return true;
            case R.id.intro:
                FancyShowCaseView.resetAllShowOnce(this);
                initShowcase();
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
                .setTitle("Izberi robota")
                .setIcon(R.drawable.ic_bluetooth_searching_black);

        dialogBuilder.setSingleChoiceItems(deviceListAdapter, -1, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                deviceListAdapter.setSelectedPosition(which);
            }
        });

        dialogBuilder
                .setPositiveButton("Poveži", new DialogInterface.OnClickListener() {
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
                .setNegativeButton("Prekini", null)
                .setOnDismissListener(new DialogInterface.OnDismissListener() {
                    @Override
                    public void onDismiss(DialogInterface dialog) {
                        stopBluetoothDiscovery();
                    }
                });

        Toasty.info(this, "Novo najdene naprave se bodo pokazale na dnu seznama.").show();
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
        if(bluetoothAdapter != null && bluetoothAdapter.isDiscovering()) {
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
            Toasty.info(this, "Povezava je že vzpostavljena.").show();
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
                Toasty.warning(this, "Kalibracija je bila prekinjena!").show();
                return;
            }
        }
        sensorListener.setVectors(vectors[0], vectors[1], vectors[2], vectors[3], vectors[4]);
        Log.d(TAG, "Calibration vectors applied to sensor listener!");

        SensorConfigurationLoader.saveCalibration(this, vectors);
    }
}
