
#include "COTDGridScene.h"
#include "COTDParse.h"
#include "COTDLog.h"

Scene* COTDGrid::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = COTDGrid::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool COTDGrid::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    COTDGrid::queryParse();
    
    COTDGrid::configureMenu();
    
    COTDGrid::configureTitle();
    
    COTDGrid::configureImages();
    
    COTDGrid::configureMenu();
    
    return true;
}


MenuItemLabel* COTDGrid::createBackButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto back = Label::createWithTTF("[ back ]", "fonts/arial.ttf", 30);
    
    auto button = MenuItemLabel::create(back, CC_CALLBACK_1(COTDGrid::menuBackCallback, this));
    
    button->setPosition(Vec2(origin.x + back->getContentSize().width,
                           origin.y + visibleSize.height - back->getContentSize().height));
    
    return button;
}

void COTDGrid::configureTitle()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto label = Label::createWithTTF("Capybara: 10 Most Liked Images", "fonts/Marker Felt.ttf", 40);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
}

void COTDGrid::queryParse()
{
    std::string term;
    COTDParse::sharedInstance()->queryTopTenImages(std::bind(&COTDGrid::parseQueryCallback,
                                                             this,
                                                             std::placeholders::_1,
                                                             std::placeholders::_2,
                                                             std::placeholders::_3));
}

void COTDGrid::parseQueryCallback(bool succeeded,
                                  std::strstream& error,
                                  const COTDImage::Vector& vector)
{
    dbg << endl;
    
}

void COTDGrid::configureImages()
{
}

void COTDGrid::configureMenu()
{
    // create menu, it's an autorelease object
    auto menu = Menu::create(COTDGrid::createBackButton(), NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}


void COTDGrid::menuBackCallback(Ref* pSender)
{
    auto director = Director::getInstance();
    director->popToRootScene();
}

