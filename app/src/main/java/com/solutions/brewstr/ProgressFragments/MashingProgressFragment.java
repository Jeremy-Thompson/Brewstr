package com.solutions.brewstr.ProgressFragments;

import android.app.Fragment;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.solutions.brewstr.DonutProgressBar;
import com.solutions.brewstr.R;

/**
 * Created by Derek on 2016-01-09.
 */
public class MashingProgressFragment extends android.support.v4.app.Fragment {
    View rootview;
    DonutProgressBar cPB;
    TextView mashTemp;
    TextView gristAmount;
    TextView timeRemaining;
    String mBatchName = "";
    Bundle bundle = new Bundle();



    int temp = 1545;
    int amount = 140;
    int time = 36;

    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.mash_prog_view, container, false);
        cPB = (DonutProgressBar) rootview.findViewById(R.id.donut_prog);
        cPB.setMax(300000/1000);
        mashTemp = (TextView) rootview.findViewById(R.id.mashing_temp);
        gristAmount = (TextView) rootview.findViewById(R.id.grist_amount);
        timeRemaining = (TextView) rootview.findViewById(R.id.time_remaining);
        initializeParameters();

        new CountDownTimer(300000, 1000) {
            public void onTick(long millisUntilFinished) {
                cPB.setProgress(Math.round(millisUntilFinished)/1000);
                cPB.invalidate();
            }

            public void onFinish() {

            }
        }.start();

        mashTemp.setText(mBatchName + temp + "" + " degrees Celsius");
        gristAmount.setText("Grist Amount:  " + amount + "" + " grams");
        timeRemaining.setText("Time Remaining:  " + time + "" + " seconds");
        return rootview;
    }

    public void initializeParameters() {
        bundle = getArguments();
        mBatchName = bundle.getString("batch");
    }
}
