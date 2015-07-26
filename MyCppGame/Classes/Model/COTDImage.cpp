//
//  COTDImage.cpp
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#include "COTDImage.h"

COTDImage::COTDImage(const char *objectId,
                     const char *link,
                     const char *thumbnail,
                     const char *title,
                     const int likes) :
objectId(objectId),
fullUrl(link),
thumbnailUrl(thumbnail),
imageTitle(title),
likes(likes)
{
}

const std::string& COTDImage::getObjectId() const
{
    return this->objectId;
}

const std::string& COTDImage::getFullUrl() const
{
    return this->fullUrl;
}

const std::string& COTDImage::getThumbnailUrl() const
{
    return this->thumbnailUrl;
}

const std::string& COTDImage::getImageTitle() const
{
    return this->imageTitle;
}

const int COTDImage::getLikes() const
{
    return this->likes;
}

