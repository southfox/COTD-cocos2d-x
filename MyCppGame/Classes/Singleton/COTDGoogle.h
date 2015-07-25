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

class COTDGoogle
{
    const char* P_Items = "items";
    const char* P_Title = "title";
    const char* P_Link = "link";
    const char* P_Image = "image";
    const char* P_ThumbnailLink = "thumbnailLink";

public:
    static COTDGoogle* sharedInstance();

    void queryTerm(const std::string& term, const int& start);

    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

protected:
    COTDGoogle();
    
    bool succeeded;
    std::string link;
    std::string thumbnailLink;
    std::string title;
    std::string error;

private:
    static COTDGoogle* _instance;
};

#endif // __COTDGOOGLE_H__
