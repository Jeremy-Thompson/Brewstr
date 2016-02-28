package com.solutions.brewstr;

import android.app.Activity;
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
public class Setup_Fragment extends Fragment  {

    //app constants
    static int MASHTEMPOFFSET = 67;
    static int MASHTIMEOFFSET = 50;
    static int BOILTEMPOFFSET = 50;
    static int BOILTIMEOFFSET = 50;

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
        setupButton.setOnClickListener(new View.OnClickListener() {
                                           @Override
                                           public void onClick(View v) {
                                               if(((MainActivity) getActivity()).start_frag.connected){
                                                   //Grab the input params, generate message for send to LL.
                                                   String messageToLL = "brew,";
                                                   messageToLL += (MASHTEMPOFFSET + mashTempSB.getProgress());
                                                   messageToLL += ",";
                                                   messageToLL += (MASHTIMEOFFSET + mashTimeSB.getProgress());
                                                   messageToLL += ",";
                                                   messageToLL += (BOILTEMPOFFSET + boilTempSB.getProgress());
                                                   messageToLL += ",";
                                                   messageToLL += (BOILTIMEOFFSET + boilTimeSB.getProgress());
                                                   messageToLL += ",";
                                                   messageToLL += (hopsTimeSB.getProgress());

                                                   ((MainActivity) getActivity()).start_frag.sendMsgToBT(messageToLL);
                                                   Status_Fragment status_fragment = new Status_Fragment();
                                                   ((MainActivity) getActivity()).fragmentReplace(status_fragment);
                                               }
                                               else{
                                                   //display error message that the device is disconnected - return to start page
                                               }
                                           }
                                       });

        //Set mashtemp seekbar
        mashTempSB = (SeekBar) rootview.findViewById(R.id.mashingTempSeekBar);
        mashTempSB.setMax(25);
        mashTempSB.setOnSeekBarChangeListener(new seekBarListener());
        // Set mashtime seekbar
        mashTimeSB = (SeekBar) rootview.findViewById(R.id.mashingTimeSeekBar);
        mashTimeSB.setMax(50);
        mashTimeSB.setOnSeekBarChangeListener(new seekBarListener());
        // Set boiltemp seekbar
        boilTempSB = (SeekBar) rootview.findViewById(R.id.boilTempSeekBar);
        boilTempSB.setOnSeekBarChangeListener(new seekBarListener());
        // Set boiltime seekbar
        boilTimeSB = (SeekBar) rootview.findViewById(R.id.boilTimeSeekBar);
        boilTimeSB.setOnSeekBarChangeListener(new seekBarListener());
        // Set hopstime seekbar
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
    private class seekBarListener implements SeekBar.OnSeekBarChangeListener {

        public void onProgressChanged(SeekBar seekBar, int progress,
        boolean fromUser) {
            // Log the progress
            switch(seekBar.getId()){
                case R.id.mashingTempSeekBar: {
                    int setpoint = progress + MASHTEMPOFFSET;
                    mashTempSP.setText("" + setpoint  + " (C)");
                    break;
                }
                case R.id.mashingTimeSeekBar: {
                    int setpoint = progress + MASHTIMEOFFSET;
                    mashTimeSP.setText("" + setpoint + " (min)");
                    break;
                }
                case R.id.boilTempSeekBar: {
                    int setpoint = progress + BOILTEMPOFFSET;
                    boilTempSP.setText("" + setpoint + " (C)");
                    break;
                }
                case R.id.boilTimeSeekBar: {
                    int setpoint = progress + BOILTIMEOFFSET;
                    boilTimeSP.setText("" + setpoint + " (min)");
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
    public TextView getTextView()
    {
        return setupButton;
    }
}