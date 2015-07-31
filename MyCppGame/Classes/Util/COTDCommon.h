//
//  COTDCommon.h
//  MyCppGame
//
//  Created by Javier Fuchs on 7/29/15.
//
//
#ifndef __CC_COMMON_H__
#define __CC_COMMON_H__

#import <Foundation/Foundation.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS

CC_DLL void COTDMessageBox(const char * title, const char * msg);

#endif


#endif // __CC_COMMON_H__

CC_DLL const char *encodeUrl(const char *url);
