//
//  DatatistTracker.h
//  DatatistTracker
//
//  Created by Mattias Levin on 3/12/13.
//  Copyright 2013 Mattias Levin. All rights reserved.
//

#define ABOVE_IOS_8_0           1 
#define WKWebView_Bridge        1
#define DatatistDeprecated(instead) NS_DEPRECATED(2_0, 2_0, 2_0, 2_0, instead)

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "DatatistOrderInfo.h"
#import "DatatistCouponInfo.h"
#import "DatatistProductInfo.h"
#import "DatatistDispatcher.h"
#import "DatatistTransaction.h"
#if ABOVE_IOS_8_0 && WKWebView_Bridge
#import "WebViewJavascriptBridge.h"
#endif
//@import WebViewJavascriptBridge;

@class DatatistOrderInfo;
@class DatatistCouponInfo;
@class DatatistProductInfo;


typedef NS_ENUM(NSInteger, DatatistPushType) {
    DatatistPushTypeJPush = 0,      // 极光
    DatatistPushTypeGetui = 1,      // 个推
    DatatistPushTypeUmeng = 2,      // 友盟
    DatatistPushTypeOthers = 256,   // 其他
};

typedef NS_ENUM(NSInteger, DatatistAPIRequestResult) {
    DatatistAPIRequestResultSuccess = 0,  // 成功
    DatatistAPIRequestResultFailed  = 1,  // 失败
};

@interface CustomVariable : NSObject

@property (nonatomic) NSUInteger index;
@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *value;


- (id)initWithIndex:(NSUInteger)index name:(NSString*)name value:(NSString*)value;

@end

/**
 
 The DatatistTracker is an Objective-C framework (iOS and OSX) for sending analytics to a Datatist server.
 
 Datatist server is a downloadable, Free/Libre (GPLv3 licensed) real time web analytics software, [http://datatist.org](http://datatist.org).
 This framework implements the Datatist tracking REST API [http://datatist.org/docs/tracking-api/reference.](http://datatist.org/docs/tracking-api/reference/)
 
 
 ###How does it work
 
 1. Create and configure the tracker
 2. Track screen views, events, errors, social interaction, search, goals and more
 3. Let the SDK dispatch events to the Datatist server automatically, or dispatch events manually
 
 All events are persisted locally in Core Data until they are dispatched and successfully received by the Datatist server.
 
 All methods are asynchronous and will return immediately.
 */
@interface DatatistTracker : NSObject


/**
 @name Creating a Datatist tracker
 */

/**
 Create and configure a shared Datatist tracker.
 
 A default dispatcher will be selected and created based on the dependencies available:
 1) AFNetworking v2
 2) NSURLSession (fallback, will always work)
 
 @param siteID The unique site id generated by the the Datatist server when a new site is created
 @param baseURL The base URL of the Datatist server. The URL should not include the tracking endpoint path component (/datatist.php)
 @return The newly created DatatistTracker
 */
+ (instancetype)initWithSiteID:(NSString*)siteID baseURL:(NSURL*)baseURL;

+ (instancetype)initWithSiteID:(NSString*)siteID baseURL:(NSURL*)baseURL  AutoTrack:(BOOL)autoTrack;

+ (instancetype)initWithSiteID:(NSString*)siteID BaseURL:(NSURL*)baseURL Site_1_ID:(NSString*)site_1_ID Base_1_URL:(NSURL*)base_1_URL DatatistDeprecated("使用initWithSiteID: baseURL:");

+ (instancetype)initWithSiteID:(NSString*)siteID BaseURL:(NSURL*)baseURL AutoTrack:(BOOL)autoTrack Site_1_ID:(NSString*)site_1_ID Base_1_URL:(NSURL*)base_1_URL DatatistDeprecated("使用initWithSiteID: baseURL:AutoTrack") ;

/**
 Return the shared Datatist tracker.
 
 The Datatist tracker must have been created and configured for this method to return the tracker.
 
 @return The existing DatatistTracker object
 @see sharedInstanceWithSiteID:baseURL:
 */
+ (instancetype)sharedInstance;

/**
 SDK version
 */
