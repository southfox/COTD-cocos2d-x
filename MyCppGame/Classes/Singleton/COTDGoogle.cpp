//
//  COTDGoogle.cpp
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

#include "COTDGoogle.h"

COTDGoogle* COTDGoogle::_instance = 0;

COTDGoogle* COTDGoogle::sharedInstance()
{
    if (_instance == 0) {
        _instance = new COTDGoogle;
    }
    return _instance;
}

//#define URLFORMAT "https://www.googleapis.com/customsearch/v1?key=AIzaSyADOPSjmHQYFFf9ZnWTqVQ3kPRwr5ND6l8&cx=003054679763599795063:tka3twkxrbw&searchType=image&&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"
//#define URLFORMAT "https://www.googleapis.com/customsearch/v1?key=AIzaSyDipywri5f6D__qqcCgvbBzP9uF5xbP9b0&cx=003054679763599795063:tka3twkxrbw&searchType=image&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"
//#define URLFORMAT "https://www.googleapis.com/customsearch/v1?key=AIzaSyCv-rIYBwljnGmgJAxMIh8wBaXpqT6U-T0&cx=003054679763599795063:tka3twkxrbw&searchType=image&&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"
#define URLFORMAT "https://www.googleapis.com/customsearch/v1?key=AIzaSyDhZSxw5rAjmGLHGBJH5ouHDWnMg42LW4g&cx=003054679763599795063:tka3twkxrbw&searchType=image&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"
//#define URLFORMAT "https://www.googleapis.com/customsearch/v1?key=AIzaSyDipywri5f6D__qqcCgvbBzP9uF5xbP9b0&cx=003054679763599795063:tka3twkxrbw&searchType=image&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"


void COTDGoogle::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    this->succeeded = false;

    if (!response)
    {
        this->error = "no response";
        return;
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
        this->error = response->getErrorBuffer();
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string res;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        res += (*buffer)[i];
    }
    dbg << "COTDGoogle::onHttpRequestCompleted() dump data:" << "{\n" << res.c_str() << "}\n" << endl;
    
    rapidjson::Document json;
    rapidjson::StringStream stream(res.c_str());
    
    json.ParseStream<0>(stream);
    if (json.HasParseError()) {
        err << "GetParseError " << json.GetParseError() << endl;
        this->error = json.GetParseError();
        return;
    }

    if (!DICTOOL->checkObjectExist_json(json, P_Items))
    {
        this->error = "not exist ";
        this->error += P_Items;
        return;
    }
    
    if (!DICTOOL->getArrayCount_json(json, P_Items))
    {
        this->error = "empty ";
        this->error += P_Items;
        return;
    }
    
    const rapidjson::Value &itemDic = DICTOOL->getDictionaryFromArray_json(json, P_Items, 0);
    
    this->title = DICTOOL->getStringValue_json(itemDic, P_Title);
    if (!this->title.length())
    {
        this->error = "empty ";
        this->error += P_Title;
        return;
    }
    this->link = DICTOOL->getStringValue_json(itemDic, P_Link);
    if (!this->link.length())
    {
        this->error = "empty ";
        this->error += P_Link;
        return;
    }
    const rapidjson::Value& image = DICTOOL->getSubDictionary_json(itemDic, P_Image);
    this->thumbnailLink = DICTOOL->getStringValue_json(image, P_ThumbnailLink);
    if (!this->thumbnailLink.length())
    {
        this->error = "empty ";
        this->error += P_ThumbnailLink;
        return;
    }
    
    dbg << "title: [" << this->title << "] - "
        << "link: [" << this->link << "] - "
        << "thumbnailLink: [" << this->thumbnailLink << "]" << endl;

    this->succeeded = true;
}


COTDGoogle::COTDGoogle()
{
    dbg << endl;
}




void COTDGoogle::queryTerm(const std::string& term, const int& start)
{
    dbg << endl;
    char aux[1000];
    sprintf(aux, URLFORMAT, 1);
    
    cocos2d::network::HttpRequest* request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl(aux);
    
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(COTDGoogle::onHttpRequestCompleted, this));
    request->setTag("GET COTD");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

