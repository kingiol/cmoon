//
//  HttpSendAction.h
//  cmoon
//
//  Created by li yajie on 12-5-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HttpSendAction : NSObject

+(void) sendHttp:(NSString *) url withParam:(NSString *)param;

@end
