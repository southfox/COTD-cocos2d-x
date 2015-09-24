
#include "COTDGridScene.h"
#include "COTDParse.h"
#include "COTDLog.h"
#include "HFCollectionView.hpp"
#include "HFCollectionViewCell.hpp"

USING_NS_CC;


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
    
    COTDGrid::showCollection();

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


#define COLLECTIONVIEW_WIDHT 1024
#define COLLECTIONVIEW_HEIGHT 768


void COTDGrid::showCollection()
{

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size winSize = Director::getInstance()->getWinSize();
    //     static HFCollectionView* create(HFCollectionViewDataSource* dataSource, Size size, Node *container);

    HFCollectionView* collectionView = HFCollectionView::create(this, visibleSize, this);
//    collectionView->setMultipleSelectEnabled(true);
    collectionView->setDirection(ScrollView::Direction::VERTICAL);
//    this->addChild(collectionView);
    collectionView->reloadData();
}

#pragma mark -
#pragma mark collection data source

#if 0
int COTDGrid::numberOfItemsInSection(HFCollectionView* collectionView, int section)
{
    return 10;
}

//int COTDGrid::numberOfSectionsInCollectionView(HFCollectionView * collectionView)
//{
//    return 1;
//}

Size COTDGrid::collectionCellSizeForIndex(HFCollectionView *collection, ssize_t idx)
{
    return Size(100,100);
};

Size COTDGrid::cellSizeForCollection(HFCollectionView *collection) {
    return Size(100,100);
};

HFCollectionViewCell* COTDGrid::collectionViewCellAtIndex(HFCollectionView *collectionView, ssize_t idx)
{
    return nullptr;
}

//// Information about the current state of the collection view.
//ssize_t COTDGrid::numberOfCellsInCollectionView(HFCollectionView *collectionView)
//{
//    return 10;
//}


#pragma mark -
#pragma mark delegate


void COTDGrid::collectionCellTouched(HFCollectionView* collectionView, HFCollectionViewCell* cell)
{
    
}

#endif

void COTDGrid::collectionCellTouched(HFCollectionView* collectionView, HFCollectionViewCell* cell)
{
    
}

HFCollectionViewCell* COTDGrid::collectionViewCellAtIndex(HFCollectionView *collectionView, ssize_t idx)
{
    return nullptr;
}

int COTDGrid::numberOfItemsInSection(HFCollectionView* collectionView, int section)
{
    return 10;
}


