//
//  DidaAppDelegate.h
//  cmoon
//
//  Created by li yajie on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DidaNetWorkEngine.h"
#import "CommomDefine.h"

@interface DidaAppDelegate : UIResponder <UIApplicationDelegate>

{
    DidaNetWorkEngine * engine;
}

@property (retain, nonatomic) UIWindow *window;

@property (assign,nonatomic) DidaNetWorkEngine * engine;
@end
