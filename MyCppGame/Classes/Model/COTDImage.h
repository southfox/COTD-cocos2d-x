//
//  COTDImage.h
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#ifndef __COTD__COTDImage__
#define __COTD__COTDImage__

#include <stdio.h>
#include <string>

class COTDImage {
private:
    std::string objectId;
    std::string fullUrl;
    std::string thumbnailUrl;
    std::string imageTitle;
    int likes;
    
public:
    
    COTDImage(const char *objectId,
              const char *link,
              const char *thumbnail,
              const char *title,
              const int likes);
    
    const std::string& getObjectId() const;
    const std::string& getFullUrl() const;
    const std::string& getThumbnailUrl() const;
    const std::string& getImageTitle() const;
    const int getLikes() const;
    
};
#endif /* defined(__COTD__COTDImage__) */
