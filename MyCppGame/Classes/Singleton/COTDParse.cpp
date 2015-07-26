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
#include <HttpResponse.h>
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

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
#if 0
    
    if (!DICTOOL->checkObjectExist_json(json, P_Items))
    {
        error = "not exist ";
        error += P_Items;
        return false;
    }
    
    if (!DICTOOL->getArrayCount_json(json, P_Items))
    {
        error = "empty ";
        error += P_Items;
        return false;
    }
    
    const rapidjson::Value &itemDic = DICTOOL->getDictionaryFromArray_json(json, P_Items, 0);
    
    title = DICTOOL->getStringValue_json(itemDic, P_Title);
    if (!title.length())
    {
        error = "empty ";
        error += P_Title;
        return false;
    }
    link = DICTOOL->getStringValue_json(itemDic, P_Link);
    if (!link.length())
    {
        error = "empty ";
        error += P_Link;
        return false;
    }
    const rapidjson::Value& image = DICTOOL->getSubDictionary_json(itemDic, P_Image);
    thumbnailLink = DICTOOL->getStringValue_json(image, P_ThumbnailLink);
    if (!thumbnailLink.length())
    {
        error = "empty ";
        error += P_ThumbnailLink;
        return false;
    }
    
    dbg << "title: [" << title << "] - "
    << "link: [" << link << "] - "
    << "thumbnailLink: [" << thumbnailLink << "]" << endl;
#endif
    return true;
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

