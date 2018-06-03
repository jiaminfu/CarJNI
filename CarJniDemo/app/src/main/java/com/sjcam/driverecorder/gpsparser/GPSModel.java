package com.sjcam.driverecorder.gpsparser;

/**
 * Created by admin on 2017/11/20.
 */

public class GPSModel {
    public int feedback;
    public int timeSpan;

    public int Hour;
    public int Minute;
    public int Second;
    public int Year;
    public int Month;
    public int Day;

    public char Status;
    public char NSInd;
    public char EWInd;
    public char reservd;
    public float Latitude;
    public float Longitude;
    public float Speed;
    public float Angle;

    @Override
    public String toString() {
        return String.format("Hour:%s , Minute:%s , Second:%s , Year:%s , Month:%s , " +
                "Day:%s , Status:%s , NSInd:%s , EWInd:%s , reservd:%s , Latitude:%s , Longitude:" +
                "%s , Speed:%s , Angle:%s",Hour,Minute,Second,Year,Month,Day,Status,NSInd,EWInd
                ,reservd,Latitude,Longitude,Speed,Angle);
    }
}
