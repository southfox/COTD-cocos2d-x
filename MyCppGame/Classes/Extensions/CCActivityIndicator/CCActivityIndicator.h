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

#ifndef CCActivityIndicator_h
#define CCActivityIndicator_h

#include "cocos2d.h"

#define kActivityIndicatorFramesCount 12
#define kActivityIndicatorDelayBetweenFrames 0.1

typedef std::function<void(void)> ccActivityCallback;

class CCActivityIndicator : public cocos2d::SpriteBatchNode {
public:
    bool init();
    CREATE_FUNC(CCActivityIndicator);
    ~CCActivityIndicator();
    void startAnimating();
    void stopAnimating();
    bool isAnimating() {return animating;}
    
    virtual void setParent(cocos2d::Node* p);
    virtual void setPosition(const cocos2d::Point& pos);
    virtual void setBeginCallback(const ccActivityCallback &callback);
    virtual void setEndCallback(const ccActivityCallback &callback);
protected:
    CC_SYNTHESIZE(bool, hidesWhenStopped, HidesWhenStopped);
private:
    bool animating;
    cocos2d::Sprite* indicator;
    
    void updateVisibility();
    ccActivityCallback beginCallback;
    ccActivityCallback endCallback;

};


#endif