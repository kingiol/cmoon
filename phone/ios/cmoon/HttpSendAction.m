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
    NSURL * target = [NSURL URLWithString:targetURL];
    NSMutableURLRequest * request = [[[NSMutableURLRequest alloc]init]autorelease];
    
    [request setURL:target];
    [request setHTTPMethod:@"GET"];
    //NSString *contentType = [NSString stringWithFormat:@"text/xml"]; 
    //[request addValue:contentType forHTTPHeaderField:@"Content-Type"];  
/*    NSURLResponse * response = nil;
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:nil];
    NSString * result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"%@",result);
    [result release];
 */
    [[[NSURLConnection alloc]initWithRequest:request delegate:self]autorelease];
    
//    [conn release];
    
}   

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
    responseData = [NSMutableData dataWithLength:400];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    [responseData appendData:data];
}

-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    [responseData release];
    [connection release];
    NSLog(@"request the network encounter some error");
}
- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
    NSString * responseJson = [[NSString alloc]initWithData:responseData encoding:NSUTF8StringEncoding];
    
    NSLog(@"%@",responseJson);
    [responseJson release];
    [responseData release];
    [connection release];
}
@end
