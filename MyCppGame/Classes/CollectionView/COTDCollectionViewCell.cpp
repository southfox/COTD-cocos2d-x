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
}

Sprite* COTDCollectionViewCell::getImageView()
{
    return _imageView;
}

