
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
    COTDMain::configureMenu();

    COTDMain::createSpinner();

    COTDMain::queryParse();
    
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

    _likeButton = MenuItemLabel::create(like, CC_CALLBACK_1(COTDMain::menuLikeCallback, this));
    
    _likeButton->setPosition(Vec2(origin.x + visibleSize.width - like->getContentSize().width,
                                 origin.y + visibleSize.height - like->getContentSize().height));
    
    return _likeButton;

}

MenuItemLabel* COTDMain::createGridButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto grid = Label::createWithTTF("[ grid ]", "fonts/arial.ttf", 30);
    
    _gridButton = MenuItemLabel::create(grid, CC_CALLBACK_1(COTDMain::menuGridCallback, this));
    
    _gridButton->setPosition(Vec2(origin.x + grid->getContentSize().width,
                           origin.y + visibleSize.height - grid->getContentSize().height));
    
    return _gridButton;
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
    _activityIndicator->stopAnimating();

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
    _activityIndicator = new CCActivityIndicator();
    _activityIndicator->init();
    _activityIndicator->setBeginCallback(CC_CALLBACK_0(COTDMain::onSpinnerStart, this));
    _activityIndicator->setEndCallback(CC_CALLBACK_0(COTDMain::onSpinnerEnd, this));

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _activityIndicator->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(_activityIndicator, 1);
}

void COTDMain::onSpinnerStart()
{
    _likeButton->setEnabled(false);
    _gridButton->setEnabled(false);
}

void COTDMain::onSpinnerEnd()
{
    _likeButton->setEnabled(true);
    _gridButton->setEnabled(true);
}

void COTDMain::queryParse()
{
    _activityIndicator->startAnimating();

    std::string term;
    COTDParse::sharedInstance()->query(std::bind(&COTDMain::onUpdateImage,
                                                 this,
                                                 std::placeholders::_1,
                                                 std::placeholders::_2));
}

void COTDMain::menuLikeCallback(Ref* pSender)
{
    _activityIndicator->startAnimating();

    COTDParse::sharedInstance()->likeCurrentImage(std::bind(&COTDMain::onLikeCurrentImage,
                                                            this,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2));
}


void COTDMain::menuGridCallback(Ref* pSender)
{
    _activityIndicator->startAnimating();

    auto scene = COTDGrid::createScene();
    auto director = Director::getInstance();
    director->pushScene(scene);
}

void COTDMain::onExit()
{
    _activityIndicator->stopAnimating();
}

void COTDMain::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

    COTDParse::destroyInstance();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void COTDMain::onDownloadError(const Downloader::Error &error)
{
    std::strstream s;
    auto currentUserImage = COTDParse::sharedInstance()->currentUserImage();
    s << "error -> code:" << (int)error.code << ", message: " << error.message << ", url:" << currentUserImage->getFullUrl() << '\0';
    dbg << s.str() << endl;
    MessageBox(s.str(), "Error");
}

void COTDMain::onDownloadProgress(double total, double downloaded, const std::string &url, const std::string &customId)
{
    dbg << "onProgress -> total " << total << ", downloaded: " << downloaded << ", url: " << url << ", customId: " << customId << endl;
}

void COTDMain::onDownloadSuccess(const std::string &srcUrl, const std::string &storagePath, const std::string &customId)
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
    
    _downloader = std::make_shared<Downloader>();

    _downloader->setConnectionTimeout(DEFAULT_CONNECTION_TIMEOUT);
    
    _downloader->setErrorCallback(std::bind(&COTDMain::onDownloadError,
                                                 this,
                                                 std::placeholders::_1));
    
    _downloader->setProgressCallback(std::bind(&COTDMain::onDownloadProgress,
                                                    this,
                                                    std::placeholders::_1,
                                                    std::placeholders::_2,
                                                    std::placeholders::_3,
                                                    std::placeholders::_4));
    
    _downloader->setSuccessCallback(std::bind(&COTDMain::onDownloadSuccess,
                                                   this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2,
                                                   std::placeholders::_3));
    
    _downloader->downloadAsync(currentUserImage->getFullUrl(), storagePath);
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
    _activityIndicator->stopAnimating();
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


