//
//  HttpJsonAction.m
//  cmoon
//
//  Created by li yajie on 12-6-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "HttpJsonAction.h"

@implementation HttpJsonAction

+(void)sendRequest:(NSString *)strURL {
    [strURL stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    
}
    
@end
