USING_NS_CC;

#include "COTDMainScene.h"
#include "COTDGridScene.h"
#include "COTDDate.h"
#include "COTDLog.h"
#include "COTDGoogle.h"


Scene* COTDMain::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = COTDMain::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool COTDMain::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    dbg << endl;
    COTDMain::searchGoogle();

    COTDMain::configureMenu();
    
    COTDMain::createCloseButton();
    
    COTDMain::configureTitle();
    
    COTDMain::configureImage("capybara.jpg");
    
    return true;
}


MenuItemImage* COTDMain::createCloseButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(COTDMain::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    return closeItem;
    
}

MenuItemLabel* COTDMain::createLikeButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto like = Label::createWithTTF("[ like ]", "fonts/arial.ttf", 30);

    auto likeButton = MenuItemLabel::create(like, CC_CALLBACK_1(COTDMain::menuLikeCallback, this));
    
    likeButton->setPosition(Vec2(origin.x + visibleSize.width - like->getContentSize().width,
                                 origin.y + visibleSize.height - like->getContentSize().height));
    
    return likeButton;

}

MenuItemLabel* COTDMain::createGridButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto grid = Label::createWithTTF("[ grid ]", "fonts/arial.ttf", 30);
    
    auto button = MenuItemLabel::create(grid, CC_CALLBACK_1(COTDMain::menuGridCallback, this));
    
    button->setPosition(Vec2(origin.x + grid->getContentSize().width,
                           origin.y + visibleSize.height - grid->getContentSize().height));
    
    return button;
}

void COTDMain::configureTitle()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    std::string title = "Capybara of the day: ";
    COTDDate *date = new COTDDate();
    
    title += date->D2Str();
    
    auto label = Label::createWithTTF(title, "fonts/Marker Felt.ttf", 50);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
}

void COTDMain::configureImage(const char *imageName)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add "COTDMain" splash screen"
    auto sprite = Sprite::create();
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
}

void COTDMain::configureMenu()
{
    // create menu, it's an autorelease object
    auto menu = Menu::create(COTDMain::createCloseButton(),
                             COTDMain::createLikeButton(),
                             COTDMain::createGridButton(), NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}


void COTDMain::searchGoogle()
{
    std::string term;
//    COTDGoogle::sharedInstance()->queryTerm(term, 1, CC_CALLBACK_1(COTDMain::googleSearchCallback, this));
    COTDGoogle::sharedInstance()->queryTerm(term, 1, std::bind(&COTDMain::googleSearchCallback,
                                                               this,
                                                               std::placeholders::_1,
                                                               std::placeholders::_2,
                                                               std::placeholders::_3,
                                                               std::placeholders::_4,
                                                               std::placeholders::_5));
    /*
     std::string& link,
     std::string& thumbnailLink,
     std::string& title,
     std::string& error);

     typedef std::function<void(bool, const std::string &, const std::string&, const std::string&, const std::string&)> ccGoogleCallback;
     this->callback(succeeded, link, thumbnailLink, title, error);

     _downloader->_onProgress = std::bind(&AssetsManagerEx::onProgress,
     this,
     std::placeholders::_1,
     std::placeholders::_2,
     std::placeholders::_3,
     std::placeholders::_4);
*/
    //std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
}

void COTDMain::menuLikeCallback(Ref* pSender)
{
}

void COTDMain::menuGridCallback(Ref* pSender)
{
    auto scene = COTDGrid::createScene();
    auto director = Director::getInstance();
    director->pushScene(scene);
}

void COTDMain::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void COTDMain::googleSearchCallback(bool succeeded,
                                    const std::string& link,
                                    const std::string& thumbnailLink,
                                    const std::string& title,
                                    const std::string& error)
{
    dbg << endl;
    
}

