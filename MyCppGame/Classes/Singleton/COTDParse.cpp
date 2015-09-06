//
//  COTDParse.m
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#include "cocos2d.h"

#include "COTDLog.h"
#include <HttpClient.h>
#include <vector>
#include <HttpResponse.h>
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include "COTDImage.h"
#include "COTDUserImage.h"
#include "COTDDate.h"
#include "base/CCConfiguration.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "COTDCommon.h"
#include <uuid/uuid.h>
#else
#define encodeUrl(url) url
#endif
#include "cocostudio/DictionaryHelper.h"
using namespace cocostudio;

#define COTDIMAGE_URL                       "https://api.parse.com/1/classes/COTDImage"
#define COTDUSERIMAGE_URL                   "https://api.parse.com/1/classes/COTDUserImage"
#define LOGIN_URL                           "https://api.parse.com/1/users"
#define LOGOUT_URL                          "https://api.parse.com/1/logout"
#define PARSEAPPLICATIONID_HEADER_FIELD     "X-Parse-Application-Id"
#define PARSERESTAPIKEY_HEADER_FIELD        "X-Parse-REST-API-Key"
#define PARSESESSIONTOKEN_HEADER_FIELD      "X-Parse-Session-Token"
#define PARSEAPPLICATIONID_HEADER_VALUE     "4w57EiBsbDCULkdlP5q1Q0R5bLPDupCbokbNT4KU"
#define PARSERESTAPIKEY_HEADER_VALUE        "IbCj3m1TlWMag98nQDDkv1nXUAvMN7PW6fNsbMYP"
#define PARSEREVOCABLESESSION_HEADER_FIELD  "X-Parse-Revocable-Session"
#define CONFIGURATION_COTD_FILE             "cotd.plist"
#define OFFSET_DAY                          3

#include "COTDParse.h"

COTDParse* COTDParse::_instance = 0;

COTDParse* COTDParse::sharedInstance()
{
    if (_instance == 0) {
        _instance = new COTDParse;
    }
    return _instance;
}

COTDParse::~COTDParse()
{
    auto fu = cocos2d::FileUtils::getInstance();
    auto plistPath = this->configFileName();
    fu->writeToFile(this->configMap, plistPath);
}

void COTDParse::destroyInstance()
{
//    if (sharedInstance()->getSessionToken().length())
//    {
//        sharedInstance()->logout();
//    }
//    else
//    {
    CC_SAFE_DELETE(_instance);
//    }
}


COTDParse::COTDParse()
{
    dbg << endl;
    
    auto plistPath = this->configFileName();
    auto fu = cocos2d::FileUtils::getInstance();
    if (fu->isFileExist(plistPath))
    {
        this->configMap = fu->getValueMapFromFile(plistPath);
    }
}

std::string COTDParse::configFileName()
{
    auto fu = cocos2d::FileUtils::getInstance();
    auto plistPath = fu->getWritablePath() + CONFIGURATION_COTD_FILE;
    return plistPath;
}


std::string COTDParse::createUUID()
{
#if defined(ANDROID)
//    return [NSString stringWithFormat:"055aa316-706f-4326-90fc-5db03520c2b6";
    std::strstream str;
    for (int i = 0; i < 3; i++)
    {
        str << (int)arc4random()%10;
    }
    str << "aa";
    for (int i = 0; i < 3; i++)
    {
        str << (int)arc4random()%10;
    }
    str << "-";
    for (int i = 0; i < 3; i++)
    {
        str << (int)arc4random()%10;
    }
    str << "f-";
    for (int i = 0; i < 4; i++)
    {
        str << (int)arc4random()%10;
    }
    str << "-90fc-" << (int)arc4random()%10 << "db";
    for (int i = 0; i < 5; i++)
    {
        str << (int)arc4random()%10;
    }
    str >> "c2b6" << '\0';
    return (std::string)s.str();
#else
    uuid_t uuid;
    uuid_generate_random(uuid);
    char s[37];
    uuid_unparse_lower(uuid, s);
    return s;
#endif
}


const cocos2d::Value & COTDParse::atConfigKey(const std::string& key) const
{
    if (!this->configMap.size())
    {
        return cocos2d::Value::Null;
    }
    return this->configMap.at(key);
}


