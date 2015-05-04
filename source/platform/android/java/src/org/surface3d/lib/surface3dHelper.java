/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.surface3d.lib;

import java.io.UnsupportedEncodingException;
import java.util.Locale;
import java.util.LinkedHashSet;
import java.util.Set;
import java.lang.Runnable;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.net.Uri;
import android.os.Build;
import android.preference.PreferenceManager.OnActivityResultListener;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;

public class surface3dHelper {
    // ===========================================================
    // Constants
    // ===========================================================
    private static final String PREFS_NAME = "Cocos2dxPrefsFile";
    private static final int RUNNABLES_PER_FRAME = 5;

    // ===========================================================
    // Fields
    // ===========================================================

    private static Cocos2dxMusic sCocos2dMusic;
    private static Cocos2dxSound sCocos2dSound;
    private static AssetManager sAssetManager;
    private static Cocos2dxAccelerometer sCocos2dxAccelerometer;
    private static boolean sAccelerometerEnabled;
    private static boolean sActivityVisible;
    private static String sPackageName;
    private static String sFileDirectory;
    private static Activity sActivity = null;
    private static surface3dHelperListener ssurface3dHelperListener;
    private static Set<OnActivityResultListener> onActivityResultListeners = new LinkedHashSet<OnActivityResultListener>();


    // ===========================================================
    // Constructors
    // ===========================================================

    public static void runOnGLThread(final Runnable r) {
        ((surface3dActivity)sActivity).runOnGLThread(r);
    }

    private static boolean sInited = false;
    public static void init(final Activity activity) {
        if (!sInited) {
            final ApplicationInfo applicationInfo = activity.getApplicationInfo();
            
            surface3dHelper.ssurface3dHelperListener = (surface3dHelperListener)activity;
                    
            surface3dHelper.sPackageName = applicationInfo.packageName;
            surface3dHelper.sFileDirectory = activity.getFilesDir().getAbsolutePath();
            surface3dHelper.nativeSetApkPath(applicationInfo.sourceDir);
    
            surface3dHelper.sCocos2dxAccelerometer = new Cocos2dxAccelerometer(activity);
            surface3dHelper.sCocos2dMusic = new Cocos2dxMusic(activity);
            surface3dHelper.sCocos2dSound = new Cocos2dxSound(activity);
            surface3dHelper.sAssetManager = activity.getAssets();
            surface3dHelper.nativeSetContext((Context)activity, surface3dHelper.sAssetManager);
    
            Cocos2dxBitmap.setContext(activity);
            sActivity = activity;

            sInited = true;

        }
    }
    
    public static Activity getActivity() {
        return sActivity;
    }
    
    public static void addOnActivityResultListener(OnActivityResultListener listener) {
        onActivityResultListeners.add(listener);
    }
    
    public static Set<OnActivityResultListener> getOnActivityResultListeners() {
        return onActivityResultListeners;
    }
    
    public static boolean isActivityVisible(){
        return sActivityVisible;
    }

    // ===========================================================
    // Getter & Setter
    // ===========================================================

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================

    private static native void nativeSetApkPath(final String pApkPath);

    private static native void nativeSetEditTextDialogResult(final byte[] pBytes);

    private static native void nativeSetContext(final Context pContext, final AssetManager pAssetManager);

    public static String getCocos2dxPackageName() {
        return surface3dHelper.sPackageName;
    }

    public static String getCocos2dxWritablePath() {
        return surface3dHelper.sFileDirectory;
    }

    public static String getCurrentLanguage() {
        return Locale.getDefault().getLanguage();
    }
    
    public static String getDeviceModel(){
        return Build.MODEL;
    }

    public static AssetManager getAssetManager() {
        return surface3dHelper.sAssetManager;
    }

    public static void enableAccelerometer() {
        surface3dHelper.sAccelerometerEnabled = true;
        surface3dHelper.sCocos2dxAccelerometer.enable();
    }


    public static void setAccelerometerInterval(float interval) {
        surface3dHelper.sCocos2dxAccelerometer.setInterval(interval);
    }

    public static void disableAccelerometer() {
        surface3dHelper.sAccelerometerEnabled = false;
        surface3dHelper.sCocos2dxAccelerometer.disable();
    }
    
    public static void setKeepScreenOn(boolean value) {
        ((surface3dActivity)sActivity).setKeepScreenOn(value);
    }
    
    public static boolean openURL(String url) { 
        boolean ret = false;
        try {
            Intent i = new Intent(Intent.ACTION_VIEW);
            i.setData(Uri.parse(url));
            sActivity.startActivity(i);
            ret = true;
        } catch (Exception e) {
        }
        return ret;
    }

    public static void preloadBackgroundMusic(final String pPath) {
        surface3dHelper.sCocos2dMusic.preloadBackgroundMusic(pPath);
    }

    public static void playBackgroundMusic(final String pPath, final boolean isLoop) {
        surface3dHelper.sCocos2dMusic.playBackgroundMusic(pPath, isLoop);
    }

    public static void resumeBackgroundMusic() {
        surface3dHelper.sCocos2dMusic.resumeBackgroundMusic();
    }

    public static void pauseBackgroundMusic() {
        surface3dHelper.sCocos2dMusic.pauseBackgroundMusic();
    }

    public static void stopBackgroundMusic() {
        surface3dHelper.sCocos2dMusic.stopBackgroundMusic();
    }

    public static void rewindBackgroundMusic() {
        surface3dHelper.sCocos2dMusic.rewindBackgroundMusic();
    }

    public static boolean isBackgroundMusicPlaying() {
        return surface3dHelper.sCocos2dMusic.isBackgroundMusicPlaying();
    }

