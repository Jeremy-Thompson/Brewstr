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
public class Setup_Fragment extends Fragment implements View.OnClickListener {
    View rootview;
    Button setupButton;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.setup_view,container,false);
        setupButton = (Button) rootview.findViewById(R.id.setupButton);
        setupButton.setOnClickListener(this);
        return rootview;
    }
    @Override
    public void onClick(View v) {
        //YOUR CODE HERE
        setupButton.setText("CLICK WORKED!");
    }
}