#define kUsernameKeyConfiguration "com.cotd.username"
#define kUUIDKeyConfiguration "com.cotd.UUID"
#define kObjectIdKeyConfiguration "com.cotd.objectId"
#define kSessionTokenKeyConfiguration "com.cotd.sessionToken"

std::string COTDParse::getUsername() const
{
    return this->atConfigKey(kUsernameKeyConfiguration).asString();
}
std::string COTDParse::getUUID() const
{
    return this->atConfigKey(kUUIDKeyConfiguration).asString();
}

std::string COTDParse::getObjectId() const
{
    return this->atConfigKey(kObjectIdKeyConfiguration).asString();
}

std::string COTDParse::getSessionToken() const
{
    return this->atConfigKey(kSessionTokenKeyConfiguration).asString();
}


void COTDParse::anonymousSignin()
{
    std::strstream error;
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    request->setUrl(LOGIN_URL);
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    std::string uuid = this->getUUID();
    if (!uuid.length())
    {
        uuid = this->createUUID();
        this->configMap[kUUIDKeyConfiguration] = uuid;
    }
    std::strstream buf;
    buf << "{ \"authData\": { \"anonymous\": { \"id\": \"" << uuid.c_str() << "\" } } }" << '\0';
    inf << buf.str() << endl;
    request->setRequestData(buf.str(), buf.pcount());
    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedAnonymousSignin, this));
    request->setTag("POST SIGN IN");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void COTDParse::logout()
{
    std::strstream error;
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    request->setUrl(LOGOUT_URL);
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    headers.push_back((std::string)PARSESESSIONTOKEN_HEADER_FIELD + ":" + this->getSessionToken());
    request->setHeaders(headers);

    request->setResponseCallback(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedLogout, this));
    request->setTag("POST SIGN IN");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


void COTDParse::query(const ccParseCallback& callback)
{
    this->callbackQueryImages = callback;

    this->anonymousSignin();
}

void COTDParse::likeCurrentImage(const ccParseCallback &callback)
{
    this->callbackQueryImages = callback;
    
    this->queryUserImages(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedQueryAndLikeCurrentUserImage, this));
    
}


void COTDParse::queryImages()
{
    dbg << endl;
    
    this->queryImages(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedQueryImages, this));
}

void COTDParse::queryUserImages()
{
    dbg << endl;
    
    this->queryUserImages(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedQueryUserImages, this));
}


void COTDParse::queryTopTenImages(const ccImageVectorParseCallback& callback)
{
    this->callbackQueryTopTenImages = callback;
    
    this->queryImages(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedQueryTopTenImages, this), 10, true);
}

void COTDParse::updateUserImage(const COTDUserImage &userImage, const ccParseCallback& callback)
{
    this->callbackUpdateImage = callback;
    this->updateUserImage(userImage, CC_CALLBACK_2(COTDParse::onHttpRequestCompletedUpdateUserImage, this));
}

void COTDParse::updateImage(const std::string& imageUrl, const std::string& thumbnailUrl, const std::string& title, const std::string& searchTerm, const ccParseCallback& callback)
{
    this->callbackUpdateImage = callback;

    auto image = this->isImageRepeated(imageUrl);
    if (image.first)
    {
        this->sandboxUserImage = COTDUserImage(image.second->getObjectId(), this->now());
        this->updateUserImage(this->sandboxUserImage, callback);
    }
    else
    {
        this->sandboxImage = COTDImage(imageUrl, thumbnailUrl, title);
        this->updateImage(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedUpdateImage, this));
    }

}

const std::pair<bool, const COTDImage *> COTDParse::isImageRepeated(const std::string& imageUrl)
{
    for (const auto &image : this->images)
    {
        if (image.getFullUrl() == imageUrl)
        {
            return std::make_pair(true, &image);
        }
    }
    return std::make_pair(false, nullptr);
}

const char *COTDParse::applicationId()
{
    std::string header = (std::string)PARSEAPPLICATIONID_HEADER_FIELD + ":" + PARSEAPPLICATIONID_HEADER_VALUE;
    return header.c_str();
}


const char *COTDParse::apiKey()
{
    return ((std::string)PARSERESTAPIKEY_HEADER_FIELD + ":" + PARSERESTAPIKEY_HEADER_VALUE).c_str();
}



