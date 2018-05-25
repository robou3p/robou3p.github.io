package org.lakos.lakosbot;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

public class SensorConfigurationLoader {

    private static final int VECTOR_COUNT = 5;

    private static final String TAG = SensorConfigurationLoader.class.getSimpleName();

    public static void saveCalibration(Context context, Vector[] vectors) {
        context = context.getApplicationContext();

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        SharedPreferences.Editor prefsEdit = prefs.edit();

        for(int i = 0; i < VECTOR_COUNT; i++) {
            prefsEdit.putFloat(i + "x", vectors[i].x);
            prefsEdit.putFloat(i + "y", vectors[i].y);
            prefsEdit.putFloat(i + "z", vectors[i].z);
        }

        prefsEdit.apply();
        Log.d(TAG, "Calibration vectors saved!");
    }

    public static Vector[] loadCalibration(Context context, SensorListener sensorListener) {

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

        Vector[] vectors = new Vector[5];

        for(int i = 0; i < VECTOR_COUNT; i++) {
            vectors[i] = new Vector(
                    prefs.getFloat(i + "x", 0),
                    prefs.getFloat(i + "y", 0),
                    prefs.getFloat(i + "z", 0)
            );
        }

        sensorListener.setVectors(vectors[0], vectors[1], vectors[2], vectors[3], vectors[4]);
        Log.d(TAG, "Calibration vectors loaded!");

        return vectors;
    }
}
