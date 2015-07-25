//
//  COTDUserImage.h
//  COTD
//
//  Created by Javier Fuchs on 7/21/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#ifndef __COTD__COTDUserImage__
#define __COTD__COTDUserImage__

#include <stdio.h>
#include <string>

#include "COTDImage.h"

class COTDUserImage {
    
protected:
    std::string image;
    std::string savedAt;
    
public:

    const std::string& getImage() const;
    void setImage(const std::string& image);
    
    const std::string& getSavedAt() const;
    void setSavedAt(const std::string& savedAt);
};

#endif /* defined(__COTD__COTDUserImage__) */
