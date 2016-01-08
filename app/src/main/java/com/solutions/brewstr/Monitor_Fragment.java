package com.solutions.brewstr;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

/**
 * Created by jeremy on 2016-01-08.
 */
public class Monitor_Fragment extends Fragment implements View.OnClickListener {
    View rootview;
    Button monitorButton;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.monitor_view,container,false);
        monitorButton = (Button) rootview.findViewById(R.id.monitorButton);
        monitorButton.setOnClickListener(this);
        return rootview;
    }
    @Override
    public void onClick(View v) {
        //YOUR CODE HERE
        monitorButton.setText("CLICK WORKED!");
    }
}