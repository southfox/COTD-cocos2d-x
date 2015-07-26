//
//  COTDUserImage.m
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#import "COTDUserImage.h"

COTDUserImage::COTDUserImage(const char *image, const char *savedAt)
: image(image), savedAt(savedAt)
{
    
}

const std::string &COTDUserImage::getImage() const
{
    return this->image;
}

const std::string& COTDUserImage::getSavedAt() const
{
    return this->savedAt;
}