+ (NSString *)version;


/**
 @name Tracker configuration
 */


/**
 The user id is any non empty unique string identifying the user, such as an email address or a username.
 
 Set this value when you have a way of uniquely identifying the user in the app, e.g. after the user signed in. The user id will appear in the Datatist server and is available for segmentation reports.
 It is important that the user id is saved locally in the app and set each time directly after the tracker is started. Otherwise tracked events sent before setting this property will not be attributed to the user id.
 
 The user id allow events from different platforms, e.g. iOS and Android, to be associated in the Datatist server to the same user as long as the same user id is used on all platforms.
 
 To stop associating tracked events to a specific user set this property to nil, e.g. after user signs out.
 
 If no user id is provided, an internal random id will be managed by the SDK to track unique users and visits. This value will be persisted across app restarts. If the app is uninstalled/installed again or the user change device, a new id will be generated and events tracked as a new unique user.
 */
//@property (nonatomic, strong) NSString *userID;

/**
 show log
 */
@property(nonatomic) BOOL showLog;


/**
 user Property
 */
@property(nonatomic) NSDictionary *userProperty;


/**
 user Id
 */
@property(nonatomic, strong) NSString *userID;

/**
 projectId
 */
@property(nonatomic) NSString *projectId;

/**
 @name Session control
 */

/**
 Set this value to YES to force a new session start when the next event is sent to the Datatist server.
 
 By default a new session is started each time the application in launched.
 */
@property (nonatomic) BOOL sessionStart;

/**
 A new session will be generated if the application spent longer time in the background then the session timeout value. Default value 120 seconds.
 
 The Datatist server will also create a new session if the event is recorded 30 minutes after the previous received event.
 */
@property (nonatomic) NSTimeInterval sessionTimeout;

@property (nonatomic, assign) BOOL enableTrack;

/**
 是否开启全埋点采集，默认为关闭
 */
@property (nonatomic, assign) BOOL enableAutoTrack;


/**
 @name Dispatch pending events
 */

/**
 The tracker will automatically dispatch all pending events on a timer. Default value 120 seconds.
 
 If a negative value is set the dispatch timer will never run and manual dispatch must be used. If 0 is set the event is dispatched as as quick as possible after it has been queued.
 
 @see dispatch
 */
@property(nonatomic) NSTimeInterval dispatchInterval;

/**
 Specifies the maximum number of events queued in core date. Default 500.
 
 If the number of queued events exceed this value events will no longer be queued.
 */
@property (nonatomic) NSUInteger maxNumberOfQueuedEvents;

/**
 Specifies how many events should be sent to the Datatist server in each request. Default 20 events per request.
 
 The Datatist server support sending one event at the time or in bulk mode.
 */
@property (nonatomic) NSUInteger eventsPerRequest;

@property (nonatomic, strong) NSString *visualDefineUrl;
@property (nonatomic, strong) NSString *visualJSUrl;

/**
 Track a single hierarchical screen view.
 
 Datatist support hierarchical screen names, e.g. appName/settings/register. Use this to create a hierarchical and logical grouping of screen views in the Datatist web interface.
 
 Screen views are prefixed with "screen" by default unless prefixing scheme is turned off.
 
 @param views A list of names of the screen to track.
 @param title title of current view.
 @param vars user defined variables.
 */
- (void)trackPageView:(NSString *)views title:(NSString *)title udVariable:(NSDictionary *)vars;
- (void)trackPageView:(NSString *)views title:(NSString *)title;


/**
 Track a search performed in the application. The search could be local or towards a server.
 
 Searches will be presented as Site Search requests in the Datatist web interface.
 
 @param keyword The search keyword entered by the user.
 @param recommendationFlag a flag whether search from recommendation.
 @param historyFlag a flag whether search from history.
 @param vars user defined variables.
 */
- (void)trackSearch:(NSString *)keyword recommendationSearchFlag:(BOOL)recommendationFlag historySearchFlag:(BOOL)historyFlag udVariable:(NSDictionary *)vars;

