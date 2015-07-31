//
//  COTDCommon.m
//  MyCppGame
//
//  Created by Javier Fuchs on 7/29/15.
//
//

#import "COTDCommon.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

void COTDMessageBox(const char * title, const char * msg)
{
    NSString * tmpTitle = (title) ? [NSString stringWithUTF8String : title] : nil;
    NSString * tmpMsg = (msg) ? [NSString stringWithUTF8String : msg] : nil;
    
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:tmpMsg];
    [alert setInformativeText:tmpTitle];
    [alert setAlertStyle:NSWarningAlertStyle];
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    id window = glview->getCocoaWindow();

    [alert beginSheetModalForWindow:window completionHandler:^(NSModalResponse returnCode) {
        
    }];

}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

void COTDMessageBox(const char * title, const char * msg)
{
    NSString * tmpTitle = (title) ? [NSString stringWithUTF8String : title] : nil;
    NSString * tmpMsg = (msg) ? [NSString stringWithUTF8String : msg] : nil;
    UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: tmpTitle
                                                          message: tmpMsg
                                                         delegate: nil
                                                cancelButtonTitle: @"OK"
                                                otherButtonTitles: nil];
    [messageBox autorelease];
    [messageBox show];
}

#endif


const char *encodeUrl(const char *url)
{
    NSString *urlString = [NSString stringWithUTF8String:url];
    urlString = [urlString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return [urlString UTF8String];
}