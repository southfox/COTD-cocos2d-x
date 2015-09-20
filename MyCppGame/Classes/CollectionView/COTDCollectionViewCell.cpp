//
//  COTDCollectionViewCell.cpp
//  MyCppGame
//
//  Created by Javier Fuchs on 9/18/15.
//
//

#include "COTDCollectionViewCell.h"

COTDCollectionViewCell::COTDCollectionViewCell()
{
    _imageView = Sprite::create();
    this->addChild(_imageView);
    _imageView->retain();
}


COTDCollectionViewCell::~COTDCollectionViewCell()
{
    CC_SAFE_RELEASE(_imageView);
}

COTDCollectionViewCell* COTDCollectionViewCell::create(const char *fileName)
{
    COTDCollectionViewCell* cell = new COTDCollectionViewCell();
    if(cell && cell->init(fileName))
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return nullptr;
}

bool COTDCollectionViewCell::init(const char *fileName)
{
    this->setImageViewWithFile(fileName);
    return true;
}

void COTDCollectionViewCell::reset()
{
    _idx = -1;
}

ssize_t COTDCollectionViewCell::getIdx() const
{
    return _idx;
}

void COTDCollectionViewCell::setIdx(ssize_t idx)
{
    _idx = idx;
}

void COTDCollectionViewCell::setImageViewWithFile(const char *fileName)
{
    Texture2D *texture2D = Director::getInstance()->getTextureCache()->addImage(fileName);
    _imageView->setTexture(texture2D);
    Size size = texture2D->getContentSize();
    _imageView->setTextureRect(Rect(0, 0, size.width, size.height));
    _imageView->setPosition(Point(size.width*0.5f, size.height*0.5f));
    _imageView->setScale((size.width - 10) / 100);
    // collectionCellSizeForIndex
    
    /*
     Size visibleSize = Director::getInstance()->getVisibleSize();
     Vec2 origin = Director::getInstance()->getVisibleOrigin();
     
     // add "COTDMain" splash screen"
     auto sprite = Sprite::create(imageName);
     
     // position the sprite on the center of the screen
     sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
     sprite->setScale((visibleSize.width - 50)/sprite->getContentSize().width);
*/
}

Sprite* COTDCollectionViewCell::getImageView()
{
    return _imageView;
}

