//
//  COTDCollectionView.h
//  MyCppGame
//
//  Created by Javier Fuchs on 9/18/15.
//
//

#ifndef __MyCppGame__COTDCollectionView__
#define __MyCppGame__COTDCollectionView__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class COTDCollectionViewCell;
class COTDCollectionViewDataSource;
class COTDCollectionViewDelegate;


USING_NS_CC;
USING_NS_CC_EXT;


class COTDCollectionView: public ScrollView, public ScrollViewDelegate
{
public:
    COTDCollectionView();
    virtual ~COTDCollectionView();
    static COTDCollectionView* create();
    static COTDCollectionView* create(COTDCollectionViewDataSource* dataSource, COTDCollectionViewDelegate* delegate, Size size);
    static COTDCollectionView* create(COTDCollectionViewDataSource* dataSource, COTDCollectionViewDelegate* delegate, Size size, Node *container);
    void setMultipleSelectEnabled(bool flag);
    void setBackGroundViewWithFile(const char* fileName);
    void reloadData();
    std::set<ssize_t>* getSelectedIndices(){return _selectedIndices;};
    COTDCollectionViewCell* dequeueCell();

private:
    bool initWithViewSize(Size size, Node* container = NULL);
    void setDataSource(COTDCollectionViewDataSource* dataSource);
    COTDCollectionViewDataSource* getDataSource() {return _dataSource;}
    void setDelegate(COTDCollectionViewDelegate* delegate);
    COTDCollectionViewDelegate* getDelegate() {return _delegate;}
    void updateCellAtIndex(ssize_t idx);
    void insertCellAtIndex(ssize_t idx);
    void removeCellAtIndex(ssize_t idx);
    COTDCollectionViewCell *cellAtIndex(ssize_t idx);
    bool getMultipleSelectEnabled(){return _multipleTouchEnabled;};
    // Overrides
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidZoom(ScrollView* view)  override {}
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;

    void updateCellPositions();
    void updateContentSize();

    long _indexFromOffset(Point offset);
    long indexFromOffset(Point offset);
    Point _offsetFromIndex(ssize_t index);
    Point offsetFromIndex(ssize_t index);

    void moveCellOutOfSight(COTDCollectionViewCell *cell);
    void setIndexForCell(ssize_t index, COTDCollectionViewCell *cell);
    void addCellIfNecessary(COTDCollectionViewCell * cell);

    ssize_t getIndexWithTouchPoint(Point point);

protected:

    Direction _oldDirection;
    COTDCollectionViewDelegate* _delegate;
    COTDCollectionViewDataSource* _dataSource;
    Sprite* _backgroundView;
    ssize_t _row;
    ssize_t _col;
    float _leftSpace;
    float _upSpace;
    float _verticalSpace;
    float _horizonalSpace;
    COTDCollectionViewCell* _touchedCell;
    std::set<ssize_t>* _indices;
    std::vector<Point> _vCellsPositions;
    Vector<COTDCollectionViewCell*> _cellsUsed;
    Vector<COTDCollectionViewCell*> _cellsFreed;

    bool _multipleTouchEnabled;
    std::set<ssize_t>* _selectedIndices;
    __Array *_cellsSelected;

    bool _isUsedCellsDirty;

};

#pragma mark DataSource

class COTDCollectionViewDataSource
{
public:
    virtual COTDCollectionViewCell* collectionCellAtIndex(COTDCollectionView* collection, ssize_t idx) = 0;
    virtual ssize_t numberOfCellsInCollection(COTDCollectionView* collection) = 0;
};


#pragma mark Delegate

class COTDCollectionViewDelegate: public ScrollViewDelegate
{
public:
    virtual void collectionCellTouched(COTDCollectionView* collection, COTDCollectionViewCell* cell) = 0;
    virtual void collectionCellHighlight(COTDCollectionView* collection, COTDCollectionViewCell* cell){};
    virtual void collectionCellUnhighlight(COTDCollectionView* collection, COTDCollectionViewCell* cell){};
    virtual cocos2d::Size collectionCellSizeForIndex(COTDCollectionView* collection, ssize_t idx){
        return cellSizeForCollection(collection);
    };
    virtual cocos2d::Size cellSizeForCollection(COTDCollectionView* collection) {
        return cocos2d::Size::ZERO;
    };
    virtual float leftSideSpaceForCollection(COTDCollectionView* collection){return 0;}
    virtual float upSideSpaceForCollection(COTDCollectionView* collection){return 0;}
    virtual void collectionCellWillRecycle(COTDCollectionView* collection, COTDCollectionViewCell* cell){};

};


#endif /* defined(__MyCppGame__COTDCollectionView__) */
