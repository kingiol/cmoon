//
//  DidaAppDelegate.m
//  cmoon
//
//  Created by li yajie on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "DidaAppDelegate.h"
#import "DidaNetWorkEngine.h"
#import "DidaViewController.h"

@implementation DidaAppDelegate

@synthesize window = _window;
@synthesize engine = _engine;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    _window = [[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
    [self initNetworkEngine];
    [engine setReachabilityChangedHandler:networkChangedListener];
    // Override point for customization after application launch.
    DidaViewController *controller = [[DidaViewController alloc ]init];
    UINavigationController * navigationController = [[UINavigationController alloc] initWithRootViewController:controller];
    [controller release];
    [_window addSubview:navigationController.view];
    [navigationController release];
    [_window makeKeyAndVisible];
    return YES;
}
/**
 * 网络变化的代码块
 * 会有问题
 */
void (^networkChangedListener)(NetworkStatus netstatus) = ^(NetworkStatus nstatus) {
        switch(nstatus) {
            case NotReachable://网络不可用
        {
            //显示一个不可用的视图
        }
            break;
            case ReachableViaWiFi:
                break;
            case ReachableViaWWAN://手机网络
                break;
                default:
                break;
    }
};

/**
 * 初始化网络引擎
 */
-(void) initNetworkEngine {
    @try {
        NSMutableDictionary *headerFields = [NSMutableDictionary dictionary];
        [headerFields setValue:@"UTF-8,*;q=0.5" forKey:@"Accept-Charset"];
        [headerFields setValue:@"text/html,application/xhtml+xml,application/xml,application/json;q=0.9,*'\'*;q=0.8" forKey:@"Accept"];
        [headerFields setValue:@"IOS" forKey:@"User-Agent"];
        [headerFields setValue:@"gzip,deflate,sdch" forKey:@"Accept-Encoding"];
        engine = [[DidaNetWorkEngine alloc] initWithHostName:SERVER_HOST customHeaderFields:headerFields];
        [engine useCache];
        [headerFields release];
    }
    @catch (NSException *exception) {
        NSLog(@"The Network initial exception %@",exception.description);
    }
    
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    [engine emptyCache];
    [engine release];
}


@end
