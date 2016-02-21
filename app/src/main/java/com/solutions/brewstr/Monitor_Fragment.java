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

import com.solutions.brewstr.ProgressFragments.MashingProgressFragment;

/**
 * Created by jeremy on 2016-01-08.
 */
public class Monitor_Fragment extends Fragment implements View.OnClickListener {
    View rootview;
    ProgressBar mMashingProgressBar;
    ProgressBar mBoilingProgressBar;
    ProgressBar mFermentationProgressBar;
    Activity mActivity;

    String mBatchName = "";
    MashingProgressFragment mMashProgFrag = new MashingProgressFragment();


    // Following variables are used for proof of concept, should be deleted when real data is available
    Button monitorButton;
    int count = 0;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.monitor_view, container, false);

        Resources res = getResources();
        Drawable mashingDrawable = res.getDrawable(R.drawable.mashing_progress_drawable);
        monitorButton = (Button) rootview.findViewById(R.id.monitorButton);
        monitorButton.setOnClickListener(this);
        mMashingProgressBar = (ProgressBar) rootview.findViewById(R.id.mashing);
        mMashingProgressBar.setMax(100);
        mMashingProgressBar.setProgress(0);
        mMashingProgressBar.setProgressDrawable(mashingDrawable);

        Drawable boilingDrawable = res.getDrawable(R.drawable.boiling_progress_drawable);
        mBoilingProgressBar = (ProgressBar) rootview.findViewById(R.id.boiling);
        mBoilingProgressBar.setProgress(0);
        mBoilingProgressBar.setProgressDrawable(boilingDrawable);

        mFermentationProgressBar = (ProgressBar) rootview.findViewById(R.id.fermentation);
        Drawable fermentationDrawable = res.getDrawable(R.drawable.fermentation_progress_drawable);
        mFermentationProgressBar.setProgress(0);
        mFermentationProgressBar.setProgressDrawable(fermentationDrawable);

        FragmentManager fragmentManager = getChildFragmentManager();
        Bundle args = new Bundle();
        args.putString("batch",mBatchName);
        mMashProgFrag.setArguments(args);
        fragmentManager.beginTransaction()
                .replace(R.id.step_progress_container, mMashProgFrag)
                .commit();

        return rootview;
    }

    @Override
    public void onClick(View v) {
        //YOUR CODE HERE
        monitorButton.setText("CLICK WORKED!");
        count++;
        mMashingProgressBar.setProgress(Math.round(count*100/3) - 10);
        mMashingProgressBar.setSecondaryProgress(Math.round(count*100/3));

        final int totalProgressTime = 100;
        final Thread t = new Thread() {
            @Override
            public void run() {
                int jumpTime = 0;

                while(jumpTime < totalProgressTime) {
                    try {
                        sleep(200);
                        jumpTime += 5;
                        mBoilingProgressBar.setProgress(jumpTime - 10);
                        mBoilingProgressBar.setSecondaryProgress(jumpTime);
                    }
                    catch (InterruptedException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                }
            }
        };
        t.start();
    }

    public void setBatch(String batchName) {
        mBatchName = batchName;
    }

    public String getBatchName() {
        return mBatchName;
    }
}