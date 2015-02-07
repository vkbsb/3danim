/**
 * 
 */
package com.model3d.ModelLoader;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.StringTokenizer;

import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

/**
 * @author bhanu
 *
 */
public class Action implements Model {

	public int numVerts;
	public String name;
	public int numKeyFrames;
	public Frame frameList[];
	public int maxFrameNo, minFrameNo;

	public Action(BufferedReader reader, int nVerts) throws IOException {
		int i;
		numVerts = nVerts;

		StringTokenizer nmst = new StringTokenizer(reader.readLine());
		name = new String(nmst.nextToken());
		numKeyFrames = Integer.valueOf(nmst.nextToken()).intValue();
		
		frameList = new Frame[numKeyFrames];
		
		for (i = 0; i < numKeyFrames; i++) {
			frameList[i] = new Frame(reader, numVerts);
		}
		minFrameNo = frameList[0].frameno;
		maxFrameNo = frameList[numKeyFrames - 1].frameno;
	}
	
	public int getEndFrame(float frameno) {
		int i;
		for (i = 0; i < numKeyFrames; i++) {
			if (frameno < frameList[i].frameno) {
				Log.v("ACTION", "i = " + i + " frameno = " + frameno + " \n");
				return i - 1;
			}
		}
		return -1;
	}
	
	/* (non-Javadoc)
	 * @see com.model3d.ModelViewer.Model#render(javax.microedition.khronos.opengles.GL10, float)
	 */
	@Override
	public void render(GL10 gl, float frameno) {
		// TODO Auto-generated method stub

	}

}
