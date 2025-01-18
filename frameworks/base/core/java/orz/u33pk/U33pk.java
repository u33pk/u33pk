package orz.u33pk;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;
import android.app.ActivityThread;
import android.util.Log;
import android.app.Application;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public final class U33pk {
    private static final String TAG = "U33PK";
    private static final String CONF_PATH = "/data/local/tmp/u33pk.conf";
    private static final String FAKT_TAG = "fake_invoke : ";
    private static final String UPK_TAG = "u33pk_pkg : ";
    private static final String FD_TAG = "fd_pkg : ";
    private static final String libfd_name = "Alog";
    private static final int SLEEP_TIME = 30 * 1000;
    // private Context
    private String u33pk_pkg = "";
    private boolean should_invoke = false;
    private String fd_pkg = "";

    public U33pk(){
        try (BufferedReader bufferedReader = new BufferedReader(new FileReader(CONF_PATH))) {
            String line;
            while ((line = bufferedReader.readLine())  != null) {
                if(line.startsWith(FAKT_TAG)){
                    String result = line.substring(14);
                    if(result.startsWith("true")){
                        this.should_invoke = true;
                    }
                } else if(line.startsWith(UPK_TAG)) {
                    this.u33pk_pkg = line.substring(12).replaceAll("\\p{C}", "");
                } else if(line.startsWith(FD_TAG)) {
                    this.fd_pkg = line.substring(9).replaceAll("\\p{C}", "");
                }
            }
            Log.i(TAG,  this.u33pk_pkg + " " + String.valueOf(this.should_invoke));
        } catch (Exception e) {
            
        }
    }

    public void U3Invoke(){
        Thread t = new Thread(() -> {
            if(this.shouldUnpk() && this.shouldInvoke()){
                try{
                    Thread.sleep(SLEEP_TIME);
                    Log.i(TAG, "wait fake start");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Log.i(TAG, "fake start");
                U33pk.fakeInvoke();
            }
        });
        t.start();
    }

    private boolean shouldInvoke(){
        return this.should_invoke;
    }

    private boolean shouldUnpk(){
        String processName = ActivityThread.currentProcessName();
        if(this.u33pk_pkg.equals(processName)){
            return true;
        }
        return false;
    }

    public static Field getClassField(ClassLoader classloader, String class_name,
                                      String filedName) {

        try {
            Class obj_class = classloader.loadClass(class_name);//Class.forName(class_name);
            Field field = obj_class.getDeclaredField(filedName);
            field.setAccessible(true);
            return field;
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return null;

    }

    public static Object getClassFieldObject(ClassLoader classloader, String class_name, Object obj,
                                             String filedName) {

        try {
            Class obj_class = classloader.loadClass(class_name);//Class.forName(class_name);
            Field field = obj_class.getDeclaredField(filedName);
            field.setAccessible(true);
            Object result = null;
            result = field.get(obj);
            return result;
            //field.setAccessible(true);
            //return field;
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        return null;

    }

    public static Object invokeStaticMethod(String class_name,
                                            String method_name, Class[] pareTyple, Object[] pareVaules) {

        try {
            Class obj_class = Class.forName(class_name);
            Method method = obj_class.getMethod(method_name, pareTyple);
            return method.invoke(null, pareVaules);
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return null;

    }

    public static Object getFieldObject(String class_name, Object obj,
                                        String filedName) {
        try {
            Class obj_class = Class.forName(class_name);
            Field field = obj_class.getDeclaredField(filedName);
            field.setAccessible(true);
            return field.get(obj);
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (NullPointerException e) {
            e.printStackTrace();
        }
        return null;

    }

    public static Application getCurrentApplication(){
        Object currentActivityThread = invokeStaticMethod(
                "android.app.ActivityThread", "currentActivityThread",
                new Class[]{}, new Object[]{});
        Object mBoundApplication = getFieldObject(
                "android.app.ActivityThread", currentActivityThread,
                "mBoundApplication");
        Application mInitialApplication = (Application) getFieldObject("android.app.ActivityThread",
                currentActivityThread, "mInitialApplication");
        Object loadedApkInfo = getFieldObject(
                "android.app.ActivityThread$AppBindData",
                mBoundApplication, "info");
        Application mApplication = (Application) getFieldObject("android.app.LoadedApk", loadedApkInfo, "mApplication");
        return mApplication;
    }

    public static ClassLoader getClassloader() {
        ClassLoader resultClassloader = null;
        Object currentActivityThread = invokeStaticMethod(
                "android.app.ActivityThread", "currentActivityThread",
                new Class[]{}, new Object[]{});
        Object mBoundApplication = getFieldObject(
                "android.app.ActivityThread", currentActivityThread,
                "mBoundApplication");
        Application mInitialApplication = (Application) getFieldObject("android.app.ActivityThread",
                currentActivityThread, "mInitialApplication");
        Object loadedApkInfo = getFieldObject(
                "android.app.ActivityThread$AppBindData",
                mBoundApplication, "info");
        Application mApplication = (Application) getFieldObject("android.app.LoadedApk", loadedApkInfo, "mApplication");
        Log.e("fartext", "go into app->" + "packagename:" + mApplication.getPackageName());
        resultClassloader = mApplication.getClassLoader();
        return resultClassloader;
    }
    
    public void loadFRD(){
        String pkg = ActivityThread.currentProcessName();
        if(pkg != null && this.fd_pkg.equals(pkg)){
            System.loadLibrary(libfd_name);
            Log.i(TAG, pkg + " load libAlog.so success [orz]");
        } else {
            Log.i(TAG, pkg + " load libAlog.so error [orz]");
        }
    } 

    public static native void fakeInvoke();

}