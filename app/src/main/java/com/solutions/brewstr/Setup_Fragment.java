package com.solutions.brewstr;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

/**
 * Created by jeremy on 2016-01-08.
 */
public class Setup_Fragment extends Fragment implements View.OnClickListener {
    View rootview;

    SeekBar mashTempSB;
    SeekBar mashTimeSB;
    SeekBar boilTempSB;
    SeekBar boilTimeSB;
    SeekBar hopsTimeSB;

    TextView mashTempSP;
    TextView mashTimeSP;
    TextView boilTempSP;
    TextView boilTimeSP;
    TextView hopsTimeSP;



    Button setupButton;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.setup_view,container,false);

        //set button mapping and listeners
        setupButton = (Button) rootview.findViewById(R.id.setupButton);
        setupButton.setOnClickListener(this);

        //set seekbar mapping and listeners
        mashTempSB = (SeekBar) rootview.findViewById(R.id.mashingTempSeekBar);
        mashTempSB.setMax(25);
        mashTempSB.setOnSeekBarChangeListener(new seekBarListener());
        mashTimeSB = (SeekBar) rootview.findViewById(R.id.mashingTimeSeekBar);
        mashTimeSB.setMax(50);
        mashTimeSB.setOnSeekBarChangeListener(new seekBarListener());
        boilTempSB = (SeekBar) rootview.findViewById(R.id.boilTempSeekBar);
        boilTempSB.setOnSeekBarChangeListener(new seekBarListener());
        boilTimeSB = (SeekBar) rootview.findViewById(R.id.boilTimeSeekBar);
        boilTimeSB.setOnSeekBarChangeListener(new seekBarListener());
        hopsTimeSB = (SeekBar) rootview.findViewById(R.id.hopsTimeSeekBar);
        hopsTimeSB.setOnSeekBarChangeListener(new seekBarListener());

        //set the setpoint indicator mapping
        mashTempSP = (TextView) rootview.findViewById(R.id.mashingTempSetpoint);
        mashTimeSP = (TextView) rootview.findViewById(R.id.mashingTimeSetpoint);
        boilTempSP = (TextView) rootview.findViewById(R.id.boilTempSetpoint);
        boilTimeSP = (TextView) rootview.findViewById(R.id.boilTimeSetpoint);
        hopsTimeSP = (TextView) rootview.findViewById(R.id.hopsTimeSetpoint);
        return rootview;
    }
    @Override
    public void onClick(View v) {
        //Grab the input params, generate message for send to LL.
        String messageToLL = "";
        messageToLL += (75 + mashTempSB.getProgress());
        messageToLL += ",";
        messageToLL += (50 + mashTimeSB.getProgress());
        messageToLL += ",";
        messageToLL += boilTimeSB.getProgress();
        setupButton.setText(messageToLL);

        Bluetooth bt = ((MainActivity)getActivity()).getBluetooth();
        if(bt != null)
        {
           //We have bluetooth object - send message to LL
            bt.getWriteHandler();
        }
        else
        {
            //display error message - bluetooth not connected/activated
        }

    }
    private class seekBarListener implements SeekBar.OnSeekBarChangeListener {

        public void onProgressChanged(SeekBar seekBar, int progress,
        boolean fromUser) {
            // Log the progress
            switch(seekBar.getId()){
                case R.id.mashingTempSeekBar: {
                    int setpoint = progress + 75;
                    mashTempSP.setText("" + setpoint  + " (C)");
                    break;
                }
                case R.id.mashingTimeSeekBar: {
                    int setpoint = progress + 50;
                    mashTimeSP.setText("" + setpoint + " (min)");
                    break;
                }
                case R.id.boilTempSeekBar: {
                    int setpoint = progress + 50;
                    boilTempSP.setText("" + progress + " (C)");
                    break;
                }
                case R.id.boilTimeSeekBar: {
                    boilTimeSP.setText("" + progress + " (min)");
                    break;
                }
                case R.id.hopsTimeSeekBar:{
                    hopsTimeSP.setText("" + progress + " (min)");
                }
                default:
                    break;
            }
        }

    public void onStartTrackingTouch(SeekBar seekBar) {}

    public void onStopTrackingTouch(SeekBar seekBar) {}

}
}