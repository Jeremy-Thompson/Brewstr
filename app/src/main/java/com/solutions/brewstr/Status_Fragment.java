package com.solutions.brewstr;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
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

    TextView mStageName;
    TextView mCurrentTemperatureSetpoint;
    TextView mCurrentTemperatureFeedback;
    TextView mBatchVolume;
    TextView mBeerType;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.control_panel, container, false);
        mStageName = (TextView) rootview.findViewById(R.id.stage_name);
        mCurrentTemperatureSetpoint = (TextView) rootview.findViewById(R.id.current_temp);
        mCurrentTemperatureFeedback = (TextView) rootview.findViewById(R.id.current_te);
        mBatchVolume = (TextView) rootview.findViewById(R.id.volume);
        mBeerType = (TextView) rootview.findViewById(R.id.beer_type);
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
}
