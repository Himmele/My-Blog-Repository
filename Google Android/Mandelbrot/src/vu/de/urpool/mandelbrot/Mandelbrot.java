package vu.de.urpool.mandelbrot;

import vu.de.urpool.mandelbrot.R;
import android.app.Activity;
import android.os.Bundle;

public class Mandelbrot extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mandelbrot);        
    }
}