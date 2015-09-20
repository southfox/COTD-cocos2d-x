//
//  COTDCollectionViewCell.h
//  MyCppGame
//
//  Created by Javier Fuchs on 9/18/15.
//
//
// Based on https://github.com/LanouTechnology/CCCollectionView/blob/master/CCCollectionView/CCCollectionViewCell.h


#ifndef __MyCppGame__COTDCollectionViewCell__
#define __MyCppGame__COTDCollectionViewCell__

#include "extensions/ExtensionMacros.h"
#include "cocos2d.h"

USING_NS_CC;

class COTDCollectionViewCell : public Node
{
public:
    COTDCollectionViewCell();
    ~COTDCollectionViewCell();
    
    static COTDCollectionViewCell* create(const char* fileName);
    bool init(const char* fileName);
    
    ssize_t getIdx() const;
    void setIdx(ssize_t uIdx);
    void reset();
    
    Sprite* getImageView();
private:
    ssize_t _idx;
    Sprite* _imageView;
    
    void setImageViewWithFile(const char* fileName);
};

#endif /* defined(__MyCppGame__COTDCollectionViewCell__) */
