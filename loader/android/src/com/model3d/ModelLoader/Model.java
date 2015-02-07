/**
 * 
 */
package com.model3d.ModelLoader;

import javax.microedition.khronos.opengles.GL10;

/**
 * @author bhanu
 *
 */
public interface Model {
	
	/**
	 * Called to render a model object
	 * @param gl
	 */
	void render(GL10 gl, float frameno);

}
