//
//  DidaNetWorkEngine.m
//  cmoon
//
//  Created by li yajie on 12-7-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "DidaNetWorkEngine.h"

@implementation DidaNetWorkEngine
- (id)init
{
    self = [super init];
    if (self) {
        
    }
    [self useCache];//使用缓存
    return self;
}


-(void) sendServerRequest:(NSMutableDictionary*) params userMethod:(NSString*) method {
    MKNetworkOperation * op = [self operationWithPath:API_SERVER params:params httpMethod:method];
    NSMutableDictionary *headerFields = [NSMutableDictionary dictionary];
    [headerFields setValue:@"iOS" forKey:@"x-client-identifier"];
    [op addHeaders:headerFields];
   // op addHeaders:<#(NSDictionary *)#>
    [op onCompletion:^(MKNetworkOperation *completedOperation) {
        [self parseJsonData:completedOperation];  
    } onError:^(NSError *error) {
        [self alertError:error];
    }];
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
