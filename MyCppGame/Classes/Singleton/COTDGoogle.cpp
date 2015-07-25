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

bool COTDGoogle::parseResponse(cocos2d::network::HttpResponse *response,
                               std::string& link,
                               std::string& thumbnailLink,
                               std::string& title,
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
    dbg << "COTDGoogle::onHttpRequestCompleted() dump data:" << "{\n" << res.c_str() << "}\n" << endl;
    
    rapidjson::Document json;
    rapidjson::StringStream stream(res.c_str());
    
    json.ParseStream<0>(stream);
    if (json.HasParseError()) {
        err << "GetParseError " << json.GetParseError() << endl;
        error = json.GetParseError();
        return false;
    }
    
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
    
    return true;
}

void COTDGoogle::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool succeeded = false;
    std::string link;
    std::string thumbnailLink;
    std::string title;
    std::string error;
    
    if (this->callback)
    {
        succeeded = this->parseResponse(response, link, thumbnailLink, title, error);

        this->callback(succeeded, link, thumbnailLink, title, error);
    }

}


COTDGoogle::COTDGoogle()
{
    dbg << endl;
}




void COTDGoogle::queryTerm(const std::string& term, const int& start, const ccGoogleCallback& callback)
{
    dbg << endl;
    
    this->callback = callback;
    
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

