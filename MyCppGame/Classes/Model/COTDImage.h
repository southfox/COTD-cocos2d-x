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
#include <vector>

class COTDImage {
private:
    std::string objectId;
    std::string fullUrl;
    std::string thumbnailUrl;
    std::string imageTitle;
    int likes;
    
public:
    
    COTDImage();
    COTDImage(const std::string &link,
              const std::string &thumbnail,
              const std::string &title,
              const int likes = 0,
              const std::string &objectId = "");
    
    const std::string& getObjectId() const;
    void setObjectId(const std::string &objectId);
    const std::string& getFullUrl() const;
    const std::string& getThumbnailUrl() const;
    const std::string& getImageTitle() const;
    const int getLikes() const;
    void incrementLikes();
    
    typedef std::vector <COTDImage> Vector;
    
    bool operator < (const COTDImage &other) const;
    
    friend std::ostream& operator << ( std::ostream& osr, const COTDImage& image);
};
#endif /* defined(__COTD__COTDImage__) */
