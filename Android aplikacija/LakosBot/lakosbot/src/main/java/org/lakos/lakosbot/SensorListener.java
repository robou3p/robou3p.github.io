package org.lakos.lakosbot;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

/**
 * Created by martin on 14.3.2018.
 */

public class SensorListener implements SensorEventListener {

    Float azimut;
    private static final int MAX_SPEED_VALUE = 100;
    private static final int SPEED_MULTIPLIER = 100;

    public SensorListener() {
        currentVector = new Vector();
        neutralVector = new Vector();
        forwardVector = new Vector();
        backwardVector = new Vector();
        leftVector = new Vector();
        rightVector = new Vector();
    }

    // Control magic: vectors
    private Vector currentVector, neutralVector, forwardVector, backwardVector, leftVector, rightVector;

    private byte availDirL, availDirR, availSpeedL, availSpeedR;

    /**
     * Returns current directions and speeds according to sensor readings.<br/>
     * Direction parameters can be:
     * <ul><li>1, meaning forward movement</li><li>0, meaning backward movement</li></ul>
     * Speed parameters can take any value between 0 and 255 (one byte)
     * @return byte array containing {dirL, speedL, dirR, speedR}
     */
    public byte[] getDirsAndSpeeds() {
        return new byte[]{availDirL, availSpeedL, availDirR, availSpeedR};
    }

    public Vector getCurrentVector() {
        return currentVector;
    }

    public Vector getNeutralVector() {
        return neutralVector;
    }

    public void setVectors(Vector neutral, Vector forward, Vector backward, Vector left, Vector right) {
        neutralVector.setValue(neutral);
        forwardVector.setValue(forward);
        backwardVector.setValue(backward);
        leftVector.setValue(left);
        rightVector.setValue(right);
    }

    private static final float ALPHA = 0.5f;

    private float[] applyLowPassFilter(float[] input, float[] output) {
        if (output == null) {
            return input;
        }

        for (int i = 0; i < input.length; i++) {
            output[i] = output[i] + ALPHA * (input[i] - output[i]);
        }

        return output;
    }

    float[] gravity;
    float[] geomagnetic;

    @Override
    public void onSensorChanged(SensorEvent event) {
        // check sensor type
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {

            /*
            TODO: ugotovit, kaj se dogaja s temi vektorji
            Kaj sta speed in steer value ???

            Kaj vem do zdaj:
            dirL / dirR se nastavita na 1 oz. 0 glede na to, ali je speedL / speedR pozitiven ali
            ne. Te vrednosti se posredujejo robotu (torej, če je dir=0, pomeni vzvratno vožnjo, če
            je dir=1, pomeni vožnjo naprej).
             */

            // assign directions
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];
            currentVector.setValue(x, y, z);

            // those are values between -1 and 1
            float forward = currentVector.getComponent(forwardVector);
            float reverse = currentVector.getComponent(backwardVector);
            float left = currentVector.getComponent(leftVector);
            float right = currentVector.getComponent(rightVector);

            float speed = (forward - reverse) * SPEED_MULTIPLIER;
            float speedValue = speed;
            //float speedValue = (float) Math.pow(Math.abs(speed), 1.20f);
            //if (speed < 0) speedValue *= -0.9;

            float steer = (right - left) * SPEED_MULTIPLIER;
            float steerValue = steer; //float steerValue = (float) Math.pow(Math.abs(steer), 1.20f);
            //if (steer < 0) steerValue *= -1;

            //Log.i("SENSOOOOOOOOOOORRR", String.format("speed: %3.3f, steer: %3.3f", speed, steer));
            //Log.i("SENSOOOOOOOOOOORRR", String.format("speedValue: %3.3f, steerValue: %3.3f", speedValue, steerValue));

            float speedR = speedValue - steerValue/10 - steerValue * speedValue / 100;
            float speedL = speedValue + steerValue/10 + steerValue * speedValue / 100;

            //Log.i("SENSOOOOOOOOOOORRR", String.format("r: %3.3f, l: %3.3f", speedR, speedL));

            // Limit speed value between +MAX and -MAX
            speedR = Math.max(Math.min(speedR, MAX_SPEED_VALUE), -MAX_SPEED_VALUE);
            speedL = Math.max(Math.min(speedL, MAX_SPEED_VALUE), -MAX_SPEED_VALUE);

            availDirL = speedL >= 0 ? (byte) 1 : (byte) 0;
            availDirR = speedR >= 0 ? (byte) 1 : (byte) 0;

            availSpeedL = (byte) Math.abs(Math.round(speedL));
            availSpeedR = (byte) Math.abs(Math.round(speedR));
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

        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            gravity = applyLowPassFilter(event.values.clone(), gravity);
        }
        if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
            geomagnetic = applyLowPassFilter(event.values.clone(), geomagnetic);
        }

        if (gravity != null && geomagnetic != null){
            float R[] = new float[9];
            float I[] = new float[9];

            boolean success = SensorManager.getRotationMatrix(R, I, gravity, geomagnetic);

            if (success) {
                float orientation[] = new float[3];
                SensorManager.getOrientation(R, orientation);
                azimut = -orientation[0];
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
