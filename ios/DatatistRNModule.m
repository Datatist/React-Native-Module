//
//  DatatistRNModule.m
//
//  Created by IOS01 on 2018/12/18.
//  Copyright © 2018 IOS01. All rights reserved.
//

#import "DatatistRNModule.h"
#import "DatatistTracker.h"

@implementation DatatistRNModule

RCT_EXPORT_MODULE(DatistRNModule);

RCT_EXPORT_METHOD(customerTrack:(NSString *)eventName udVariable:(NSDictionary *)var)
{
    [[DatatistTracker sharedInstance] customerTrack:eventName udVariable:var];
}

RCT_EXPORT_METHOD(trackPageview:(NSString *)page title:(NSString *)title)
{
    [[DatatistTracker sharedInstance] trackPageView:page title:title];
}

RCT_EXPORT_METHOD(setUserId:(NSString *)userId)
{
    [[DatatistTracker sharedInstance] setUserID:userId];
}

RCT_EXPORT_METHOD(setUserProperties:(NSDictionary *)property)
{
    [[DatatistTracker sharedInstance] setUserProperty:property];
}

RCT_EXPORT_METHOD(trackLogin:(NSString *)userId)
{
    [[DatatistTracker sharedInstance] trackLogin:userId];
}

RCT_EXPORT_METHOD(trackRegister:(NSString *)uid userType:(NSString *)type authenticated:(BOOL)authenticated )
{
    [[DatatistTracker sharedInstance] trackRegister:uid type:type authenticated:authenticated];
}

RCT_EXPORT_METHOD(trackLogout)
{
    [[DatatistTracker sharedInstance] trackLogout];
}
@end
