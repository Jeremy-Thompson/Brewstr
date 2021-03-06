package com.solutions.brewstr;


import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.support.v4.app.FragmentManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

public class BatchListAdapter extends BaseAdapter implements View.OnClickListener {

    private MainActivity activity;
    private ArrayList data;
    private static LayoutInflater inflater=null;
    public Resources res;
    BatchDataHolder tempValues=null;
    Monitor_Fragment monitor_frag = new Monitor_Fragment();

    /*************  CustomAdapter Constructor *****************/
    public BatchListAdapter(MainActivity a, ArrayList d,Resources resLocal) {

        activity = a;
        data=d;
        res = resLocal;

        /***********  Layout inflator to call external xml layout () ***********/
        inflater = ( LayoutInflater )activity.
                getSystemService(Context.LAYOUT_INFLATER_SERVICE);

    }

    /******** What is the size of Passed Arraylist Size ************/
    public int getCount() {

        if(data.size()<=0)
            return 1;
        return data.size();
    }

    public Object getItem(int position) {
        return position;
    }

    public long getItemId(int position) {
        return position;
    }

    /********* Create a holder Class to contain inflated xml file elements *********/
    public static class ViewHolder{

        public TextView text;
        public TextView text1;
        public TextView textWide;
        public ImageView image;

    }

    /****** Depends upon data size called for each row , Create each ListView row *****/
    public View getView(int position, View convertView, ViewGroup parent) {

        View vi = convertView;
        ViewHolder holder;

        if(convertView==null){

            /****** Inflate tabitem.xml file for each row ( Defined below ) *******/
            vi = inflater.inflate(R.layout.batch_item_list, null);

            /****** View Holder Object to contain tabitem.xml file elements ******/

            holder = new ViewHolder();
            holder.text = (TextView) vi.findViewById(R.id.text);
            holder.text1 = (TextView) vi.findViewById(R.id.text1);
            holder.image=(ImageView)vi.findViewById(R.id.beer_image);

            /************  Set holder with LayoutInflater ************/
            vi.setTag( holder );
        }
        else
            // Reuse the viewholder and find it using a tag
            holder=(ViewHolder)vi.getTag();

        if(data.size()<=0)
        {
            holder.text.setText("No Data");

        }
        else
        {
            /***** Get each Model object from Arraylist ********/
            tempValues=null;
            tempValues = ( BatchDataHolder ) data.get( position );

            /************  Set Model values in Holder elements ***********/

            holder.text.setText( tempValues.getBatchName() );
            holder.text1.setText( tempValues.getStage() );
            holder.image.setImageDrawable(tempValues.getImage());

            /******** Set Item Click Listner for LayoutInflater for each row *******/

            vi.setOnClickListener(new OnItemClickListener( position ));
        }
        return vi;
    }

    @Override
    public void onClick(View v) {
        Log.v("CustomAdapter", "=====Row button clicked=====");
    }

    /********* Called when Item click in ListView ************/
    private class OnItemClickListener  implements View.OnClickListener {
        private int mPosition;

        OnItemClickListener(int position){
            mPosition = position;
        }

        @Override
        public void onClick(View arg0) {
            // Set batch name of the list item that was selected
            tempValues = (BatchDataHolder) data.get(mPosition);
            monitor_frag.setBatch(tempValues.getBatchName());

            activity.fragmentReplace(monitor_frag);
}
    }
}