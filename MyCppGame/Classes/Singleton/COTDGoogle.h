//
//  COTDGoogle.h
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#ifndef __COTDGOOGLE_H__
#define __COTDGOOGLE_H__

#include "network/HttpClient.h"
typedef std::function<void(bool, const std::string &, const std::string&, const std::string&, const std::string&)> ccGoogleCallback;

class COTDGoogle
{
    const char* P_Items = "items";
    const char* P_Title = "title";
    const char* P_Link = "link";
    const char* P_Image = "image";
    const char* P_ThumbnailLink = "thumbnailLink";

public:
    static COTDGoogle* sharedInstance();

    void queryTerm(const std::string& term, const int& start, const ccGoogleCallback& callback);

    ccGoogleCallback callback;

protected:
    static COTDGoogle* _instance;
    
    COTDGoogle();
    
private:
    bool parseResponse(cocos2d::network::HttpResponse *response,
                       std::string& link,
                       std::string& thumbnailLink,
                       std::string& title,
                       std::string& error);
    
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

};

#endif // __COTDGOOGLE_H__
