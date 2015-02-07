/**
 * 
 */
package com.model3d.ModelLoader;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.StringTokenizer;

import javax.microedition.khronos.opengles.GL10;

/**
 * @author bhanu
 *
 */
public class Frame implements Model {
	public float vertexArr[];
	public int frameno;

	public Frame(BufferedReader reader, int numVerts) throws IOException {
		int i;

		frameno = Integer.valueOf(reader.readLine()).intValue();
		
		vertexArr = new float[3*numVerts];
		
		for (i = 0; i < numVerts; i++) {
			StringTokenizer fst = new StringTokenizer(reader.readLine());
			vertexArr[3*i + 0] = Float.valueOf(fst.nextToken()).floatValue();			
			vertexArr[3*i + 1] = Float.valueOf(fst.nextToken()).floatValue();			
			vertexArr[3*i + 2] = Float.valueOf(fst.nextToken()).floatValue();			
		}
	}
	
	/* (non-Javadoc)
	 * @see com.model3d.ModelViewer.Model#render(javax.microedition.khronos.opengles.GL10, float)
	 */
	@Override
	public void render(GL10 gl, float frameno) {
		// TODO Auto-generated method stub

	}

}
