package com.solutions.brewstr;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

/**
 * Created by jeremy on 2016-01-08.
 */
public class Start_Fragment extends Fragment implements View.OnClickListener {
    View rootview;
    Button startButton;
    TextView logBox;
    Bluetooth bluetooth;

    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.start_view, container, false);
        startButton = (Button) rootview.findViewById(R.id.startButton);
        startButton.setOnClickListener(this);
        logBox = (TextView) rootview.findViewById(R.id.textView2);
        return rootview;
    }

    @Override
    public void onClick(View v) {
        //Retrieve the main activity to grab the bluetooth obj for starting connection process
        MainActivity activity = (MainActivity)getActivity();
        bluetooth = activity.getBluetooth();
        if(bluetooth != null)
        {
            if(true)//add the bluetooth connection)
            {
                //successfull connection - call bluetooth thread.
            }
        }
        else{
            //unable to connect to bluetooth, don't let the user pass this page.
        }
    }
}
