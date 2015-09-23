#ifndef __COTDGRID_SCENE_H__
#define __COTDGRID_SCENE_H__

#include "cocos2d.h"

#include "COTDImage.h"
#include <strstream>


USING_NS_CC;
using namespace std;

using namespace cocos2d;

class COTDGrid : public cocos2d::Layer
{

private:
    MenuItemLabel* createBackButton();
    void configureTitle();
    void configureImages();
    void configureMenu();
    
    void queryParse();
    void parseQueryCallback(bool succeeded,
                            strstream& error,
                            const COTDImage::Vector& vector);
    
    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);
    
    // collection view
    void showCollection();

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(COTDGrid);
    
};

#endif // __COTDGRID_SCENE_H__
