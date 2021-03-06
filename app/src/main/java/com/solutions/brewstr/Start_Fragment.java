package com.solutions.brewstr;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by jeremy on 2016-01-08.
 */
public class Start_Fragment extends Fragment implements View.OnClickListener {
    View rootview;
    ImageButton bluetoothButton;
    Button displayDataLog;
    Bluetooth bt;
    boolean connected = false;
    TextView status;
    public Map<Integer, Float> mTempVsTime = new HashMap<>();

    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.start_view, container, false);
        bluetoothButton = (ImageButton) rootview.findViewById(R.id.bluetooth_button);
        bluetoothButton.setOnClickListener(this);
        status = (TextView) rootview.findViewById(R.id.startLabel);
        return rootview;
    }

    @Override
    public void onClick(View v) {
        //Retrieve the main activity to grab the bluetooth obj for starting connection process
        MainActivity activity = (MainActivity) getActivity();
        Log.i("DEBUG","Attempting to connect to bluetooth...\n");

        try {
            int REQUEST_ENABLE_BT = 1234; //sample enable code.
            BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

            if (mBluetoothAdapter == null) {
                // Device does not support Bluetooth
                Log.i("DEBUG","No Bluetooth Support...\n");
            } else {
                if (!mBluetoothAdapter.isEnabled()) {
                    Log.i("DEBUG","Requesting Bluetooth Enable From User\n");

                    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);

                    Log.i("DEBUG","Bluetooth enabled.\n");
                }
                Log.i("DEBUG","Searching for paired devices!\n\n");

                Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
                // If there are paired devices
                if (pairedDevices.size() > 0) {
                    // Loop through paired devices
                    for (BluetoothDevice device : pairedDevices) {
                        // Add the name and address to an array adapter to show in a ListView
                        Log.i("DEBUG","Paired Device: " + ") " + device.getName() + " - " + device.getAddress() + "\n\n");
                        if (device.getName().equals("HC-06")) {
                            //We know this is the right device, grab the mac address and try to connect.
                            bt = new Bluetooth(device.getAddress(), new Handler() {
                                @Override
                                public void handleMessage(Message message) {

                                    String s = (String) message.obj;

                                    if (s.equals("CONNECTED")) {
                                        Log.i("DEBUG","Connection Status Recieved: " + s + "\n\n");
                                        connected = true;
                                        status.setText("Connected.");
                                    } else if (s.equals("DISCONNECTED")) {
                                        Log.i("DEBUG","Connection Status Recieved: " + s + "\n\n");
                                        connected = false;
                                        status.setText("Disconnected.");
                                    } else if (s.equals("CONNECTION FAILED")) {
                                        Log.i("DEBUG","Connection Status Recieved: " + s + "\n\n");
                                        connected = false;
                                        status.setText("Connection Failed.");
                                    } else {
                                        try {
                                                if(s.contains("Temperature:"))
                                                {
                                                    // This currently does not use the "temperature" or "time" key
                                                    // TODO: decide if we need the keys
                                                    Pattern pattern = Pattern.compile("[\\d*]");
                                                    Matcher matcher = pattern.matcher(s);
                                                    List<String> vals= new ArrayList<>();
                                                    while (matcher.find()) {
                                                        vals.add((matcher.group()));
                                                    }
                                                    float currentTemperature = Float.parseFloat(vals.get(0))/100;
                                                    int time = Integer.parseInt(vals.get(1));

                                                    mTempVsTime.put(time, currentTemperature);

                                                    Log.i("DEBUG","Message Recieved: " + s + "\n\n");

                                                    MainActivity activity = (MainActivity) getActivity();
                                                    String str = Float.toString(currentTemperature);
                                                    activity.updateTemp(str);// = "derp";//str;
                                                    //Status_Fragment frag = activity.status_frag;
                                                    //if(frag != null) {
                                                      //  String str = Float.toString(currentTemperature);
                                                        //frag.setCurrentTemperatureFeedback(str);
                                                    //}

                                                }

                                        }catch(Exception ex)
                                        {
                                            //do something
                                            if(ex.getMessage() != null) {
                                                Log.i("Handle Message Error", ex.getMessage());
                                            }
                                            else {
                                                // do fuck all
                                            }
                                        }

                                    }
                                }
                            });
                            bt.start();
                        }
                    }
                }
            }
        } catch (Exception ex) {
            Log.i("DEBUG","Exception Caught: " + ex.getMessage());
        }
    }
    public void sendMsgToBT(String s)
    {
        Handler writeHandler = bt.getWriteHandler();
        Message msg = new Message();
        msg.obj = s;
        writeHandler.sendMessage(msg);
    }
}
