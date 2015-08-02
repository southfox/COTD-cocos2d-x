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
    
    COTDUserImage();
    COTDUserImage(const std::string &image, const std::string &savedAt);
    
    const std::string& getImage() const;
    const std::string& getSavedAt() const;
    const std::string getDateSavedAt() const;
    
    typedef std::vector <COTDUserImage> Vector;

    friend std::ostream& operator << ( std::ostream& osr, const COTDUserImage& userImage);

};

#endif /* defined(__COTD__COTDUserImage__) */
