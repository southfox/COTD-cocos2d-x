#ifndef __COTDMAIN_SCENE_H__
#define __COTDMAIN_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class COTDMain : public cocos2d::Layer
{

private:
    MenuItemImage* createCloseButton();
    MenuItemLabel* createLikeButton();
    MenuItemLabel* createGridButton();
    void configureTitle();
    void configureImage(const char *imageName);
    void configureMenu();
    void searchGoogle();
    
    // a selector callback
    void menuLikeCallback(cocos2d::Ref* pSender);
    void menuGridCallback(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    void googleSearchCallback(bool succeeded,
                              const std::string& link,
                              const std::string& thumbnailLink,
                              const std::string& title,
                              const std::string& error);

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(COTDMain);
    
};

#endif // __COTDMAIN_SCENE_H__
