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
#include "COTDCommon.h"
#include <uuid/uuid.h>
#include "cocostudio/DictionaryHelper.h"
using namespace cocostudio;

#define COTDIMAGE_URL                       "https://api.parse.com/1/classes/COTDImage"
#define COTDUSERIMAGE_URL                   "https://api.parse.com/1/classes/COTDUserImage"
#define LOGIN_URL                           "https://api.parse.com/1/users"
#define PARSEAPPLICATIONID_HEADER_FIELD     "X-Parse-Application-Id"
#define PARSERESTAPIKEY_HEADER_FIELD        "X-Parse-REST-API-Key"
#define PARSEAPPLICATIONID_HEADER_VALUE     "4w57EiBsbDCULkdlP5q1Q0R5bLPDupCbokbNT4KU"
#define PARSERESTAPIKEY_HEADER_VALUE        "IbCj3m1TlWMag98nQDDkv1nXUAvMN7PW6fNsbMYP"
#define PARSEREVOCABLESESSION_HEADER_FIELD  "X-Parse-Revocable-Session"

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
    delete _instance;
}


COTDParse::COTDParse()
{
    dbg << endl;
}

std::string COTDParse::createUUID()
{
    uuid_t uuid;
    uuid_generate_random(uuid);
    char s[37];
    uuid_unparse_lower(uuid, s);
    return s;
}

void COTDParse::setObjectForKey(const std::string &object, const std::string& key)
{
    auto conf = cocos2d::Configuration::getInstance();
    cocos2d::Value value = cocos2d::Value(object);
    dbg << "v = " << value.asString() << endl;
    conf->setValue(key, value);
}

const char * COTDParse::getObjectForKey(const std::string& key)
{
    auto conf = cocos2d::Configuration::getInstance();
    cocos2d::Value value = conf->getValue(key);
    if (value.isNull())
    {
        return nullptr;
    }
    return value.asString().c_str();
}


#define kUsernameKeyConfiguration "com.cotd.username"
#define kUUIDKeyConfiguration "com.cotd.UUID"
#define kObjectIdKeyConfiguration "com.cotd.objectId"
#define kSessionTokenKeyConfiguration "com.cotd.sessionToken"

const char *COTDParse::getUsername()
{
    return this->getObjectForKey(kUsernameKeyConfiguration);
}
void COTDParse::setUsername(const std::string &username)
{
    this->setObjectForKey(username, kUsernameKeyConfiguration);
}

void COTDParse::setUUID(const std::string &uuid)
{
    this->setObjectForKey(uuid, kUUIDKeyConfiguration);
}
const char *COTDParse::getUUID()
{
    return this->getObjectForKey(kUUIDKeyConfiguration);
}

void COTDParse::setObjectId(const std::string &objectId)
{
    this->setObjectForKey(objectId, kObjectIdKeyConfiguration);
}
const char *COTDParse::getObjectId()
{
    return this->getObjectForKey(kObjectIdKeyConfiguration);
}

void COTDParse::setSessionToken(const std::string &sessionToken)
{
    this->setObjectForKey(sessionToken, kSessionTokenKeyConfiguration);
}
const char *COTDParse::getSessionToken()
{
    return this->getObjectForKey(kSessionTokenKeyConfiguration);
}


