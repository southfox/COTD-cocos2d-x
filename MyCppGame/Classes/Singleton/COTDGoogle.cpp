//
//  COTDGoogle.cpp
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#include "COTDGoogle.h"


COTDGoogle* COTDGoogle::_instance = 0;

COTDGoogle* COTDGoogle::sharedInstance()
{
    if (_instance == 0) {
        _instance = new COTDGoogle;
    }
    return _instance;
}


COTDGoogle::COTDGoogle()
{
}


//#define URLFORMAT @"https://www.googleapis.com/customsearch/v1?key=AIzaSyADOPSjmHQYFFf9ZnWTqVQ3kPRwr5ND6l8&cx=003054679763599795063:tka3twkxrbw&searchType=image&&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"
//#define URLFORMAT @"https://www.googleapis.com/customsearch/v1?key=AIzaSyDipywri5f6D__qqcCgvbBzP9uF5xbP9b0&cx=003054679763599795063:tka3twkxrbw&searchType=image&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"
//#define URLFORMAT @"https://www.googleapis.com/customsearch/v1?key=AIzaSyCv-rIYBwljnGmgJAxMIh8wBaXpqT6U-T0&cx=003054679763599795063:tka3twkxrbw&searchType=image&&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"
#define URLFORMAT @"https://www.googleapis.com/customsearch/v1?key=AIzaSyDhZSxw5rAjmGLHGBJH5ouHDWnMg42LW4g&cx=003054679763599795063:tka3twkxrbw&searchType=image&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"
//#define URLFORMAT @"https://www.googleapis.com/customsearch/v1?key=AIzaSyDipywri5f6D__qqcCgvbBzP9uF5xbP9b0&cx=003054679763599795063:tka3twkxrbw&searchType=image&fields=items(link,title,image/thumbnailLink)&start=%d&num=1&q=capybara"


void COTDGoogle::queryTerm(const std::string& term, const int start, bool& succeeded, std::string &link, std::string &thumbnailLink, std::string &title, std::string error)
{
}

