package com.solutions.brewstr;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.net.wifi.WifiConfiguration;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import org.w3c.dom.Text;

public class Status_Fragment extends Fragment {
    View rootview;
    int mTimer;
    int mTemperaturesp;

    private TextView mStageName;
    private TextView mCurrentTemperatureSetpoint;
    private TextView mCurrentTemperatureFeedback;
    private TextView mBatchVolume;
    private TextView mBeerType;
    private DonutProgressBar cPB;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.control_panel, container, false);
        mStageName = (TextView) rootview.findViewById(R.id.stage_name);
        mCurrentTemperatureSetpoint = (TextView) rootview.findViewById(R.id.current_temp);
        mCurrentTemperatureFeedback = (TextView) rootview.findViewById(R.id.current_te);
        mBatchVolume = (TextView) rootview.findViewById(R.id.volume);
        mBeerType = (TextView) rootview.findViewById(R.id.beer_type);
        cPB = (DonutProgressBar) rootview.findViewById(R.id.stage_time_remaining);

        Bundle bundle = this.getArguments();
        if(bundle != null) {
            mTimer = bundle.getInt("MashTime");
            cPB.setMax(mTimer*60000);
            mTemperaturesp = bundle.getInt("MashTemp");
            mCurrentTemperatureSetpoint.setText(Integer.toString(mTemperaturesp) + " C");
        }

        new CountDownTimer(mTimer*60000, 1000) {
            public void onTick(long millisUntilFinished) {
                cPB.setProgress(Math.round(millisUntilFinished)/1000);
                cPB.invalidate();
                String temperatureFeedback = ((MainActivity) getActivity()).test;
                mCurrentTemperatureFeedback.setText(temperatureFeedback);
            }

            public void onFinish() {

            }
        }.start();
        return rootview;
    }
    public void setStageName(String str)
    {
        if(mStageName != null)
        {
            mStageName.setText(str);
        }
    }
    public void setCurrentTemperatureSetpoint(String str)
    {
        if(mCurrentTemperatureSetpoint != null)
        {
            mCurrentTemperatureSetpoint.setText(str);
        }
    }
    public void setCurrentTemperatureFeedback(String str)
    {
        if(mCurrentTemperatureFeedback != null)
        {
            mCurrentTemperatureFeedback.setText(str);
        }
    }
    public void setBatchVolume(String str)
    {
        if(mBatchVolume != null)
        {
            mBatchVolume.setText(str);
        }
    }
    public void setBeerType(String str)
    {
        if(mBeerType != null)
        {
            mBeerType.setText(str);
        }
    }
    public void getStatusFrag()
    {

    }

}
