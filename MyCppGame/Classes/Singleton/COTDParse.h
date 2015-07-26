//
//  COTDParse.h
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#ifndef __COTDPARSE_H__
#define __COTDPARSE_H__

#include "network/HttpClient.h"
#include "COTDImage.h"

typedef std::function<void(bool, const std::string&)> ccParseCallback;

class COTDParse
{
    const char* P_results = "results";
    const char* P_fullUrl = "fullUrl";
    const char* P_imageTitle = "imageTitle";
    const char* P_likes = "likes";
    const char* P_objectId = "objectId";
    const char* P_thumbnailUrl = "thumbnailUrl";

public:
    static COTDParse* sharedInstance();

    void query(const ccParseCallback& callback);

    ccParseCallback callback;

#if 0
- (void)configureWithLaunchOptions:(NSDictionary *)launchOptions finishBlock:(void (^)(BOOL succeeded, NSError *error))finishBlock;

- (NSString *)currentUserSearchTerm;
- (void)changeCurrentUserSearchTerm:(NSString *)searchTerm;

- (NSUInteger)currentStart;

- (NSString *)currentUserImageTitle;

- (NSString *)currentUserImageUrl;

- (BOOL)isLinkRepeated:(NSString *)fullUrl;

- (void)likeCurrentImage:(void (^)(BOOL succeeded, NSError *error))finishBlock;

- (void)updateImage:(NSString *)imageUrl thumbnailUrl:(NSString *)thumbnailUrl title:(NSString *)title searchTerm:(NSString *)searchTerm finishBlock:(void (^)(BOOL succeeded, COTDImage* image, NSError *error))finishBlock;

- (void)topTenImages:(void (^)(NSArray *objects, NSError *error))finishBlock;

#endif

protected:
    static COTDParse* _instance;
        
    COTDParse();
    
private:
    bool parseResponse(cocos2d::network::HttpResponse *response,
                       std::string& error);
    
    void onHttpRequestCompletedImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

    void queryImages();
    
    const char * applicationId();
    const char * apiKey();

    COTDImage::Vector images;

};

#endif // __COTDPARSE_H__
