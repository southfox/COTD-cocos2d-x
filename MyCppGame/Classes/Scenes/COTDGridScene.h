#ifndef __COTDGRID_SCENE_H__
#define __COTDGRID_SCENE_H__

#include "cocos2d.h"

#include "COTDImage.h"
#include <strstream>

#include "COTDCollectionView.h"


//class COTDCollectionView;
//class COTDCollectionViewDelegate;
//class COTDCollectionViewDataSource;

using namespace cocos2d;

class COTDGrid : public cocos2d::Layer, public COTDCollectionViewDelegate, public COTDCollectionViewDataSource
{

private:
    MenuItemLabel* createBackButton();
    void configureTitle();
    void configureImages();
    void configureMenu();
    
    void queryParse();
    void parseQueryCallback(bool succeeded,
                            std::strstream& error,
                            const COTDImage::Vector& vector);
    
    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);
    
    // collection view
    void showCollection();

    virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {}
    virtual void collectionCellTouched(COTDCollectionView* table, COTDCollectionViewCell* cell);
    virtual cocos2d::Size collectionCellSizeForIndex(COTDCollectionView *collectionView, ssize_t idx);
    virtual COTDCollectionViewCell* collectionCellAtIndex(COTDCollectionView *collectionView, ssize_t idx);
    virtual ssize_t numberOfCellsInCollection(COTDCollectionView *collectionView);
    virtual float leftSideSpaceForCollection(COTDCollectionView* collectionView);
    virtual float upSideSpaceForCollection(COTDCollectionView* collectionView);

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(COTDGrid);
    
};

#endif // __COTDGRID_SCENE_H__
