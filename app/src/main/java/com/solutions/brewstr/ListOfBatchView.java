package com.solutions.brewstr;

import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

/**
 * Main list view that displays all the brews with their name, stage and a beer icon
 */
public class ListOfBatchView extends android.support.v4.app.Fragment {
    View rootview;
    ListView lv;
    BatchListAdapter adapter;
    public  ArrayList<BatchDataHolder> CustomListViewValuesArr = new ArrayList<BatchDataHolder>();


    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.batches_list, container, false);

        Resources res = getActivity().getResources();

        setListData(res);

        lv = ( ListView ) rootview.findViewById( R.id.list );  // List defined in XML ( See Below )

        // Create Custom Adapter
        adapter = new BatchListAdapter((MainActivity)getActivity(), CustomListViewValuesArr, res );
        lv.setAdapter( adapter );

        return rootview;
    }

    /**
     * Retrieve the batch list information
     * @param res Needed to get the drawables
     */
    public void setListData(Resources res)
    {
        Drawable beerGlassIcon = res.getDrawable(R.mipmap.beer_glass);
        CustomListViewValuesArr.clear();
        for (int i = 0; i < 5; i++) {

            final BatchDataHolder data = new BatchDataHolder();

            data.setBatchName("Batch " + i);
            data.setStage("Stage " + i);
            data.setImage(beerGlassIcon);

            CustomListViewValuesArr.add(data);
        }

    }
}
