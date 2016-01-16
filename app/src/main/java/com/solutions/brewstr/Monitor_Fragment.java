package com.solutions.brewstr;

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
    Button monitorButton;
    ProgressBar mashingProgressBar;
    ProgressBar boilingProgressBar;
    ProgressBar fermentationProgressBar;
    int count = 0;
    MashingProgressFragment mashProgFrag = new MashingProgressFragment();

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.monitor_view, container, false);
        monitorButton = (Button) rootview.findViewById(R.id.monitorButton);
        monitorButton.setOnClickListener(this);
        mashingProgressBar = (ProgressBar) rootview.findViewById(R.id.mashing);
        mashingProgressBar.setMax(100);
        mashingProgressBar.setProgress(0);

        boilingProgressBar = (ProgressBar) rootview.findViewById(R.id.boiling);
        boilingProgressBar.setProgress(0);

        fermentationProgressBar = (ProgressBar) rootview.findViewById(R.id.fermentation);

        FragmentManager fragmentManager = getChildFragmentManager();
        fragmentManager.beginTransaction()
                .replace(R.id.step_progress_container, mashProgFrag)
                .commit();
        return rootview;
    }
    @Override
    public void onClick(View v) {
        //YOUR CODE HERE
        monitorButton.setText("CLICK WORKED!");
        count++;
        mashingProgressBar.setProgress(Math.round(count*100/3));

        final int totalProgressTime = 100;
        final Thread t = new Thread() {
            @Override
            public void run() {
                int jumpTime = 0;

                while(jumpTime < totalProgressTime) {
                    try {
                        sleep(200);
                        jumpTime += 5;
                        boilingProgressBar.setProgress(jumpTime);
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

}