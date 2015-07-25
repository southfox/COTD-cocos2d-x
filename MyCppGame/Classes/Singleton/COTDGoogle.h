//
//  COTDGoogle.h
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#ifndef __COTDGOOGLE_H__
#define __COTDGOOGLE_H__

class COTDGoogle
{
public:
    static COTDGoogle* sharedInstance();

    void queryTerm(const std::string term, const int start, bool& succeeded, std::string &link, std::string &thumbnailLink, std::string &title, std::string error);

protected: 
    COTDGoogle();

private:
    static COTDGoogle* _instance;
};

#endif // __COTDGOOGLE_H__
