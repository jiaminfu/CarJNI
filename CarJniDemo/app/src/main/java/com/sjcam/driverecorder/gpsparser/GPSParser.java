package com.sjcam.driverecorder.gpsparser;

/**
 * Created by admin on 2017/11/20.
 */

public class GPSParser {
    static {
        System.loadLibrary("mp4Parser-lib");
    }


    public native GPSModel[] parser(String path);
}
