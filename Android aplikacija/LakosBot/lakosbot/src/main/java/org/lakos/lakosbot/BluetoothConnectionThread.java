package org.lakos.lakosbot;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.os.ParcelUuid;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class BluetoothConnectionThread extends Thread {

    private static final String TAG = BluetoothConnectionThread.class.getSimpleName();

    public static final String
        BTC_CREATE = TAG + ".Create",
        BTC_START = TAG + ".Start",
        BTC_CLOSE = TAG + ".Close";

    private BluetoothAdapter bluetoothAdapter;
    private BluetoothDevice bluetoothDevice;
    private BluetoothSocket bluetoothSocket;

    private InputStream inputStream;
    private OutputStream outputStream;

    private LocalBroadcastManager broadcastManager;

    private Context context;
    private boolean isConnected;

    public BluetoothConnectionThread(Context context, BluetoothAdapter adapter, BluetoothDevice device) {
        this.context = context.getApplicationContext();
        bluetoothAdapter = adapter;
        bluetoothDevice = device;
        broadcastManager = LocalBroadcastManager.getInstance(context);

        try {
            // Establish a master-slave connection with the robot. If the connection succeeds,
            // also create two threads, for sending and receiving data (seperate threads prevent
            // blocking).
            ParcelUuid[] uuids = device.getUuids();
            if(uuids != null) {
                Log.d(TAG, "Got " + uuids.length + " device UUIDs");
                bluetoothSocket = device.createRfcommSocketToServiceRecord(uuids[0].getUuid());
            } else {
                Log.d(TAG, "No UUIDS given from device. Trying the well-known SPP UUID for Bluetooth boards");
                bluetoothSocket = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
            }

            broadcastManager.sendBroadcast(new Intent(BTC_CREATE));
        } catch (IOException | IndexOutOfBoundsException e) {
            Log.e(TAG, "Socket's create() method failed", e);
            broadcastManager.sendBroadcast(new Intent(BTC_CLOSE));
        }

        isConnected = false;
    }

    public void run() {
        // Cancel discovery because it otherwise slows down the connection.
        bluetoothAdapter.cancelDiscovery();
        try {
            // Connect to the remote device through the socket. This call blocks
            // until it succeeds or throws an exception.
            Log.d(TAG, "Connecting to device...");
            bluetoothSocket.connect();
        } catch (IOException connectException) {
            Log.d(TAG, "Connection failed!", connectException);
            // Unable to connect; close the socket and return.
            try {
                bluetoothSocket.close();
            } catch (IOException closeException) {
                Log.e(TAG, "Could not close the client socket", closeException);
            }
            return;
        }

        Log.d(TAG, "Connection successful!");
        isConnected = true;
        broadcastManager.sendBroadcast(new Intent(BTC_START).putExtra("name", bluetoothDevice.getName()));
        // The connection attempt succeeded. Perform work associated with
        // the connection in a separate thread.

        initializeStreams();
    }

    private void initializeStreams() {
        // Get the input and output streams; using temp objects because
        // member streams are final.
        try {
            inputStream = bluetoothSocket.getInputStream();
        } catch (IOException e) {
            Log.e(TAG, "Error occurred when creating input stream", e);
        }
        try {
            outputStream = bluetoothSocket.getOutputStream();
        } catch (IOException e) {
            Log.e(TAG, "Error occurred when creating output stream", e);
        }
    }

    public boolean isInputStreamAvailable() {
        return inputStream != null;
    }

    public boolean isOutputStreamAvailable() {
        return outputStream != null;
    }

    public boolean isConnected() {
        return isConnected;
    }

    public void write(byte[] bytes) {
        if(isOutputStreamAvailable()) {
            try {
                outputStream.write(bytes);
            } catch (IOException e) {
                Log.e(TAG, "Error occurred when sending data", e);
            }
        } else {
            Log.w(TAG, "OutputStream is not available.");
        }
    }

    public String read() {
        if(isInputStreamAvailable()) {
            try {
                StringBuilder s = new StringBuilder();
                while(inputStream.available() > 0) {
                    s.append((char) inputStream.read());
                }
                return s.toString();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    // Closes the client socket and causes the thread to finish.
    public void cancel() {
        try {
            bluetoothSocket.close();
        } catch (IOException e) {
            Log.e(TAG, "Could not close the client socket", e);
        }
        isConnected = false;
        broadcastManager.sendBroadcast(new Intent(BTC_CLOSE));
    }

}
