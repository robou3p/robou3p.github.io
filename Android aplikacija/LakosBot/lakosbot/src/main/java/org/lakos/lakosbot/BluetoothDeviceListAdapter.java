package org.lakos.lakosbot;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.RadioButton;
import android.widget.TextView;

import java.util.List;

/**
 * Created by martin on 15.3.2018.
 */

public class BluetoothDeviceListAdapter extends BaseAdapter {

    private List<BluetoothDevice> devices;
    private Context context;
    private int selectedPosition = -1;

    public BluetoothDeviceListAdapter(Context context, List<BluetoothDevice> devices) {
        this.devices = devices;
        this.context = context;
    }

    public void setSelectedPosition(int position) {
        selectedPosition = position;
        notifyDataSetChanged();
    }

    public int getSelectedPosition() {
        return selectedPosition;
    }

    @Override
    public int getCount() {
        return devices.size();
    }

    @Override
    public BluetoothDevice getItem(int position) {
        return devices.get(position);
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // inflate the layout for each list row
        if (convertView == null) {
            convertView = LayoutInflater.from(context).inflate(R.layout.list_item_device, parent, false);
        }

        // get current item to be displayed
        BluetoothDevice bluetoothDevice = getItem(position);

        // set view for the current item
        TextView textViewDeviceName = convertView.findViewById(R.id.textViewName);
        TextView textViewDeviceAddress = convertView.findViewById(R.id.textViewMAC);
        RadioButton radioButtonSelected = convertView.findViewById(R.id.radioButtonSelected);

        if(position == selectedPosition) radioButtonSelected.setChecked(true);
        else radioButtonSelected.setChecked(false);

        textViewDeviceName.setText(bluetoothDevice.getName());
        textViewDeviceAddress.setText(bluetoothDevice.getAddress());

        // returns the view for the current row
        return convertView;
    }
}
