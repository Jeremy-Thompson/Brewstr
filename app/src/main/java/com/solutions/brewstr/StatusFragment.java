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

public class StatusFragment extends Fragment {
    View rootview;
    TextView status;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.status_view, container, false);
        status = (TextView) rootview.findViewById(R.id.statusTextView);
        return rootview;
    }
    public void displayStatus(String str)
    {
        //decode and display the status.
        status.setText(str);
    }
}
