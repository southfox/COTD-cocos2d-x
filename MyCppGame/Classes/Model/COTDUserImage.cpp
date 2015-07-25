//
//  COTDUserImage.m
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#import "COTDUserImage.h"


const std::string &COTDUserImage::getImage() const
{
    return this->image;
}

void COTDUserImage::setImage(const std::string &image)
{
    this->image = image;
}

const std::string& COTDUserImage::getSavedAt() const
{
    return this->savedAt;
}

void COTDUserImage::setSavedAt(const std::string& savedAt)
{
    this->savedAt = savedAt;
}




