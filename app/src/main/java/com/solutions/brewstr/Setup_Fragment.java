package com.solutions.brewstr;

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
public class Setup_Fragment extends Fragment implements View.OnClickListener {
    View rootview;

    SeekBar mashTempSB;
    SeekBar mashTimeSB;
    SeekBar boilTempSB;
    SeekBar boilTimeSB;
    SeekBar hopsTimeSB;

    TextView mashTempSP;
    TextView MashTimeSP;
    TextView boilTimeSP;
    TextView boilTempSP;
    TextView mashTimeSP;


    Button setupButton;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.setup_view,container,false);

        //set button mapping and listeners
        setupButton = (Button) rootview.findViewById(R.id.setupButton);
        setupButton.setOnClickListener(this);

        //set seekbar mapping and listeners
        mashTempSB = (SeekBar) rootview.findViewById(R.id.mashingTempSeekBar);
        mashTempSB.setOnSeekBarChangeListener(new seekBarListener());
        mashTimeSB = (SeekBar) rootview.findViewById(R.id.mashingTimeSeekBar);
        mashTimeSB.setOnSeekBarChangeListener(new seekBarListener());
        boilTempSB = (SeekBar) rootview.findViewById(R.id.boilTempSeekBar);
        boilTempSB.setOnSeekBarChangeListener(new seekBarListener());
        boilTimeSB = (SeekBar) rootview.findViewById(R.id.boilTimeSeekBar);
        boilTimeSB.setOnSeekBarChangeListener(new seekBarListener());
        return rootview;
    }
    @Override
    public void onClick(View v) {
        //YOUR CODE HERE
        String tmp = "CLICK WORKED";
        setupButton.setText(tmp);
    }
    private class seekBarListener implements SeekBar.OnSeekBarChangeListener {

        public void onProgressChanged(SeekBar seekBar, int progress,
        boolean fromUser) {
            // Log the progress
            Log.d("DEBUG", "Progress is: " + progress);
            //set textView's text
        }

    public void onStartTrackingTouch(SeekBar seekBar) {}

    public void onStopTrackingTouch(SeekBar seekBar) {}

}
}