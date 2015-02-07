/**
 * 
 */
package com.model3d.ModelLoader;

import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.StringTokenizer;

/**
 * @author bhanu
 *
 */
public class Model3dsAnim implements Model {
	private static final String TAG = "Model3DAnim";
	private int curAnimID = 0;
	private float r = 1.0f, g = 1.0f, b = 1.0f;

	/* (non-Javadoc)
	 * @see com.model3d.ModelViewer.Model#render(javax.microedition.khronos.opengles.GL10, float)
	 */
	@Override
	public void render(GL10 gl, float frameno) {
		int i;

		r -= 0.01f;
		if (r <= 0f) r = 1.0f;
		gl.glClearColor(r, g, b, 1);
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
		
		Action curAction = actionList[curAnimID];
		
		if (frameno > curAction.maxFrameNo) {
			float div = frameno / curAction.maxFrameNo;
			int d = (int) div;
			frameno = frameno - curAction.maxFrameNo*d;
		}
		if (frameno < curAction.minFrameNo)
			frameno = curAction.minFrameNo;

		Log.v(TAG, "RUNNING!");
		int f = curAction.getEndFrame(frameno);
		if (f < 0) {
			Log.e(TAG, "END FRAME FAILED!!!\n");
		}
		Frame startFrame = curAction.frameList[f];
		Frame endFrame = curAction.frameList[f+1];
		float ratio = (endFrame.frameno - frameno)/(endFrame.frameno - startFrame.frameno);
		
		gl.glColor4f(0, 0, 0, 1);
		
		// Start Rendering
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);

		// Load all vertices into a vertex array
		ByteBuffer vbb = ByteBuffer.allocateDirect(numVerts * 3 * 4);
	    vbb.order(ByteOrder.nativeOrder());
	    FloatBuffer vertexBuf = vbb.asFloatBuffer();
	    for (i = 0; i < numVerts*3; i++) {
	    	float r = startFrame.vertexArr[i]*ratio + endFrame.vertexArr[i]*(1.0f-ratio) ;
	    	vertexBuf.put(r);
	    }
		vertexBuf.position(0);
		
		// Define the vertices
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuf);		

		// Create each face here
		for (i = 0; i < numFaces; i++) {
			// Load indicies
			ByteBuffer ibb = ByteBuffer.allocateDirect(3 * 2);
			ibb.order(ByteOrder.nativeOrder());
			ShortBuffer indexBuffer = ibb.asShortBuffer();
			indexBuffer.put(faceList[i].index);
			indexBuffer.position(0);
			
			// Draw the face
			gl.glDrawElements(GL10.GL_TRIANGLES, 3, GL10.GL_UNSIGNED_SHORT, indexBuffer);

		}

		
		
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glFlush();
	}

	public void nextAnimation() {
		// Set Type of Animation here
		curAnimID++;
		if (numActions <= curAnimID) {
			curAnimID = 0;
		}
	}
	
	private int numVerts;
	private int numFaces;
	private int numActions;
	private class Face {
		public short index[];
		public float tex[];
		public Face(BufferedReader reader) throws IOException {
			int i;
			
			index = new short[3];
			tex = new float[6];

			StringTokenizer idxst = new StringTokenizer(reader.readLine());
			for (i = 0; i < 3; i++) {
				StringTokenizer texst = new StringTokenizer(reader.readLine());
				index[i] = Short.valueOf(idxst.nextToken()).shortValue();
				tex[2*i] = Float.valueOf(texst.nextToken()).floatValue();			
				tex[2*i+1] = Float.valueOf(texst.nextToken()).floatValue();
			}
		}
	}
	Face faceList[];
	Action actionList[];

	
	public void load(InputStream is) throws IOException {
		int i;
		
		BufferedReader reader = new BufferedReader(new InputStreamReader(is, "UTF-8"));

		// Read the number of vertices, faces & actions
		StringTokenizer subst = new StringTokenizer(reader.readLine());
		numVerts = Integer.valueOf(subst.nextToken()).intValue();
		numFaces = Integer.valueOf(subst.nextToken()).intValue();
		numActions = Integer.valueOf(subst.nextToken()).intValue();
		
		faceList = new Face[numFaces];

		// Read the list of Faces
		for (i = 0; i < numFaces; i++) {
			faceList[i] = new Face(reader);
		}

		actionList = new Action[numActions];
		// Read the list of Actions
		for (i = 0; i < numActions; i++) {
			actionList[i] = new Action(reader, numVerts);
		}
		
	}

	public void setBackground(float x, float y, float z) {
		r = x; g = y; b = z;
	}
}