void COTDParse::queryImages(const cocos2d::network::ccHttpRequestCallback& callback, int limit, bool onlyLikes)
{
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    std::strstream aux;
    aux << COTDIMAGE_URL << "?limit=" << limit;
    if (onlyLikes)
    {
        aux << "&where={\"likes\":{\"$gte\":0}}";
        aux << "&order=-likes";
    }
    aux << '\0';

    const char *urlEncoded = encodeUrl(aux.str());
    dbg << "Url = [" << urlEncoded << "]" << endl;
    request->setUrl(urlEncoded);

    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);

    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    request->setHeaders(headers);
    
    request->setResponseCallback(callback);
    request->setTag("GET COTD_IMAGE");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void COTDParse::getCurrentImage(const cocos2d::network::ccHttpRequestCallback& callback)
{
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    std::strstream aux;
    aux << COTDIMAGE_URL << "/" << this->currentUserImage()->getObjectId() << '\0';
    
    const char *urlEncoded = encodeUrl(aux.str());
    dbg << "Url = [" << urlEncoded << "]" << endl;
    request->setUrl(urlEncoded);
    
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    
    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    request->setHeaders(headers);
    
    request->setResponseCallback(callback);
    request->setTag("GET COTD_IMAGE/objectId");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


void COTDParse::queryUserImages(const cocos2d::network::ccHttpRequestCallback& callback)
{
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();

    COTDDate aWeekAgo(-7);
    std::string aWeekAgoStr = aWeekAgo.format((char*)"%FT%T");
    
    std::string objectId = this->getObjectId();
    std::strstream aux;
    aux << COTDUSERIMAGE_URL
        << "?where={\"user\":{\"__type\":\"Pointer\",\"className\":\"_User\",\"objectId\":\""
        << objectId.c_str()
        << "\"},\"savedAt\":{\"$gte\":{\"__type\":\"Date\",\"iso\":\""
        << aWeekAgoStr
        << "\"}}}"
        << '\0';
    
    const char *urlEncoded = encodeUrl(aux.str());
    
    dbg << "Url = [" << urlEncoded << "]" << endl;
    request->setUrl(urlEncoded);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    
    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    request->setHeaders(headers);
    
    request->setResponseCallback(callback);
    request->setTag("GET COTD_USER_IMAGE");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}




void COTDParse::likeCurrentImage(const cocos2d::network::ccHttpRequestCallback& callback)
{
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    std::strstream url;
    url << COTDIMAGE_URL << "/" << this->currentUserImage()->getObjectId() << '\0';
    
    const char *urlEncoded = encodeUrl(url.str());
    
    dbg << "Url = [" << urlEncoded << "]" << endl;
    request->setUrl(urlEncoded);
    request->setRequestType(cocos2d::network::HttpRequest::Type::PUT);
    
    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    request->setHeaders(headers);
    
    std::strstream aux;
    aux << "{"
    << "\"likes\": " << this->currentUserImage()->getLikes()+1
    << "}"
    << '\0';
    
    dbg << "Data = [" << aux.str() << "]" << endl;
    
    request->setRequestData(aux.str(), aux.pcount());
    
    request->setResponseCallback(callback);
    request->setTag("PUT COTD_IMAGE");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void COTDParse::updateImage(const cocos2d::network::ccHttpRequestCallback& callback)
{
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    const char *urlEncoded = encodeUrl(COTDIMAGE_URL);
    
    dbg << "Url = [" << urlEncoded << "]" << endl;
    request->setUrl(urlEncoded);
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    
    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    request->setHeaders(headers);
    
    std::strstream aux;
    aux << "{"
        << "\"fullUrl\": \"" << this->sandboxImage.getFullUrl().c_str() << "\","
        << "\"imageTitle\": \"" << this->sandboxImage.getImageTitle().c_str() << "\","
        << "\"likes\": 0,"
        << "\"thumbnailUrl\": \"" << this->sandboxImage.getThumbnailUrl().c_str() << "\""
        << "}"
        << '\0';

    dbg << "Data = [" << aux.str() << "]" << endl;
    
    request->setRequestData(aux.str(), aux.pcount());
    
    request->setResponseCallback(callback);
    request->setTag("POST COTD_IMAGE");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


void COTDParse::updateUserImage(const COTDUserImage &userImage, const cocos2d::network::ccHttpRequestCallback& callback)
{
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    const char *urlEncoded = encodeUrl(COTDUSERIMAGE_URL);
    
    dbg << "Url = [" << urlEncoded << "]" << endl;
    request->setUrl(urlEncoded);
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    
    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    request->setHeaders(headers);
    
    std::strstream aux;
    aux << "{\"image\":{\"__type\":\"Pointer\",\"className\":\"COTDImage\",\"objectId\":\"" << userImage.getImage().c_str() << "\"},"
        << "\"savedAt\":{\"__type\":\"Date\",\"iso\":\"" << userImage.getSavedAt().c_str() << "\"},"
        << "\"user\":{\"__type\":\"Pointer\",\"className\":\"_User\",\"objectId\":\"" << this->getObjectId() << "\"}}" << '\0';

    dbg << "Data = [" << aux.str() << "]" << endl;

    request->setRequestData(aux.str(), aux.pcount());
    
    request->setResponseCallback(callback);
    request->setTag("GET COTD_USER_IMAGE");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


bool COTDParse::parseResponse(cocos2d::network::HttpResponse *response,
                              std::strstream& error,
                              rapidjson::Document &json)
{
    if (!response)
    {
        error << "no response" << '\0';
        return false;
    }
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        dbg << response->getHttpRequest()->getTag() << " completed" << endl;
    }
    
    long statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
    dbg << "statusString: " << statusString << endl;
    
    dbg << "response code:" << statusCode << endl;
    
    if (!response->isSucceed())
    {
        err << "response failed" << endl;
        err << "error buffer:" << response->getErrorBuffer() << endl;
        error << response->getErrorBuffer() << '\0';
        return false;
    }
    
    std::string res;
    for (const auto &character : *response->getResponseData())
    {
        res += character;
    }
    dbg << "COTDParse::onHttpRequestCompleted() dump data:" << "{\n" << res.c_str() << "}\n" << endl;
    
    rapidjson::StringStream stream(res.c_str());
    
    json.ParseStream<0>(stream);
    if (json.HasParseError()) {
        err << "GetParseError " << json.GetParseError() << endl;
        error << json.GetParseError() << '\0';
        return false;
    }
    return true;
}

int COTDParse::countElements(std::strstream& error,
                             rapidjson::Document &json)
{
    if (!DICTOOL->checkObjectExist_json(json, P_results))
    {
        return 0;
    }
    
    int count = DICTOOL->getArrayCount_json(json, P_results);
    
    return count;
}
    
bool COTDParse::parseResponseFromAnonymousSignin(cocos2d::network::HttpResponse *response,
                                                 std::string& objectId,
                                                 std::string& sessionToken,
                                                 std::string& username,
                                                 std::strstream& error)
{
    rapidjson::Document json;
    if (!this->parseResponse(response, error, json))
    {
        return false;
    }
    objectId = DICTOOL->getStringValue_json(json, P_objectId);
    if (!objectId.length())
    {
        error << "empty " << P_objectId << '\0';
        return false;
    }
    sessionToken = DICTOOL->getStringValue_json(json, P_sessionToken);
    if (!sessionToken.length())
    {
        error << "empty " << P_sessionToken << '\0';
        return false;
    }
    username = DICTOOL->getStringValue_json(json, P_username);
    if (!username.length())
    {
        error << "empty " << P_username << '\0';
        return false;
    }

    return true;
}



bool COTDParse::parseResponse(cocos2d::network::HttpResponse *response,
                              std::strstream& error,
                              COTDImage::Vector &vector)
{
    rapidjson::Document json;
    if (!this->parseResponse(response, error, json))
    {
        return false;
    }
    
    int count = this->countElements(error, json);
    if (!count)
    {
        return true;
    }
    
    int iterator = 0;
    for (;iterator < count; iterator++)
    {
        const rapidjson::Value &resultsDict = DICTOOL->getDictionaryFromArray_json(json, P_results, iterator);
        
        const std::string& fullUrl = DICTOOL->getStringValue_json(resultsDict, P_fullUrl);
        if (!fullUrl.length())
        {
            error << "empty " << P_results << "->" << P_fullUrl << '\0';
            break;
        }
        
        const std::string& imageTitle = DICTOOL->getStringValue_json(resultsDict, P_imageTitle);
        if (!imageTitle.length())
        {
            error << "empty " << P_results << "->" << P_imageTitle << '\0';
            break;
        }
        
        int likes = DICTOOL->getIntValue_json(resultsDict, P_likes);
        
        const std::string& objectId = DICTOOL->getStringValue_json(resultsDict, P_objectId);
        if (!objectId.length())
        {
            error << "empty " << P_results << "->" << P_objectId << '\0';
            break;
        }
        
        const std::string& thumbnailUrl = DICTOOL->getStringValue_json(resultsDict, P_thumbnailUrl);
        if (!thumbnailUrl.length())
        {
            error << "empty " << P_results << "->" << P_thumbnailUrl << '\0';
            break;
        }
        
        COTDImage image(fullUrl, thumbnailUrl, imageTitle, likes, objectId);
        dbg << "#" << iterator << ": " << image << endl;
        vector.push_back(image);
    }
    
    if (iterator)
    {
        if (error.pcount())
        {
            err << error.str() << endl;
        }
        error.clear();
        return true;
    }
    
    return false;
}


bool COTDParse::parseResponse(cocos2d::network::HttpResponse *response,
                              std::strstream& error,
                              std::string& objectId)
{
    rapidjson::Document json;
    if (!this->parseResponse(response, error, json))
    {
        return false;
    }
    objectId = DICTOOL->getStringValue_json(json, P_objectId);
    if (!objectId.length())
    {
        error << "empty " << P_objectId << '\0';
        return false;
    }
    
    return true;
}



bool COTDParse::parseResponse(cocos2d::network::HttpResponse *response,
                              std::strstream& error,
                              COTDUserImage::Vector &vector)
{
    rapidjson::Document json;
    if (!this->parseResponse(response, error, json))
    {
        return false;
    }
    
    int count = this->countElements(error, json);
    if (!count)
    {
        return true;
    }
    
    int iterator = 0;
    for (;iterator < count; iterator++)
    {
        const rapidjson::Value &resultsDict = DICTOOL->getDictionaryFromArray_json(json, P_results, iterator);
        if (resultsDict.IsNull())
        {
            error << "null element " << iterator << " in array " << P_results << '\0';
            break;
        }
        
        const rapidjson::Value& imageDict = DICTOOL->getSubDictionary_json(resultsDict, P_image);
        if (imageDict.IsNull())
        {
            error << "empty " << P_results << "->" << P_image << '\0';
            break;
        }
        const std::string& image = DICTOOL->getStringValue_json(imageDict, P_objectId);
        if (!image.length())
        {
            error << "empty " << P_results << "->" << P_image << "->" << P_objectId << '\0';
            break;
        }
        
        const rapidjson::Value& savedAtDict = DICTOOL->getSubDictionary_json(resultsDict, P_savedAt);
        if (savedAtDict.IsNull())
        {
            error << "empty " << P_results << "->" << P_savedAt << '\0';
            break;
        }
        
        const std::string& savedAtDateTime = DICTOOL->getStringValue_json(savedAtDict, P_iso);
        if (!savedAtDateTime.length())
        {
            error << "empty " << P_results << "->" << P_savedAt << "->" << P_iso << '\0';
            break;
        }
        
        COTDUserImage userImage(image, savedAtDateTime);
        dbg << "#" << iterator << ": " << userImage << endl;
        vector.push_back(userImage);
    }
    
    if (iterator)
    {
        if (error.pcount())
        {
            err << error.str() << endl;
        }
        error.clear();
        return true;
    }
    
    return false;
}

void COTDParse::onHttpRequestCompletedLogout(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    rapidjson::Document json;
    succeeded = this->parseResponse(response, error, json);
    
    if (succeeded)
    {
        this->configMap.erase(kSessionTokenKeyConfiguration);
    }
    
    this->destroyInstance();
}

void COTDParse::onHttpRequestCompletedAnonymousSignin(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    std::string objectId;
    std::string sessionToken;
    std::string username;

    succeeded = this->parseResponseFromAnonymousSignin(response, objectId, sessionToken, username, error);
    
    if (succeeded)
    {
        this->configMap[kObjectIdKeyConfiguration] = objectId;
        this->configMap[kSessionTokenKeyConfiguration] = sessionToken;
        this->configMap[kUsernameKeyConfiguration] = username;
        
        this->queryImages();
    }
    else
    {
        this->callbackQueryImages(succeeded, error);
    }
}


void COTDParse::onHttpRequestCompletedQueryImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    if (this->callbackQueryImages)
    {
        succeeded = this->parseResponse(response, error, this->images);
        
        this->queryUserImages();
    }
}

void COTDParse::onHttpRequestCompletedQueryUserImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    if (this->callbackQueryImages)
    {
        this->userImages.clear();
        succeeded = this->parseResponse(response, error, this->userImages);
        
        this->callbackQueryImages(succeeded, error);
    }
    
    for (const auto &userImage : this->userImages)
    {
        inf << "getSavedAt = " << userImage.getSavedAt() << endl;
    }
    
}

void COTDParse::onHttpRequestCompletedQueryAndLikeCurrentUserImage(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    if (this->callbackQueryImages)
    {
        this->userImages.clear();
        succeeded = this->parseResponse(response, error, this->userImages);
        
        if (succeeded)
        {
            this->likeCurrentImage(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedLikeCurrentImage, this));
        }
        else
        {
            this->callbackQueryImages(succeeded, error);
        }
    }
}


