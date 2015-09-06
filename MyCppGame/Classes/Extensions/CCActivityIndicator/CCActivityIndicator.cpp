/****************************************************************************
 Copyright (c) 2013 Artavazd Barseghyan
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

/****************************************************************************
 Touched by Javier Fuchs to addapt to cocos2d-x 3.6
 ****************************************************************************/

#include "CCActivityIndicator.h"

USING_NS_CC;

bool CCActivityIndicator::init()
{
    SpriteFrameCache * spriteFramecache = SpriteFrameCache::getInstance();;
    
    spriteFramecache->addSpriteFramesWithFile("ccactivityindicator.plist");
    CCSpriteBatchNode::initWithFile("ccactivityindicator.png", 1);
    
    Size winSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    indicator = CCSprite::createWithSpriteFrameName("ccactivityindicator_1.gif");
    indicator->setPosition(Vec2(winSize.width/2,winSize.height/2));
    addChild(indicator);
    
    animating = false;
    hidesWhenStopped = true;
    
//    spriteFrames = new Vector<SpriteFrame*>;

    //load all sprite frames into array
    for (int i=1; i<=kActivityIndicatorFramesCount; i++) {
        SpriteFrame * frame = spriteFramecache->getSpriteFrameByName(CCString::createWithFormat("ccactivityindicator_%d.gif",i)->getCString());
        frame->retain();
        spriteFrames.pushBack(frame);
    }
    
    return true;
}

CCActivityIndicator::~CCActivityIndicator()
{
    for (auto frame : spriteFrames)
    {
        frame->release();
    }
}

void CCActivityIndicator::updateVisibility()
{
    if (hidesWhenStopped && !isAnimating()) {
        indicator->setVisible(false);
    }
}

void CCActivityIndicator::setParent(Node* p)
{
    CCSpriteBatchNode::setParent(p);
    
    if (p!=NULL) {
        updateVisibility();
    }
}

void CCActivityIndicator::setPosition(const cocos2d::Point& pos)
{
    indicator->setPosition(pos);
}

void CCActivityIndicator::startAnimating()
{
    if (animating) return;
    animating = true;
    
    indicator->setVisible(true);
    Animation * anim = Animation::createWithSpriteFrames(spriteFrames, kActivityIndicatorDelayBetweenFrames);
    Animate * action = Animate::create(anim);
    indicator->runAction(CCRepeatForever::create(action));
}

void CCActivityIndicator::stopAnimating()
{
    if (!animating) return;
    animating = false;
    
    indicator->stopAllActions();
    updateVisibility();
}