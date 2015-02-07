package com.model3d.ModelViewer;

import android.app.Activity;
import android.os.Bundle;

public class ModelViewer extends Activity {
	
	private MVView mvv;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        mvv = new MVView(this);
        setContentView(mvv);
    }
}