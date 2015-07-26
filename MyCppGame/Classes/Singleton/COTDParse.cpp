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

#include "cocostudio/DictionaryHelper.h"
using namespace cocostudio;

#include "COTDParse.h"

COTDParse* COTDParse::_instance = 0;

COTDParse* COTDParse::sharedInstance()
{
    if (_instance == 0) {
        _instance = new COTDParse;
    }
    return _instance;
}

bool COTDParse::parseResponse(cocos2d::network::HttpResponse *response,
                               std::string& error)
{
    if (!response)
    {
        error = "no response";
        return false;
    }
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        dbg << response->getHttpRequest()->getTag() << " completed";
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
        error = response->getErrorBuffer();
        return false;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string res;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        res += (*buffer)[i];
    }
    dbg << "COTDParse::onHttpRequestCompleted() dump data:" << "{\n" << res.c_str() << "}\n" << endl;
    
    rapidjson::Document json;
    rapidjson::StringStream stream(res.c_str());
    
    json.ParseStream<0>(stream);
    if (json.HasParseError()) {
        err << "GetParseError " << json.GetParseError() << endl;
        error = json.GetParseError();
        return false;
    }
    
    if (!DICTOOL->checkObjectExist_json(json, P_results))
    {
        error = "not exist ";
        error += P_results;
        return false;
    }
    
    int count = DICTOOL->getArrayCount_json(json, P_results);
    
    if (!count)
    {
        error = "empty ";
        error += P_results;
        return false;
    }

    int iterator = 0;
    for (;iterator < count; iterator++)
    {
        const rapidjson::Value &resultsDict = DICTOOL->getDictionaryFromArray_json(json, P_results, iterator);
        
        std::string fullUrl = DICTOOL->getStringValue_json(resultsDict, P_fullUrl);
        if (!fullUrl.length())
        {
            error = "empty ";
            error += P_fullUrl;
            break;
        }
        
        std::string imageTitle = DICTOOL->getStringValue_json(resultsDict, P_imageTitle);
        if (!imageTitle.length())
        {
            error = "empty ";
            error += P_imageTitle;
            break;
        }
        
        int likes = DICTOOL->getIntValue_json(resultsDict, P_likes);

        std::string objectId = DICTOOL->getStringValue_json(resultsDict, P_objectId);
        if (!objectId.length())
        {
            error = "empty ";
            error += P_objectId;
            break;
        }
        
        std::string thumbnailUrl = DICTOOL->getStringValue_json(resultsDict, P_thumbnailUrl);
        if (!thumbnailUrl.length())
        {
            error = "empty ";
            error += P_thumbnailUrl;
            break;
        }
        
        dbg << "#: [" << iterator << "] - "
            << "fullUrl: [" << fullUrl << "] - "
            << "imageTitle: [" << imageTitle << "]"
            << "likes: [" << likes << "]"
            << "objectId: [" << objectId << "]"
            << "thumbnailUrl: [" << thumbnailUrl << "]"
            << endl;
        
        COTDImage image(objectId, fullUrl, thumbnailUrl, imageTitle, likes);
        this->images.push_back(image);
    }
    
    if (iterator)
    {
        if (error.size())
        {
            err << error.c_str() << endl;
        }
        error = "";
        return true;
    }
    
    return false;
}

void COTDParse::onHttpRequestCompletedImages(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::string error;
    
    if (this->callback)
    {
        succeeded = this->parseResponse(response, error);

        this->callback(succeeded, error);
    }
    
    for (COTDImage::Vector::iterator iter = this->images.begin(); iter < this->images.end(); ++iter)
    {
        inf << "getFullUrl = " << iter->getFullUrl() << endl;
    }
}


COTDParse::COTDParse()
{
    dbg << endl;
}



#define COTDIMAGE_URL "https://api.parse.com/1/classes/COTDImage"
#define COTDUSERIMAGE_URL "https://api.parse.com/1/classes/COTDUserImage"
#define PARSEAPPLICATIONID_HEADER_FIELD     "X-Parse-Application-Id"
#define PARSERESTAPIKEY_HEADER_FIELD        "X-Parse-REST-API-Key"
#define PARSEAPPLICATIONID_HEADER_VALUE     "4w57EiBsbDCULkdlP5q1Q0R5bLPDupCbokbNT4KU"
#define PARSERESTAPIKEY_HEADER_VALUE        "IbCj3m1TlWMag98nQDDkv1nXUAvMN7PW6fNsbMYP"

void COTDParse::query(const ccParseCallback& callback)
{
    dbg << endl;
    
    this->callback = callback;
    
    this->queryImages();
}

const char *COTDParse::applicationId()
{
    std::string applicationId = PARSEAPPLICATIONID_HEADER_FIELD;
    applicationId += ":";
    applicationId += PARSEAPPLICATIONID_HEADER_VALUE;
    return applicationId.c_str();
}


const char *COTDParse::apiKey()
{
    std::string apiKey = PARSERESTAPIKEY_HEADER_FIELD;
    apiKey += ":";
    apiKey += PARSERESTAPIKEY_HEADER_VALUE;
    return apiKey.c_str();
}


void COTDParse::queryImages()
{
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl(COTDIMAGE_URL);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);

    std::vector<std::string> headers;
    headers.push_back(this->applicationId());
    headers.push_back(this->apiKey());
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(COTDParse::onHttpRequestCompletedImages, this));
    request->setTag("GET COTD_IMAGES");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

