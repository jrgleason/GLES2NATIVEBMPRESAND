package com.gleason.gl.nat;

import java.io.FileDescriptor;

import android.content.res.AssetManager;

public class Native {
	static 
	{
		System.loadLibrary("testand");
	}

	public static native void init(int resize, int perspective);
	public static native void resize(int width, int height);
	public static native void render();
	public static native void initBMPTexture(int[] pixels, int width, int height);
}
