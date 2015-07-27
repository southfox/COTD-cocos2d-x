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
typedef std::function<void(bool, const std::string&, const COTDImage::Vector&)> ccTopTenParseCallback;

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
    ~COTDParse();

    void query(const ccParseCallback& callback);
    void queryTopTenImages(const ccTopTenParseCallback& callback);

    ccParseCallback callbackQueryImages;
    ccTopTenParseCallback callbackQueryTopTenImages;

protected:
    static COTDParse* _instance;
        
    COTDParse();
    
private:
    bool parseResponse(cocos2d::network::HttpResponse *response,
                       std::string& error,
                       COTDImage::Vector &vector);
    
    void queryImages(const cocos2d::network::ccHttpRequestCallback& callback, int limit = 1000, bool onlyLikes = false);

    void onHttpRequestCompletedQueryImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedQueryTopTenImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    const char * applicationId();
    const char * apiKey();

    COTDImage::Vector images;

};

#endif // __COTDPARSE_H__
