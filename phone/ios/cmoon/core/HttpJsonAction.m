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

    NSURL * url = [NSURL URLWithString:strURL];
    NSURLRequest * request = [NSURLRequest requestWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:60.0];

    AFJSONRequestOperation * operation = [AFJSONRequestOperation JSONRequestOperationWithRequest:request success:^(NSURLRequest *request, NSHTTPURLResponse *response, id JSON) {
        
        NSLog(@"%@",[JSON valueForKeyPath:@"ip"]);
    } failure:^(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id JSON) {
        NSLog(@"%@",[error description]);
    }];
    [operation start];
}
    
@end
