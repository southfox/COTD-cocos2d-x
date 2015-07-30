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
#include "COTDUserImage.h"
#include "json/document.h"
#include <strstream>

typedef std::function<void(bool, std::strstream&)> ccParseCallback;
typedef std::function<void(bool, std::strstream&, const COTDImage::Vector&)> ccImageVectorParseCallback;
typedef std::function<void(bool, std::strstream&, const COTDUserImage::Vector&)> ccUserImageVectorParseCallback;

class COTDParse
{
    const char* P_results = "results";
    const char* P_fullUrl = "fullUrl";
    const char* P_imageTitle = "imageTitle";
    const char* P_likes = "likes";
    const char* P_objectId = "objectId";
    const char* P_thumbnailUrl = "thumbnailUrl";
    const char* P_image = "image";
    const char* P_user = "user";
    const char* P_savedAt = "savedAt";
    const char* P_iso = "iso";
    const char* P_authData = "authData";
    const char* P_anonymous = "anonymous";
    const char* P_id = "id";
    const char* P_sessionToken = "sessionToken";
    const char* P_username = "username";

public:
    static COTDParse* sharedInstance();
    ~COTDParse();

    void query(const ccParseCallback& callback);
    void queryImages();
    void queryUserImages();
    void queryTopTenImages(const ccImageVectorParseCallback& callback);

    ccParseCallback callbackQueryImages;
    ccImageVectorParseCallback callbackQueryTopTenImages;

    const char * currentUserImageUrl();

protected:
    static COTDParse* _instance;
        
    COTDParse();

private:
    bool parseResponseFromAnonymousSignin(cocos2d::network::HttpResponse *response,
                                          std::string& id,
                                          std::string& objectId,
                                          std::string& sessionToken,
                                          std::string& username,
                                          std::strstream& error);
    bool parseResponse(cocos2d::network::HttpResponse *response,
                       std::strstream& error,
                       rapidjson::Document &json);
    int countElements(std::strstream& error,
                      rapidjson::Document &json);

    bool parseResponse(cocos2d::network::HttpResponse *response,
                       std::strstream& error,
                       COTDImage::Vector &vector);
    
    bool parseResponse(cocos2d::network::HttpResponse *response,
                       std::strstream& error,
                       COTDUserImage::Vector &vector);
    
    void queryImages(const cocos2d::network::ccHttpRequestCallback& callback, int limit = 1000, bool onlyLikes = false);
    void queryUserImages(const cocos2d::network::ccHttpRequestCallback& callback);

    void onHttpRequestCompletedAnonymousSignin(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedQueryImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedQueryTopTenImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedQueryUserImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    const char * applicationId();
    const char * apiKey();

    COTDImage::Vector images;
    COTDUserImage::Vector userImages;

    const std::pair<bool, const COTDUserImage &> userImage();
    
    const char * today() const;
    
    void anonymousSignin();
    
    void setObjectForKey(const std::string &object, const std::string& key);
    const char * getObjectForKey(const std::string& key);
    const char *getUsername();
    void setUsername(const std::string &username);
    const char *getUUID();
    void setUUID(const std::string &uuid);
    
    void setObjectId(const std::string &objectId);
    const char *getObjectId();
    void setSessionToken(const std::string &sessionToken);
    const char *getSessionToken();
    
    std::string createUUID();

};

#endif // __COTDPARSE_H__
