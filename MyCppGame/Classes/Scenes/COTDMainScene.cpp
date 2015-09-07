
#include "COTDMainScene.h"
#include "COTDGridScene.h"
#include "COTDDate.h"
#include "COTDLog.h"
#include "COTDGoogle.h"
#include "COTDParse.h"
#include "COTDImage.h"
#include <assets-manager/Downloader.h>

#include "COTDCommon.h"
#include "CCActivityIndicator.h"

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
    COTDMain::createSpinner();

    COTDMain::queryParse();

    COTDMain::configureMenu();
    
    COTDMain::createCloseButton();
    
    COTDMain::configureTitle();
    
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

    std::string title = "Capybara of the day: ";
    title += COTDParse::sharedInstance()->today();
    
    auto label = Label::createWithTTF(title, "fonts/Marker Felt.ttf", 40);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
}

void COTDMain::configureImage(const std::string &imageName)
{
    this->activityIndicator->stopAnimating();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add "COTDMain" splash screen"
    auto sprite = Sprite::create(imageName);
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setScale((visibleSize.width - 50)/sprite->getContentSize().width);
    
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
    COTDGoogle::sharedInstance()->queryTerm(term, COTDParse::sharedInstance()->currentStart(), std::bind(&COTDMain::googleSearchCallback,
                                                               this,
                                                               std::placeholders::_1,
                                                               std::placeholders::_2,
                                                               std::placeholders::_3,
                                                               std::placeholders::_4,
                                                               std::placeholders::_5));
}

void COTDMain::createSpinner()
{
    this->activityIndicator = new CCActivityIndicator();
    this->activityIndicator->init();
//    this->activityIndicator->setParent(this);
    this->activityIndicator->startAnimating();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->activityIndicator->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(this->activityIndicator);
}

void COTDMain::queryParse()
{
    std::string term;
    COTDParse::sharedInstance()->query(std::bind(&COTDMain::onUpdateImage,
                                                 this,
                                                 std::placeholders::_1,
                                                 std::placeholders::_2));
}

void COTDMain::menuLikeCallback(Ref* pSender)
{
    COTDParse::sharedInstance()->likeCurrentImage(std::bind(&COTDMain::onLikeCurrentImage,
                                                            this,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2));
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

    COTDParse::destroyInstance();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void COTDMain::onError(const cocos2d::extension::Downloader::Error &error)
{
    std::strstream s;
    auto currentUserImage = COTDParse::sharedInstance()->currentUserImage();
    s << "error -> code:" << (int)error.code << ", message: " << error.message << ", url:" << currentUserImage->getFullUrl() << '\0';
    dbg << s.str() << endl;
    MessageBox(s.str(), "Error");
}

void COTDMain::onProgress(double total, double downloaded, const std::string &url, const std::string &customId)
{
    dbg << "onProgress -> total " << total << ", downloaded: " << downloaded << ", url: " << url << ", customId: " << customId << endl;
}

void COTDMain::onSuccess(const std::string &srcUrl, const std::string &storagePath, const std::string &customId)
{
    dbg << "onSuccess -> srcUrl " << srcUrl << ", storagePath: " << storagePath << ", customId: " << customId << endl;

    COTDMain::configureImage(storagePath);

}

void COTDMain::download(const COTDImage *currentUserImage)
{
#define DEFAULT_CONNECTION_TIMEOUT 8
    std::size_t found = currentUserImage->getFullUrl().find_last_of("/");
    FileUtils* fu = FileUtils::getInstance();
    std::string storagePath = fu->getWritablePath();
    storagePath += currentUserImage->getObjectId();
    storagePath += "_";
    storagePath += currentUserImage->getFullUrl().substr(found+1);
    
    if (fu->isFileExist(storagePath))
    {
        COTDMain::configureImage(storagePath);
        return;
    }
    
    // File does not exist, download.
    
    this->downloader = std::make_shared<cocos2d::extension::Downloader>();

    this->downloader->setConnectionTimeout(DEFAULT_CONNECTION_TIMEOUT);
    
    this->downloader->setErrorCallback(std::bind(&COTDMain::onError,
                                                 this,
                                                 std::placeholders::_1));
    
    this->downloader->setProgressCallback(std::bind(&COTDMain::onProgress,
                                                    this,
                                                    std::placeholders::_1,
                                                    std::placeholders::_2,
                                                    std::placeholders::_3,
                                                    std::placeholders::_4));
    
    this->downloader->setSuccessCallback(std::bind(&COTDMain::onSuccess,
                                                   this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2,
                                                   std::placeholders::_3));
    
    this->downloader->downloadAsync(currentUserImage->getFullUrl(), storagePath);
}

void COTDMain::googleSearchCallback(bool succeeded,
                                    const std::string& link,
                                    const std::string& thumbnailLink,
                                    const std::string& title,
                                    std::strstream& error)
{
    dbg << endl;
    
    if (!succeeded)
    {
        MessageBox((char*)error.str(), "Error");

        COTDMain::searchGoogle();
        return;
    }
    
    if (COTDParse::sharedInstance()->isLinkRepeated(link))
    {
        MessageBox("Result repeated. Random failed", "Warning");
        COTDMain::searchGoogle();
    }
    else
    {
        std::string searchTerm;
        COTDParse::sharedInstance()->updateImage(link, thumbnailLink, title, searchTerm,
                                                 std::bind(&COTDMain::onUpdateImage,
                                                           this,
                                                           std::placeholders::_1,
                                                           std::placeholders::_2));
    }
}


void COTDMain::onLikeCurrentImage(bool succeeded, std::strstream& error)
{
    
}

void COTDMain::onUpdateImage(bool succeeded, std::strstream& error)
{
    dbg << endl;
    if (error.pcount())
    {
        MessageBox((char*)error.str(), "Error");
        return;
    }
    auto currentUserImage = COTDParse::sharedInstance()->currentUserImage();
    if (currentUserImage == nullptr)
    {
        COTDMain::searchGoogle();
    }
    else
    {
        COTDMain::download(currentUserImage);
    }
}


