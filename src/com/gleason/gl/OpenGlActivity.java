package com.gleason.gl;


import com.gleason.gl.nat.Native;
import com.gleason.gl.renderer.NativeRenderer;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.util.Log;
import android.view.Display;
import android.view.Menu;

public class OpenGlActivity extends Activity {
	private GLSurfaceView view;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		BitmapFactory.Options options = new BitmapFactory.Options();
//	    options.inJustDecodeBounds = true;
	    
	    Bitmap b = BitmapFactory.decodeResource(getResources(), R.drawable.test, options);
	    int[] pixels = new int[b.getHeight()*b.getWidth()];
	    int[] pixelOut = new int[1];
	    if(b != null){
	    	pixelOut = new int[b.getHeight()*b.getWidth()*3];
	    	b.getPixels(pixels, 0, b.getWidth(), 0, 0, b.getWidth(), b.getHeight());
	    	int R, G, B,Y;
	    	int i = 0;
	    	Log.d("Test", "Starting loop");
            for (int y = 0; y < b.getHeight(); y++){
            	for (int x = 0; x < b.getWidth(); x++)
                {
            		int index = y * b.getWidth() + x;
            		R = (pixels[index] >> 16) & 0xff;     //bitwise shifting
            		G = (pixels[index] >> 8) & 0xff;
            		B = pixels[index] & 0xff;
            	    pixelOut[i++]=R;
            	    pixelOut[i++]=G;
            	    pixelOut[i++]=B;
                }
            	
            }
            Log.d("Test", "Loop finished");
	    }
	    
	    else{
	    	Log.d("Test", "Bitmap not found");
	    }
	    Log.d("Test", "Calling native BMP text.");
	    Native.initBMPTexture(pixelOut, b.getWidth(), b.getHeight());
	    view = new GLSurfaceView(this);
	    // Tell EGL to use a ES 2.0 Context
	    view.setEGLContextClientVersion(2);
	    view.setEGLConfigChooser(8 , 8, 8, 8, 16, 0);
	    // Set the renderer
	    setContentView(view);
	}
	
	private void setRenderer(){
		Display display = getWindowManager().getDefaultDisplay();
	    Point size = new Point();
	    display.getSize(size);
		GLSurfaceView.Renderer render = new NativeRenderer();
	    Native.resize(size.x, size.y);
	    view.setRenderer(render);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_open_gl, menu);
		return true;
	}
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		setRenderer();
	}
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	}

}
