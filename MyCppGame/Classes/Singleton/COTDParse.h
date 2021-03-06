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
    const char* P_updatedAt = "updatedAt";

public:
    static COTDParse* sharedInstance();
    ~COTDParse();

    void query(const ccParseCallback& callback);
    void likeCurrentImage(const ccParseCallback& callback);
    void queryImages();
    void queryUserImages();
    void queryTopTenImages(const ccImageVectorParseCallback& callback);
    void updateUserImage(const COTDUserImage &userImage, const ccParseCallback& callback);
    void updateImage(const std::string& imageUrl, const std::string& thumbnailUrl, const std::string& title, const std::string& searchTerm, const ccParseCallback& callback);

    ccParseCallback callbackQueryImages;
    ccImageVectorParseCallback callbackQueryTopTenImages;
    ccParseCallback callbackUpdateImage;
    ccParseCallback callbackUpdateUserImage;

    const COTDImage * currentUserImage();
    void incrementLikes();
    static void destroyInstance();
    bool isLinkRepeated(const std::string& fullUrl);

    int currentStart();
    const char * today() const;


protected:
    static COTDParse* _instance;
        
    COTDParse();
    
    cocos2d::ValueMap _configMap;
    
    COTDImage _sandboxImage;
    COTDUserImage _sandboxUserImage;

private:
    std::string configFileName();

    bool parseResponseFromAnonymousSignin(cocos2d::network::HttpResponse *response,
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
    
    bool parseResponse(cocos2d::network::HttpResponse *response,
                       std::strstream& error,
                       const std::string& key,
                       std::string& value);

    void queryImages(const cocos2d::network::ccHttpRequestCallback& callback, int limit = 1000, bool onlyLikes = false);
    void queryUserImages(const cocos2d::network::ccHttpRequestCallback& callback);
    void updateImage(const cocos2d::network::ccHttpRequestCallback& callback);
    void updateUserImage(const COTDUserImage &userImage, const cocos2d::network::ccHttpRequestCallback& callback);
    void getCurrentImage(const cocos2d::network::ccHttpRequestCallback& callback);
    void likeCurrentImage(const cocos2d::network::ccHttpRequestCallback& callback);

    void onHttpRequestCompletedAnonymousSignin(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedLogout(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedQueryImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedQueryTopTenImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedQueryUserImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedUpdateImage(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedUpdateUserImage(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedLikeCurrentImage(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void onHttpRequestCompletedQueryAndLikeCurrentUserImage(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    const char * applicationId();
    const char * apiKey();

    COTDImage::Vector images;
    COTDImage::Vector topTenImages;
    COTDUserImage::Vector userImages;

    const std::pair<bool, const COTDUserImage &> userImage();
    
    void anonymousSignin();
    void logout();
    
    const cocos2d::Value& atConfigKey(const std::string& key) const;
    std::string getUsername() const;
    std::string getUUID() const;
    std::string getObjectId() const;
    std::string getSessionToken() const;
    std::string createUUID();

    const std::pair<bool, const COTDImage *> isImageRepeated(const std::string& imageUrl);
    
    const std::string date(int n) const;
    const std::string now() const;
    const char * aWeekAgo() const;


};

#endif // __COTDPARSE_H__
