//
//  DidaNetWorkEngine.m
//  cmoon
//
//  Created by li yajie on 12-7-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "DidaNetWorkEngine.h"

@implementation DidaNetWorkEngine

-(id)initWithHostName:(NSString *)hostName customHeaderFields:(NSDictionary *)headers {
    
//    [self setReachabilityChangedHandler:^(NetworkStatus status) {
//        NSLog(@"The status %d",status);
//    }];
    return [super initWithHostName:hostName customHeaderFields:headers];
    
}

/**
 *Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*\*;q=0.8
* Accept-Charset:UTF-8,*;q=0.5
Accept-Encoding:gzip,deflate,sdch
Accept-Language:en-US,en;q=0.8
Cache-Control:max-age=0
Connection:keep-alive
 */
-(void) sendServerRequest:(NSString*) path withParam:(NSMutableDictionary*) params userMethod:(NSString*) method {
//    if (![self isReachable]) {
//        UIAlertView * alertView = [[UIAlertView alloc]initWithTitle:@"警告" message:@"当前网络不可用" delegate:nil cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
//        [alertView show];
//        [alertView release];
//        return;
//    }
    
    
    MBProgressHUD *hub = [[MBProgressHUD alloc]initWithView:[UIApplication sharedApplication].inputView];
    MKNetworkOperation * op = [self operationWithPath:path params:nil httpMethod:method ];
    [op onCompletion:^(MKNetworkOperation *completedOperation) {
        [hub show:NO];
        [self parseJsonData:completedOperation]; 
    } onError:^(NSError *error) {
        [hub show:NO];
        [self alertError:error];
    }];
    [hub show:YES];
    [hub release];
    [self enqueueOperation:op];
}
/**
 * @param 解析json数据
 */
-(void) parseJsonData:(MKNetworkOperation*) operation {
    if ([operation isCachedResponse]) {
        //cached data
        NSString *jonData = [[operation responseString] JSONString];//直接清除原理数据
        NSLog(@"Cache %@",jonData);
        [operation cancel];
    } else {
        //if (cached == NO) {//追加数据
            //fresh data
            NSString *jonData = [[operation responseString] JSONString];
            NSLog(@"Server %@",jonData);
       // }
    }
}

/**
 * 将错误消息发送出去
 * @param error 
 */
-(void) alertError:(NSError*) error {
//    [error.description
    int errorCode = error.code;
    NSLog(@"get the error code = %d",errorCode);
    switch (errorCode) {
//        case Mk:
//            <#statements#>
//            break;
            
        default:
            break;
    }
    [[NSNotificationCenter defaultCenter] postNotificationName:@"networkrequesterror" object:error.description  userInfo:nil];
}

-(void)dealloc {
    [super dealloc];
}

@end
