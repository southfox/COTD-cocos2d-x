#ifndef __COTDGRID_SCENE_H__
#define __COTDGRID_SCENE_H__

#include "cocos2d.h"

#include "COTDImage.h"
#include "HFCollectionView.hpp"
#include <strstream>


USING_NS_CC;
using namespace std;

using namespace cocos2d;

class COTDGrid : public cocos2d::Layer, public HFCollectionViewDelegate, public HFCollectionViewDataSource
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
    
    /*
     virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
     virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
     virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
     virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;
*/
//    virtual int numberOfItemsInSection(HFCollectionView* collectionView, int section) override;
//    virtual int numberOfSectionsInCollectionView(HFCollectionView * collectionView) override;
//    virtual Size collectionCellSizeForIndex(HFCollectionView *collection, ssize_t idx) override;
//    virtual Size cellSizeForCollection(HFCollectionView *collection) override;
//    virtual HFCollectionViewCell* collectionViewCellAtIndex(HFCollectionView *collectionView, ssize_t idx) override;
//    virtual ssize_t numberOfCellsInCollectionView(HFCollectionView *collectionView) override;

    // implement the "static create()" method manually
    
    
    CREATE_FUNC(COTDGrid);

//    virtual void collectionCellWillRecycle(HFCollectionView* collectionView, HFCollectionViewCell* cell) override;
//    virtual void collectionCellHighlight(HFCollectionView* collectionView, HFCollectionViewCell* cell) override;
//    virtual void collectionCellUnhighlight(HFCollectionView* collectionView, HFCollectionViewCell* cell) override;
//    virtual void collectionCellTouched(HFCollectionView* collectionView, HFCollectionViewCell* cell) override;

    virtual void collectionCellTouched(HFCollectionView* collectionView, HFCollectionViewCell* cell);
    virtual HFCollectionViewCell* collectionViewCellAtIndex(HFCollectionView *collectionView, ssize_t idx);
    virtual int numberOfItemsInSection(HFCollectionView* collectionView, int section);

    
};

#endif // __COTDGRID_SCENE_H__
