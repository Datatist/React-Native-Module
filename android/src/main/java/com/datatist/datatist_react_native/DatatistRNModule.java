package com.datatist.datatist_react_native;

import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableNativeMap;

import org.datatist.sdk.DatatistSDK;
import org.datatist.sdk.Track;
import org.json.JSONObject;


public class DatatistRNModule extends ReactContextBaseJavaModule {


    private static final String TAG = "DatatistRNModule";
    private ReactContext mReactContext;

    public DatatistRNModule(ReactApplicationContext context) {
        super(context);
        this.mReactContext = context;
    }

    @Override
    public String getName() {
        return "DatatistRNModule";
    }

    /**
     * ReadableMap 转换成 JSONObject
     */
    private JSONObject convertToJSONObject(ReadableMap properties) {
        if (properties == null) {
            return null;
        }

        JSONObject json = null;
        ReadableNativeMap nativeMap = null;
        try {
            nativeMap = (ReadableNativeMap) properties;
            json = new JSONObject(properties.toString()).getJSONObject("NativeMap");
        } catch (Exception e) {
            Log.e(TAG, "" + e.getMessage());
            String superName = nativeMap.getClass().getSuperclass().getSimpleName();
            try {
                json = new JSONObject(properties.toString()).getJSONObject(superName);
            } catch (Exception e1) {
                Log.e(TAG, "" + e1.getMessage());
            }
        }
        return json;
    }

    /**
     * 参数类型在@ReactMethod注明的方法中，会被直接映射到它们对应的JavaScript类型
     * String -> String
     * ReadableMap -> Object
     * Boolean -> Bool
     * Integer -> Number
     * Double -> Number
     * Float -> Number
     * Callback -> function
     * ReadableArray -> Array
     * <p>
     * 导出 customerTrack 方法给 RN 使用.
     *
     * @param eventName  事件名称
     * @param properties 事件属性(自定义参数)
     *                   <p>
     *                   RN 中使用示例：（上报 share 事件，事件属性 "shareId":123456,"shareType":"热门"）
     *                   <Button
     *                   title="Button"
     *                   onPress={()=>
     *                   DatatistRNModule.customerTrack("share",{ "shareId":123456,"shareType":"热门"})}>
     *                   </Button>
     */
    @ReactMethod
    public void customerTrack(String eventName, ReadableMap properties) {
        Track.track().customerEvent().customerTrack(eventName, convertToJSONObject(properties));
    }

    @ReactMethod
    public void trackPageview(String url, String title) {
        Track.track().pageview().setTitle(title).setUrl(url).trackPageview().submit(DatatistSDK.getTracker());
    }

    @ReactMethod
    public void setUserId(String userId) {
        DatatistSDK.setUserId(userId);
    }

    @ReactMethod
    public void setUserProperties(ReadableMap properties) {
        DatatistSDK.setUserProperty(convertToJSONObject(properties));
    }

    @ReactMethod
    public void trackLogin(String userId) {
        Track.track().login().trackLogin(userId).submit(DatatistSDK.getTracker());

    }

    @ReactMethod
    public void trackRegister(String uid, String type, boolean authenticated) {
        Track.track().register().trackRegister(uid, type, authenticated).submit(DatatistSDK.getTracker());
    }

    @ReactMethod
    public void trackLogout() {
        Track.track().logout().trackLogout().submit(DatatistSDK.getTracker());
    }

}
