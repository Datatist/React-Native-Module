# 1. 使用 npm 方式安装 react-native-datatist 模块
对于 React Native 开发的应用，可以使用 npm 方式集成 react-native-datatist 模块。
## 1.1 npm 安装 react-native-datatist 模块
```
npm install react-native-datatist
```

## 1.2 link react-native-datatist 模块
```
react-native link react-native-datatist
```

# 2. Android 集成
## 2.1 添加 Android 端原生 SDK 依赖
react-native-datatist 模块 是在 Android 原生 SDK 上的扩展，参照 `集成 Android SDK`，集成配置SDK 和 插件，**SDK 版本号最低为 2.2.7.**

## 2.2 重要配置
react-native-datatist 模块需要在 `java` 代码中初始化。

在项目继承 Application 的类中，添加`DatatistRNModule`:
> 如果自定义的 Application 类名称是`MainApplication`，则`react-native link` 命令会在`MainApplication` 类中自动添加 `DatatistRNPackage`。

```
private final ReactNativeHost mReactNativeHost = new ReactNativeHost(this) {

    @Override
    protected List<ReactPackage> getPackages() {
        return Arrays.asList(
                new MainReactPackage(),
                //添加 DatatistRNModule
                new DatatistRNPackage()
        );
    }

};
```

# 3. iOS 集成
如果react-native link react-native-datatist失败

（成功则忽略此步骤），即发现Libraries中没有DatatistRNModule.xcodeproj，则可手动配置：

打开 XCode 工程中, 右键点击 Libraries 文件夹 ➜ Add Files to <...> 

去 node_modules  ➜ react-native-datatist ➜ ios ➜ 选择 DatatistRNModule.xcodeproj 

在工程Build Phases ➜ Link Binary With Libraries中添加libDatatistRNModule.a
# 4. ReactNative 埋点 Api
**埋点示例：**
```
var datatistRNModule = NativeModules.DatatistRNModule;

<Button
    title="登录"
    onPress={() => {
        nativeModule.trackLogin("userId");}
    }
/>

<Button
    title="自定义事件"
    onPress={() => {
        var properties = {shareId: 123, shareType: '热门'};
        datatistRNModule.customerTrack("share", properties);}
    }
/>

```

## 1. 自定义事件
参数：
1. eventName ： 事件名
2. properties ： 事件属性(自定义参数)

```
customerTrack(String eventName,JSONObject properties)
```

## 2. 页面采集
您需要在您认为页面发生切换时， 将最新的页面标识和标题名称告诉我们。 调用方法如下。

参数：
1. url ： 页面标识
2. title ：页面标题
```
trackPageview(String url,String title);
```

## 3. 设置用户ID
参数：
1. userId ：用户唯一标识
```
setUserId(String userId)
```
## 4. 设置用户属性
参数：
1. properties ：用户属性
```
setUserProperties(JSONObject properties);
```

## 5. 登录
参数:
1. uid :用户唯一标识
```
trackLogin(String uid);
```

## 6. 注册
参数：
1. uid: 用户注册的用户ID
2. type: 用户类型
3. authenticated: 是否已认证的标识，值为true/false
```
trackRegister(String uid,String type,boolean authenticated);
```

## 7. 登出
参数：
1. 无
```
trackLogout();
```