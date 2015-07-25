USING_NS_CC;

#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    HelloWorld::configureMenu();
    
    HelloWorld::createCloseButton();
    
    HelloWorld::configureTitle();
    
    HelloWorld::configureImage();
    
    HelloWorld::configureMenu();
    
    return true;
}


MenuItemImage* HelloWorld::createCloseButton()
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
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    return closeItem;
    
}

MenuItemLabel* HelloWorld::createLikeButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto like = Label::createWithTTF("[ like ]", "fonts/arial.ttf", 30);

    auto likeButton = MenuItemLabel::create(like, CC_CALLBACK_1(HelloWorld::menuLikeCallback, this));
    
    likeButton->setPosition(Vec2(origin.x + visibleSize.width - like->getContentSize().width,
                                 origin.y + visibleSize.height - like->getContentSize().height));
    
    return likeButton;

}

MenuItemLabel* HelloWorld::createGridButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto grid = Label::createWithTTF("[ grid ]", "fonts/arial.ttf", 30);
    
    auto button = MenuItemLabel::create(grid, CC_CALLBACK_1(HelloWorld::menuGridCallback, this));
    
    button->setPosition(Vec2(origin.x + grid->getContentSize().width,
                           origin.y + visibleSize.height - grid->getContentSize().height));
    
    return button;
}

void HelloWorld::configureTitle()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Capybara of the day", "fonts/Marker Felt.ttf", 50);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
}

void HelloWorld::configureImage()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("capybara.jpg");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
}

void HelloWorld::configureMenu()
{
    // create menu, it's an autorelease object
    auto menu = Menu::create(HelloWorld::createCloseButton(),
                             HelloWorld::createLikeButton(),
                             HelloWorld::createGridButton(), NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

void HelloWorld::menuLikeCallback(Ref* pSender)
{

}

void HelloWorld::menuGridCallback(Ref* pSender)
{
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