    public static float getBackgroundMusicVolume() {
        return surface3dHelper.sCocos2dMusic.getBackgroundVolume();
    }

    public static void setBackgroundMusicVolume(final float volume) {
        surface3dHelper.sCocos2dMusic.setBackgroundVolume(volume);
    }

    public static void preloadEffect(final String path) {
        surface3dHelper.sCocos2dSound.preloadEffect(path);
    }

    public static int playEffect(final String path, final boolean isLoop, final float pitch, final float pan, final float gain) {
        return surface3dHelper.sCocos2dSound.playEffect(path, isLoop, pitch, pan, gain);
    }

    public static void resumeEffect(final int soundId) {
        surface3dHelper.sCocos2dSound.resumeEffect(soundId);
    }

    public static void pauseEffect(final int soundId) {
        surface3dHelper.sCocos2dSound.pauseEffect(soundId);
    }

    public static void stopEffect(final int soundId) {
        surface3dHelper.sCocos2dSound.stopEffect(soundId);
    }

    public static float getEffectsVolume() {
        return surface3dHelper.sCocos2dSound.getEffectsVolume();
    }

    public static void setEffectsVolume(final float volume) {
        surface3dHelper.sCocos2dSound.setEffectsVolume(volume);
    }

    public static void unloadEffect(final String path) {
        surface3dHelper.sCocos2dSound.unloadEffect(path);
    }

    public static void pauseAllEffects() {
        surface3dHelper.sCocos2dSound.pauseAllEffects();
    }

    public static void resumeAllEffects() {
        surface3dHelper.sCocos2dSound.resumeAllEffects();
    }

    public static void stopAllEffects() {
        surface3dHelper.sCocos2dSound.stopAllEffects();
    }

    public static void end() {
        surface3dHelper.sCocos2dMusic.end();
        surface3dHelper.sCocos2dSound.end();
    }

    public static void onResume() {
        sActivityVisible = true;
        if (surface3dHelper.sAccelerometerEnabled) {
            surface3dHelper.sCocos2dxAccelerometer.enable();
        }
    }

    public static void onPause() {
        sActivityVisible = false;
        if (surface3dHelper.sAccelerometerEnabled) {
            surface3dHelper.sCocos2dxAccelerometer.disable();
        }
    }

    public static void onEnterBackground() {
        sCocos2dSound.onEnterBackground();
        sCocos2dMusic.onEnterBackground();
    }
    
    public static void onEnterForeground() {
        sCocos2dSound.onEnterForeground();
        sCocos2dMusic.onEnterForeground();
    }
    
    public static void terminateProcess() {
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    private static void showDialog(final String pTitle, final String pMessage) {
        surface3dHelper.ssurface3dHelperListener.showDialog(pTitle, pMessage);
    }

    private static void showEditTextDialog(final String pTitle, final String pMessage, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) {
        surface3dHelper.ssurface3dHelperListener.showEditTextDialog(pTitle, pMessage, pInputMode, pInputFlag, pReturnType, pMaxLength);
    }

    public static void setEditTextDialogResult(final String pResult) {
        try {
            final byte[] bytesUTF8 = pResult.getBytes("UTF8");

            surface3dHelper.ssurface3dHelperListener.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    surface3dHelper.nativeSetEditTextDialogResult(bytesUTF8);
                }
            });
        } catch (UnsupportedEncodingException pUnsupportedEncodingException) {
            /* Nothing. */
        }
    }

    public static int getDPI()
    {
        if (sActivity != null)
        {
            DisplayMetrics metrics = new DisplayMetrics();
            WindowManager wm = sActivity.getWindowManager();
            if (wm != null)
            {
                Display d = wm.getDefaultDisplay();
                if (d != null)
                {
                    d.getMetrics(metrics);
                    return (int)(metrics.density*160.0f);
                }
            }
        }
        return -1;
    }
    
    // ===========================================================
    // Functions for CCUserDefault
    // ===========================================================
    
    public static boolean getBoolForKey(String key, boolean defaultValue) {
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        return settings.getBoolean(key, defaultValue);
    }
    
    public static int getIntegerForKey(String key, int defaultValue) {
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        return settings.getInt(key, defaultValue);
    }
    
    public static float getFloatForKey(String key, float defaultValue) {
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        return settings.getFloat(key, defaultValue);
    }
    
    public static double getDoubleForKey(String key, double defaultValue) {
        // SharedPreferences doesn't support saving double value
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        return settings.getFloat(key, (float)defaultValue);
    }
    
    public static String getStringForKey(String key, String defaultValue) {
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        return settings.getString(key, defaultValue);
    }
    
    public static void setBoolForKey(String key, boolean value) {
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putBoolean(key, value);
        editor.commit();
    }
    
    public static void setIntegerForKey(String key, int value) {
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(key, value);
        editor.commit();
    }
    
    public static void setFloatForKey(String key, float value) {
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putFloat(key, value);
        editor.commit();
    }
    
    public static void setDoubleForKey(String key, double value) {
        // SharedPreferences doesn't support recording double value
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putFloat(key, (float)value);
        editor.commit();
    }
    
    public static void setStringForKey(String key, String value) {
        SharedPreferences settings = sActivity.getSharedPreferences(surface3dHelper.PREFS_NAME, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString(key, value);
        editor.commit();
    }
    
    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================

    public static interface surface3dHelperListener {
        public void showDialog(final String pTitle, final String pMessage);
        public void showEditTextDialog(final String pTitle, final String pMessage, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength);

        public void runOnGLThread(final Runnable pRunnable);
    }
}
