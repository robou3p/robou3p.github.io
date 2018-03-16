package org.lakos.lakosbot;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.widget.ArrayAdapter;

/**
 * Created by martin on 14.3.2018.
 */

public class SensorListener implements SensorEventListener {

    // Control magic: vectors
    Vector currentVector = new Vector(0, 0, 0);
    Vector neutralVector = new Vector(0, 0, 0);
    Vector forwardVector = new Vector(0, 0, 0);
    Vector reverseVector = new Vector(0, 0, 0);
    Vector leftVector = new Vector(0, 0, 0);
    Vector rightVector = new Vector(0, 0, 0);

    private byte dirL = 0;
    private byte dirR = 0;
    private float speedL = 0;
    private float speedR = 0;

    public float[] getSpeeds() {
        return new float[]{speedL, speedR};
    }

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

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

}
