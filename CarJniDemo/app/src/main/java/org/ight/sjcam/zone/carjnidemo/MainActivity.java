package org.ight.sjcam.zone.carjnidemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.jms.JMSFramework;
import com.sjcam.driverecorder.gpsparser.GPSModel;
import com.sjcam.driverecorder.gpsparser.GPSParser;

public class MainActivity extends AppCompatActivity {



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);

        GPSParser gpsParser = new GPSParser();
        GPSModel[] parser = gpsParser.parser("/sdcard/SJCAMCAR/fun/ttt.mp4");

        if (parser!=null) {
            for (GPSModel model : parser) {
                double w = JMSFramework.changetoLatLon(model.Latitude, 0);
                double j = JMSFramework.changetoLatLon(model.Longitude, 1);

                System.out.println("=========model:"+String.format("经度：%s  ， 纬度：%s",j,w));
            }
        }
    }


}
