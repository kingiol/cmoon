//
//  DidaAppDelegate.h
//  cmoon
//
//  Created by li yajie on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "DidaNetWorkEngine.h"
#import "CommomDefine.h"
#import "ArcHelper.h"

@interface DidaAppDelegate : UIResponder <UIApplicationDelegate,CLLocationManagerDelegate,UITabBarControllerDelegate>

{
    CLLocationCoordinate2D curLoc;//当前位置信息
    CLLocationManager * mLocationMgr;//位置管理操作类
}

@property (retain, nonatomic) UIWindow *window;

@property (retain,nonatomic) DidaNetWorkEngine * engine;

@property (retain,nonatomic) CLLocationManager* mLocationMgr;

@property (nonatomic,assign) CLLocationCoordinate2D curLoc; 

@property (nonatomic,retain) UITabBarController *tabeViewController;

@end
