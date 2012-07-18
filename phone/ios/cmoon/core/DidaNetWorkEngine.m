//
//  DidaNetWorkEngine.m
//  cmoon
//
//  Created by li yajie on 12-7-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "DidaNetWorkEngine.h"

@implementation DidaNetWorkEngine

/**
 *Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*\*;q=0.8
* Accept-Charset:UTF-8,*;q=0.5
Accept-Encoding:gzip,deflate,sdch
Accept-Language:en-US,en;q=0.8
Cache-Control:max-age=0
Connection:keep-alive
 */
-(void) sendServerRequest:(NSString*) path withParam:(NSMutableDictionary*) params userMethod:(NSString*) method {
    MKNetworkOperation * op = [self operationWithPath:path params:nil httpMethod:method];

    
    [op onCompletion:^(MKNetworkOperation *completedOperation) {
        [self parseJsonData:completedOperation];  
    } onError:^(NSError *error) {
        [self alertError:error];
    }];
    [self enqueueOperation:op];
}
/**
 * @param 解析json数据
 */
-(void) parseJsonData:(MKNetworkOperation*) operation {
    NSLog(@"%@",[operation responseString]);
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

@end
