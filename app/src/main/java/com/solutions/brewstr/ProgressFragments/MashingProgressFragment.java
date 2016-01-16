package com.solutions.brewstr.ProgressFragments;

import android.app.Fragment;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.solutions.brewstr.R;

/**
 * Created by Derek on 2016-01-09.
 */
public class MashingProgressFragment extends android.support.v4.app.Fragment {
    View rootview;
    TextView mashTemp;
    TextView gristAmount;
    TextView timeRemaining;

    int temp = 170;
    int amount = 100;
    int time = 30;

    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.mash_prog_view, container, false);
        mashTemp = (TextView) rootview.findViewById(R.id.mashing_temp);
        gristAmount = (TextView) rootview.findViewById(R.id.grist_amount);
        timeRemaining = (TextView) rootview.findViewById(R.id.time_remaining);


        mashTemp.setText("Mashing Temperature:  " + temp + "" + " degrees Celsius");
        gristAmount.setText("Grist Amount:  " + amount + "" + " grams");
        timeRemaining.setText("Time Remaining:  " + time + "" + " seconds");
        return rootview;
    }
}
