package com.model3d.ModelViewer;

import java.io.IOException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.model3d.ModelLoader.Model3dsAnim;

import android.content.res.Resources;
import android.content.res.Resources.NotFoundException;
import android.opengl.GLU;
import android.opengl.GLSurfaceView.Renderer;
import android.util.FloatMath;

public class MVRenderer implements Renderer {
	
	private Resources res;
	private float frameno;
	private Model3dsAnim model;

	public MVRenderer(Resources r) {
		res = r;
	}
	
	@Override
	public void onDrawFrame(GL10 gl) {
	    lookAt(gl);
	    
		// Render the model
		model.render(gl, frameno);
		frameno += 0.1f;
	}
	
	private float hangle = 0.0f;
	private float vangle = 0.0f;
	private boolean cameraDirty = true;
	
	public void setHAngle(float a) {
		hangle = a;
		cameraDirty = true;
	}
	public void setVAngle(float a) {
		vangle = a;
		cameraDirty = true;
	}
	
	public void lookAt(GL10 gl) {
		if (cameraDirty) {
		    gl.glMatrixMode(GL10.GL_MODELVIEW);
		    gl.glLoadIdentity();
		    GLU.gluLookAt(gl, 20*FloatMath.sin(hangle)*FloatMath.cos(vangle), 10*FloatMath.sin(vangle), 20*FloatMath.cos(hangle)*FloatMath.cos(vangle), 0, 0, 0, 0, 100, 0);		
	//	    GLU.gluLookAt(gl, 0, -20, 10, 0, 0, 0, 0, 1, 0);
		    cameraDirty = false;
		}
	}

	public void nextAnimation() {
		model.nextAnimation();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		gl.glViewport(0, 0, width, height);
	    gl.glMatrixMode(GL10.GL_PROJECTION);
	    gl.glLoadIdentity();
	    GLU.gluPerspective(gl, 40f, (float)width/(float)height, 0.1f, 100f);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		model = new Model3dsAnim();
		
		// Load the model resource file here
		try {
			model.load(res.openRawResource(R.raw.model1));
		} catch (NotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
