//
//  HttpSendAction.m
//  cmoon
//
//  Created by li yajie on 12-5-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "HttpSendAction.h"

@implementation HttpSendAction


+(void) sendHttp:(NSString *) url withParam:(NSString *)param {
    NSMutableString * targetURL = [NSMutableString stringWithString:url];
    [targetURL appendString:param];
    NSLog(@"%@",[targetURL description]);
    NSURL * target = [NSURL URLWithString:url];
    NSMutableURLRequest * request = [[[NSMutableURLRequest alloc]init]autorelease];
    
    [request setURL:target];
    [request setHTTPMethod:@"GET"];
    NSString *contentType = [NSString stringWithFormat:@"text/xml"]; 
    [request addValue:contentType forHTTPHeaderField:@"Content-Type"];  
    NSURLResponse * response = nil;
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:nil];
    NSString * result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"%@",result);
    [result release];
}   
@end