void COTDParse::anonymousSignin()
{
    std::strstream error;
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    request->setUrl(LOGIN_URL);
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    const char *uuid = this->getUUID();
    if (uuid == nullptr)
    {
        uuid = this->createUUID().c_str();
        this->setUUID(uuid);
    }
    std::strstream buf;
    buf << "{ \"authData\": { \"anonymous\": { \"id\": \"" << uuid << "\" } } }" << '\0';
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


void COTDParse::query(const ccParseCallback& callback)
{
    this->anonymousSignin();

    this->callbackQueryImages = callback;

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


const char *COTDParse::applicationId()
{
    std::string header = PARSEAPPLICATIONID_HEADER_FIELD;
    header += ":";
    header += PARSEAPPLICATIONID_HEADER_VALUE;
    return header.c_str();
}


const char *COTDParse::apiKey()
{
    std::string header = PARSERESTAPIKEY_HEADER_FIELD;
    header += ":";
    header += PARSERESTAPIKEY_HEADER_VALUE;
    return header.c_str();
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

    dbg << "Url = [" << aux.str() << "]" << endl;
    request->setUrl(aux.str());
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

void COTDParse::queryUserImages(const cocos2d::network::ccHttpRequestCallback& callback)
{
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    
    std::strstream aux;
    aux << COTDUSERIMAGE_URL << "?user=" << this->getObjectId();
    COTDDate now;
    COTDDate aWeekAgo(-7);
//    COTDDate aWeekAgo(now.year(), now.month(), now.day()-7);
    aux << "&where={\"savedAt\":{\"$gte\":" << aWeekAgo << "}}";
    aux << '\0';
    
    dbg << "Url = [" << aux.str() << "]" << endl;
    request->setUrl(aux.str());
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
    
    std::vector<char> *buffer = response->getResponseData();
    std::string res;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        res += (*buffer)[i];
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
        error << "not exist " << P_results << '\0';
        return 0;
    }
    
    int count = DICTOOL->getArrayCount_json(json, P_results);
    
    if (!count)
    {
        error << "empty " << P_results << '\0';
        return 0;
    }

    return count;
}
    
bool COTDParse::parseResponseFromAnonymousSignin(cocos2d::network::HttpResponse *response,
                                                 std::string& id,
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
//    if (!DICTOOL->checkObjectExist_json(json, P_authData))
//    {
//        error << "not exist " << P_authData << '\0';
//        return false;
//    }
//    const rapidjson::Value& authData = DICTOOL->getSubDictionary_json(json, P_authData);
//    if (authData.IsNull())
//    {
//        error << "empty " << P_authData << '\0';
//        return false;
//    }
//    const rapidjson::Value& anonymous = DICTOOL->getSubDictionary_json(authData, P_anonymous);
//    if (anonymous.IsNull())
//    {
//        error << "empty " << P_authData << "->" << P_anonymous << '\0';
//        return false;
//    }
//    id = DICTOOL->getStringValue_json(anonymous, P_id);
//    if (!id.length())
//    {
//        error << "empty " << P_authData << "->" << P_anonymous << "->" << P_id << '\0';
//        return false;
//    }
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
        return false;
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
        
        COTDImage image(objectId, fullUrl, thumbnailUrl, imageTitle, likes);
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
        return false;
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
        const std::string& image = DICTOOL->getStringValue_json(resultsDict, P_objectId);
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
        if (!image.length())
        {
            error << "empty " << P_results << "->" << P_savedAt << "->" << P_iso << '\0';
            break;
        }
        
        std::size_t positionT = savedAtDateTime.find_last_of("T");
        const std::string& savedAt = savedAtDateTime.substr(0, positionT);
        
        COTDUserImage userImage(image, savedAt);
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

void COTDParse::onHttpRequestCompletedAnonymousSignin(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    std::string id;
    std::string objectId;
    std::string sessionToken;
    std::string username;

    succeeded = this->parseResponseFromAnonymousSignin(response, id, objectId, sessionToken, username, error);
    
    if (succeeded)
    {
        this->setUsername(username);
        this->setUUID(id);
        this->setObjectId(objectId);
        this->setSessionToken(sessionToken);
        this->setUsername(username);
        
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
    
    for (COTDImage::Vector::iterator iter = this->images.begin(); iter < this->images.end(); ++iter)
    {
        inf << "getFullUrl = " << iter->getFullUrl() << endl;
    }
}

void COTDParse::onHttpRequestCompletedQueryUserImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    if (this->callbackQueryImages)
    {
        succeeded = this->parseResponse(response, error, this->userImages);
        
        this->callbackQueryImages(succeeded, error);
    }
    
    for (COTDUserImage::Vector::iterator iter = this->userImages.begin(); iter < this->userImages.end(); ++iter)
    {
        inf << "getSavedAt = " << iter->getSavedAt() << endl;
    }
}


void COTDParse::onHttpRequestCompletedQueryTopTenImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::strstream error;
    
    if (this->callbackQueryImages)
    {
        succeeded = this->parseResponse(response, error, this->images);
        
        this->callbackQueryImages(succeeded, error);
    }
    
    for (COTDImage::Vector::iterator iter = this->images.begin(); iter < this->images.end(); ++iter)
    {
        inf << "getFullUrl = " << iter->getFullUrl() << endl;
    }
}


const std::pair<bool, const COTDUserImage &> COTDParse::userImage()
{
    const std::string & today = this->today();
    for (COTDUserImage::Vector::iterator iter = this->userImages.begin(); iter < this->userImages.end(); ++iter)
    {
        if (iter->getSavedAt() == today)
        {
            return std::make_pair(true, *iter);
        }
    }
    const auto userImage = COTDUserImage();
    return std::make_pair(false, userImage);
}

const char * COTDParse::today() const
{
    std::string todayStr;
    COTDDate((char*)"%F", todayStr);
    return todayStr.c_str();
}

const char * COTDParse::currentUserImageUrl()
{
    const auto userImage = this->userImage();
    if (!userImage.first)
    {
        return nullptr;
    }
    for (COTDImage::Vector::iterator iter = this->images.begin(); iter < this->images.end(); ++iter)
    {
        if (iter->getObjectId() == userImage.second.getImage())
        {
            return iter->getFullUrl().c_str();
        }
    }
    return nullptr;
}