/**
 Track a search performed in the application. The search could be local or towards a server.
 
 Searches will be presented as Site Search requests in the Datatist web interface.
 
 @param keyword The search keyword entered by the user.
 @param recommendationFlag a flag whether search from recommendation.
 @param historyFlag a flag whether search from history.
 */
- (void)trackSearch:(NSString *)keyword recommendationSearchFlag:(BOOL)recommendationFlag historySearchFlag:(BOOL)historyFlag;



/**
 Track a register event in the application.
 
 @param uid user id.
 @param type register type.
 @param auth a flag whether authenticated.
 @param vars user defined variables.
 */
- (void)trackRegister:(NSString *)uid type:(NSString *)type authenticated:(BOOL)auth udVariable:(NSDictionary *)vars;

/**
 Track a register event in the application.
 
 @param uid user id.
 @param type register type.
 @param auth a flag whether authenticated.
 */
- (void)trackRegister:(NSString *)uid type:(NSString *)type authenticated:(BOOL)auth;


/**
 Track product info.
 @param vars user defined variables.
 */
- (void)trackProductPage:(NSString *)sku productCategory1:(NSString *)category1 productCategory2:(NSString *)category2 productCategory3:(NSString *)category3 productOriginPrice: (double)originPrice productRealPrice:(double)realPrice udVariable:(NSDictionary *)vars;
- (void)trackProductPage:(NSString *)sku productCategory1:(NSString *)category1 productCategory2:(NSString *)category2 productCategory3:(NSString *)category3 productOriginPrice: (double)originPrice productRealPrice:(double)realPrice;


/**
 Track event of adding product to cart.
 
 @param sku product sku.
 @param quantity product quantity.
 @param realPrice product real price.
 @param vars user defined variables.
 */
- (void)trackAddCart:(NSString *)sku productQuantity:(long)quantity productRealPrice:(double)realPrice udVariable:(NSDictionary *)vars;

/**
 Track event of adding product to cart.
 
 @param sku product sku.
 @param quantity product quantity.
 @param realPrice product real price.
 */
- (void)trackAddCart:(NSString *)sku productQuantity:(long)quantity productRealPrice:(double)realPrice;


/**
 Track a order.
 
 A order information as well as an optional list of items included in the transaction.
 
 @param order a object of DatatistOrderInfo.
 @param coupons an array of DatatistCouponInfo.
 @param products an array of DatatistProductInfo.
 @param vars user defined variables.
 @see DatatistOrderInfo
 @see DatatistCouponInfo
 @see DatatistProductInfo
 */
- (void)trackOrder:(DatatistOrderInfo *)order couponInfo:(NSArray *)coupons productInfo:(NSArray *)products udVariable:(NSDictionary *)vars;

/**
 Track a order.
 
 A order information as well as an optional list of items included in the transaction.
 
 @param order a object of DatatistOrderInfo.
 @param coupons an array of DatatistCouponInfo.
 @param products an array of DatatistProductInfo.
 @see DatatistOrderInfo
 @see DatatistCouponInfo
 @see DatatistProductInfo
 */
- (void)trackOrder:(DatatistOrderInfo *)order couponInfo:(NSArray *)coupons productInfo:(NSArray *)products;

/**
 Track a payment.
 
 @param orderId order id.
 @param method payment method.
 @param pay pay status.
 @param amt total amount
 @param vars user defined variables.
 */
- (void)trackPayment:(NSString *)orderId payMethod:(NSString *)method payStatus:(BOOL)pay payAMT:(double)amt udVariable:(NSDictionary *)vars;

/**
 Track a payment.
 
 @param orderId order id.
 @param method payment method.
 @param pay pay status.
 @param amt total amount
 */
- (void)trackPayment:(NSString *)orderId payMethod:(NSString *)method payStatus:(BOOL)pay payAMT:(double)amt;


/**
 Track a precharge.
 
 @param amt total amount.
 @param chargeMethod charge method.
 @param coupon coupon amount.
 @param pay pay status.
 @param vars user defined variables.
 */
- (void)trackPreCharge:(double)amt chargeMethod:(NSString *)chargeMethod couponAMT:(double)coupon payStatus:(BOOL)pay udVariable:(NSDictionary *)vars;