void COTDParse::onHttpRequestCompletedUpdateUserImage(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    std::string objectId;
    succeeded = this->parseResponse(response, error, objectId);
    
    if (succeeded)
    {
        this->userImages.push_back(this->sandboxUserImage);
    }
    
    if (this->callbackUpdateImage)
    {
        this->callbackUpdateImage(succeeded, error);
    }
}

void COTDParse::onHttpRequestCompletedUpdateImage(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    std::string objectId;
    succeeded = this->parseResponse(response, error, objectId);
    
    if (succeeded)
    {
        this->sandboxImage.setObjectId(objectId);
        this->images.push_back(this->sandboxImage);
        this->sandboxUserImage = COTDUserImage(this->sandboxImage.getObjectId(), this->now());
    }
    if (this->callbackUpdateImage)
    {
        if (succeeded)
        {
            this->updateUserImage(this->sandboxUserImage, this->callbackUpdateImage);
        }
        else
        {
            this->callbackUpdateImage(succeeded, error);
        }
    }
}



void COTDParse::onHttpRequestCompletedLikeCurrentImage(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    std::string objectId;
    succeeded = this->parseResponse(response, error, objectId);
    
    if (this->callbackQueryImages)
    {
        this->callbackQueryImages(succeeded, error);
    }
}

