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
    
    const std::string& getObjectId() const;
    void setObjectId(const std::string& objectId);

    const std::string& getFullUrl() const;
    void setFullUrl(const std::string& fullUrl);
    
    const std::string& getThumbnailUrl() const;
    void setThumbnailUrl(const std::string& thumbnailUrl);
    
    const std::string& getImageTitle() const;
    void setImageTitle(const std::string& imageTitle);

    const int getLikes() const;
    void setLikes(const int likes);
    
};
#endif /* defined(__COTD__COTDImage__) */
