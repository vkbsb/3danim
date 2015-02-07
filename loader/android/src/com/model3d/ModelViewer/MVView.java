package com.model3d.ModelViewer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class MVView extends GLSurfaceView {
	
	private MVRenderer mvr;
	
	public MVView(Context c) {
		super(c);

		mvr = new MVRenderer(this.getResources());
		this.setRenderer(mvr);
		
		this.requestFocus();
		this.setFocusableInTouchMode(true);

	}

	public boolean onTouchEvent(final MotionEvent event) {
	    queueEvent(new Runnable() {
	        public void run() {
	            mvr.setHAngle(-5f * event.getX() / getWidth());
	            mvr.setVAngle(-5f * event.getY() / getHeight());
	        }
	    });
	    return true;
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		super.onKeyDown(keyCode, event);
	    if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
	    	mvr.nextAnimation();
	    	return true;
	    }
	    return false;
    }

}