void COTDParse::onHttpRequestCompletedQueryTopTenImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    if (this->callbackQueryTopTenImages)
    {
        succeeded = this->parseResponse(response, error, this->topTenImages);
        
        this->callbackQueryTopTenImages(succeeded, error, this->topTenImages);
    }
    
    for (const auto &image : this->images)
    {
        inf << "getFullUrl = " << image.getFullUrl() << endl;
    }
}


const std::pair<bool, const COTDUserImage &> COTDParse::userImage()
{
    const std::string & today = this->today();
    for (const auto &userImage : this->userImages)
    {
        if (userImage.getDateSavedAt() == today)
        {
            return std::make_pair(true, userImage);
        }
    }
    return std::make_pair(false, COTDUserImage());
}


const COTDImage * COTDParse::currentUserImage()
{
    const auto userImage = this->userImage();
    if (!userImage.first)
    {
        return nullptr;
    }
    for (const auto &image : this->images)
    {
        if (image.getObjectId() == userImage.second.getImage())
        {
            return &image;
        }
    }
    return nullptr;
}

bool COTDParse::isLinkRepeated(const std::string& fullUrl)
{
    for (const auto &userImage : this->userImages)
    {
        for (const auto &image : this->images)
        {
            if (image.getObjectId() == userImage.getImage())
            {
                if (image.getFullUrl() == fullUrl)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

const std::string COTDParse::date(int n) const
{
    COTDDate now(n+OFFSET_DAY);
    std::string nowStr = now.format((char*)"%FT%T");
    return nowStr;
}

const std::string COTDParse::now() const
{
    return this->date(0);
}

const char * COTDParse::aWeekAgo() const
{
    auto now = this->date(-7+OFFSET_DAY);
    std::size_t positionT = now.find_last_of("T");
    const std::string aWeekAgoStr = now.substr(0, positionT);
    return aWeekAgoStr.c_str();
}

const char * COTDParse::today() const
{
    auto now = this->now();
    std::size_t positionT = now.find_last_of("T");
    const std::string todayStr = now.substr(0, positionT);
    return todayStr.c_str();
}

int COTDParse::currentStart()
{
    int num = (int)this->userImages.size() + (int)this->images.size() + 1;
    int r = num + arc4random()%15;
    return r;
}