/**
 Track a precharge.
 
 @param amt total amount.
 @param chargeMethod charge method.
 @param coupon coupon amount.
 @param pay pay status.
 */
- (void)trackPreCharge:(double)amt chargeMethod:(NSString *)chargeMethod couponAMT:(double)coupon payStatus:(BOOL)pay;


/**
 Track user login event.
 
 @param uid user id.
 @param vars user defined variables.
 */
- (void)trackLogin:(NSString *)uid udVariable:(NSDictionary *)vars;
- (void)trackLogin:(NSString *)uid;

/**
 Track user logout event.
 
 @param uid user id.
 @param vars user defined variables.
 */
- (void)trackLogout:(NSString *)uid udVariable:(NSDictionary *)vars DatatistDeprecated("使用trackLogout");

//- (void)trackLogoutWithUid:(NSString *)uid;

- (void)trackLogout:(NSDictionary *)vars;

- (void)trackLogout;

/**
 *  track Event
 **/
- (void)trackEvent:(NSString *)name udVariable:(NSDictionary *)vars;

/**
 *  customer Track
 **/
- (void)customerTrack:(NSString*)name udVariable:(NSDictionary *)vars;

/**
 *  track JPush with vars
 **/
- (void)trackJPush:(NSDictionary *)pushInfo pushIntent:(NSDictionary *)pushIntent udVariable:(NSDictionary *)vars;

/**
 *  track JPush
 **/
- (void)trackJPush:(NSDictionary *)pushInfo pushIntent:(NSDictionary *)pushIntent;

/**
 *  track Init JPush with vars
 **/
- (void)trackInitJPush:(NSDictionary *)pushManager udVariable:(NSDictionary *)vars;

/**
 *  track Init JPush
 **/
- (void)trackInitJPush:(NSDictionary *)pushManager;

/**
 *  track Open Channel with vars
 **/
- (void)trackOpenChannel:(NSString *)openChannelName udVariable:(NSDictionary *)vars;

/**
 *  track Open Channel with vars
 **/
- (void)trackOpenChannel:(NSString *)openChannelName;

/**
 Manually start a dispatch of all pending events.
 
 @return YES if the dispatch process was started.
 */
- (BOOL)dispatch;

- (void)trackForbiddenController:(NSArray *)array;

/**
 设置忽略采集的UIControl

 @param array 例如要忽略 UIImageView，就把[UIImageView class]加入数组
 */
- (void)trackForbiddenControlClass:(NSArray *)array;

/**
 判断某个UIControl是否被忽略

 @param aClass 被忽略UIControl的Class
 @return YES:被忽略; NO:没有被忽略
 */
- (BOOL)isViewTypeForbidden:(Class)aClass;
- (BOOL)permittedController:(NSString *)controllerName;

- (BOOL)hasWebView:(UIView *)view;

- (void)enableTrack:(BOOL)enable;

/**
 是否上报GPS信息
 */
- (void)enableGPSTrack:(BOOL)enable;
/**
 是否开启上报第三方H5页面的ProjectId
 */
- (void)enableJSProjectIdTrack:(BOOL)enable;

- (void)trackJSEvent:(NSDictionary *)parameters;

- (void)trackClick:(NSDictionary *)parameters;

/**
 校验可视化圈选的schmeURL

 @param url schmeURL
 @return YES:匹配成功 NO:不匹配
 */
+ (BOOL)handleUrl:(NSURL*)url;
- (void)resetSiteId:(NSString *)siteId;

#if ABOVE_IOS_8_0 && WKWebView_Bridge
@property (nonatomic, weak) WebViewJavascriptBridge *bridge;
#endif

- (BOOL)sendTransaction:(DatatistTransaction*)transaction withCustomVariable:(NSDictionary *)vars;  
- (BOOL)sendEventWithCategory:(NSString*)category action:(NSString*)action name:(NSString*)name value:(NSString *)value withCustomVariable:(NSDictionary *)vars;
- (BOOL)sendWithCustomVariable:(NSDictionary *)vars Views:(NSString*)screen, ...;

@end
