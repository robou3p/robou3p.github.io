package org.lakos.lakosbot;

import android.content.Context;
import android.content.DialogInterface;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.FragmentManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

import static android.content.Context.SENSOR_SERVICE;

/**
 * Calibration Dialog has <b>it's own SensorListener</b> for accelerometer readings. When
 * calibration is complete, values are passed back to calling context (i.e. activity) with
 * CalibrationDialogListener's calibrationDialogResult method.<br/>
 * Note that if calling context does not implement CalibrationDialogListener, it will result in a
 * RuntimeException.
 */

public class CalibrationDialog extends DialogFragment {

    public interface CalibrationDialogListener {
        /**
         * @param vectors calibrated vectors array of size 5. Vectors are in following order:<br />
         *                0 - neutral vector<br />
         *                1 - forward vector<br />
         *                2 - backward vector<br />
         *                3 - left vector<br />
         *                4 - right vector<br />
         */
        void calibrationDialogResult(Vector[] vectors);
    }

    private static final int
        C_NEUTRAL = 0,
        C_FORWARD = 1,
        C_BACKWARD = 2,
        C_LEFT = 3,
        C_RIGHT = 4,
        C_FINISH = 5;

    private int currentState;
    private Vector[] vectors;

    private CalibrationDialogListener listener;

    // Accelerometer
    private SensorManager sensorManager;
    private SensorListener sensorListener;

    @BindView(R.id.textViewCalibrationDialog) TextView textViewCalibrationDialog;
    @BindView(R.id.progressBarCalibrationDialog) ProgressBar progressBarCalibrationDialog;
    @BindView(R.id.buttonCalibrationDialog) Button buttonCalibrationDialog;

    @OnClick(R.id.buttonCalibrationDialog) public void buttonCalibrationDialogOnClick() {
        switch (currentState) {
            case C_NEUTRAL:
                vectors[C_NEUTRAL] = new Vector(sensorListener.getCurrentVector());
                textViewCalibrationDialog.setText("Position FORWARD and press NEXT");
                break;
            case C_FORWARD:
                vectors[C_FORWARD] = new Vector(sensorListener.getCurrentVector().subtract(sensorListener.getNeutralVector()));
                textViewCalibrationDialog.setText("Position BACKWARD and press NEXT");
                break;
            case C_BACKWARD:
                vectors[C_BACKWARD] = new Vector(sensorListener.getCurrentVector().subtract(sensorListener.getNeutralVector()));
                textViewCalibrationDialog.setText("Position LEFT and press NEXT");
                break;
            case C_LEFT:
                vectors[C_LEFT] = new Vector(sensorListener.getCurrentVector().subtract(sensorListener.getNeutralVector()));
                textViewCalibrationDialog.setText("Position RIGHT and press NEXT");
                break;
            case C_RIGHT:
                vectors[C_RIGHT] = new Vector(sensorListener.getCurrentVector().subtract(sensorListener.getNeutralVector()));
                textViewCalibrationDialog.setText("Calibration finished");
                break;
            case C_FINISH:
                dismiss();
                break;
        }
        // move to next step of calibration
        currentState++;
        progressBarCalibrationDialog.setProgress(currentState);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        getDialog().getWindow().requestFeature(STYLE_NO_TITLE);

        View v = inflater.inflate(R.layout.dialog_calibration, container);

        ButterKnife.bind(this, v);

        sensorManager = (SensorManager) getContext().getSystemService(SENSOR_SERVICE);
        sensorListener = new SensorListener();

        currentState = C_NEUTRAL;
        vectors = new Vector[5];
        sensorListener = new SensorListener();
        textViewCalibrationDialog.setText("Position NEUTRAL and press NEXT");
        progressBarCalibrationDialog.setProgress(0);

        return v;
    }

    public void show(Context caller, FragmentManager manager) {
        super.show(manager, "CalibrationDialog");

        if (caller instanceof CalibrationDialogListener) {
            listener = (CalibrationDialogListener) caller;
        } else {
            throw new RuntimeException(caller.toString() + " must implement " + CalibrationDialogListener.class);
        }
    }

    @Override
    public void onResume() {
        // resize dialog to max width and height
        getDialog().getWindow().setLayout(WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.WRAP_CONTENT);
        sensorManager.registerListener(sensorListener, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
        sensorManager.unregisterListener(sensorListener);
    }

    @Override
    public void onDismiss(DialogInterface dialog) {
        listener.calibrationDialogResult(vectors);
        super.onDismiss(dialog);
    }
}
