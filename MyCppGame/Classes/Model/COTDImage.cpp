//
//  COTDImage.cpp
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#include "COTDImage.h"
#import <strstream>

COTDImage::COTDImage()
{
}

COTDImage::COTDImage(const std::string &link,
                     const std::string &thumbnail,
                     const std::string &title,
                     const int likes,
                     const std::string &objectId) :
fullUrl(link),
thumbnailUrl(thumbnail),
imageTitle(title),
likes(likes),
objectId(objectId)
{
}

const std::string& COTDImage::getObjectId() const
{
    return this->objectId;
}

void COTDImage::setObjectId(const std::string &objectId)
{
    this->objectId = objectId;
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

void COTDImage::incrementLikes()
{
    this->likes++;
}


bool COTDImage::operator < (const COTDImage &other) const
{
    return this->likes > other.likes;
}

std::ostream& operator << ( std::ostream& osr, const COTDImage& image)
{
#if defined(ANDROID)
    std::strstream buf;
    buf << "objectId: [" << image.getObjectId() << "], "
        << "fullUrl: [" << image.getFullUrl() << "], "
        << "thumbnailUrl: [" << image.getThumbnailUrl() << "], "
        << "imageTitle: [" << image.getImageTitle() << "], "
        << "likes: [" << image.getLikes();
    osr << buf.str();
#else
    osr << "objectId: [" << image.getObjectId() << "], "
        << "fullUrl: [" << image.getFullUrl() << "], "
        << "thumbnailUrl: [" << image.getThumbnailUrl() << "], "
        << "imageTitle: [" << image.getImageTitle() << "], "
        << "likes: [" << image.getLikes();
#endif
    return osr;
}
