//
//  COTDImage.cpp
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#include "COTDImage.h"

const std::string& COTDImage::getObjectId() const
{
    return this->objectId;
}

void COTDImage::setObjectId(const std::string& objectId)
{
    this->objectId = objectId;
}

const std::string& COTDImage::getFullUrl() const
{
    return this->fullUrl;
}

void COTDImage::setFullUrl(const std::string &fullUrl)
{
    this->fullUrl = fullUrl;
}

const std::string& COTDImage::getThumbnailUrl() const
{
    return this->thumbnailUrl;
}

void COTDImage::setThumbnailUrl(const std::string &thumbnailUrl)
{
    this->thumbnailUrl = thumbnailUrl;
}

const std::string& COTDImage::getImageTitle() const
{
    return this->imageTitle;
}

void COTDImage::setImageTitle(const std::string &imageTitle)
{
    this->imageTitle = imageTitle;
}

const int COTDImage::getLikes() const
{
    return this->likes;
}

void COTDImage::setLikes(const int likes)
{
    this->likes = likes;
}
