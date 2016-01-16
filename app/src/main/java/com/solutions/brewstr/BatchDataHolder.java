package com.solutions.brewstr;

import android.graphics.drawable.Drawable;

/**
 * Created by Derek on 2016-01-12.
 */
public class BatchDataHolder {
    private  String batchName="";
    private  Drawable Image= null;
    private  String stage="";

    public void setBatchName(String batchName)
    {
        this.batchName = batchName;
    }

    public void setImage(Drawable drawable)
    {
        this.Image = drawable;
    }

    public void setStage(String stage)
    {
        this.stage = stage;
    }


    public String getBatchName()
    {
        return this.batchName;
    }

    public Drawable getImage()
    {
        return this.Image;
    }

    public String getStage()
    {
        return this.stage;
    }
}
