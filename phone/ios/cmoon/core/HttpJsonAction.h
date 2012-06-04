//
//  HttpJsonAction.h
//  cmoon
//
//  Created by li yajie on 12-6-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AFNetworking.h"


@interface HttpJsonAction : NSObject

+(void) sendRequest: (NSString *) url;

@end
