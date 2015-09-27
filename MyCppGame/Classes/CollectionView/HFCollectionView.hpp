//
//  HFCollectionView.hpp
//  Hungry & Foolish
//
//  Created by Javier Fuchs on 9/21/15.
//
//

#ifndef HFCollectionView_hpp
#define HFCollectionView_hpp

#include "HFView.hpp"
//#include "CCScrollView.h"
//#include <extensions/GUI/ScrollView/CCScrollView.h>
#include "cocos-ext.h"


class HFCollectionView;
class HFCollectionViewCell;

class HFCollectionViewDataSource
{
public:

    // Required
    // --------
    
    // Information about the current state of the collection view.

    virtual int numberOfRowsInColumn(HFCollectionView* collectionView, int column) = 0;
    
    // Optional
    // --------
    virtual int numberOfColumnsInCollectionView(HFCollectionView * collectionView)
    {
        return 1;
    };
    
    virtual Size collectionCellSizeForIndex(HFCollectionView *collection, const HFIndexPath& indexPath) {
        return cellSizeForCollection(collection);
    };

    virtual Size cellSizeForCollection(HFCollectionView *collection) {
        return Size::ZERO;
    };

    virtual HFCollectionViewCell* cellForItemAtIndexPath(HFCollectionView* collectionView, const HFIndexPath& indexPath) = 0;

};

USING_NS_CC_EXT;

class HFCollectionViewDelegate : public ScrollViewDelegate
{
public:
    // Optional
    // --------
    virtual void collectionCellWillRecycle(HFCollectionView* collectionView, HFCollectionViewCell* cell){};
    virtual void collectionCellHighlight(HFCollectionView* collectionView, HFCollectionViewCell* cell){};
    virtual void collectionCellUnhighlight(HFCollectionView* collectionView, HFCollectionViewCell* cell){};
    virtual void collectionCellTouched(HFCollectionView* collectionView, HFCollectionViewCell* cell) = 0;

};


class HFCollectionView : public ScrollView, public HFView, public ScrollViewDelegate
{
protected:
    enum class VerticalFillOrder
    {
        TOP_DOWN,
        BOTTOM_UP
    };

    HFView* _backgroundView;
    HFCollectionViewDataSource* _dataSource;
    HFCollectionViewDelegate* _delegate;
    Direction _oldDirection;
    
    // These properties control whether items can be selected, and if so, whether multiple items can be simultaneously selected.
    bool _allowsSelection; // default is YES
    bool _allowsMultipleSelection; // default is NO
    
//    Vector<HFIndexPath*> _indexPathsForSelectedItems;
//    Vector<HFCollectionViewCell*> _visibleCells;
//    Vector<HFIndexPath*> _indexPathsForVisibleItems;

    /**
     * index set to query the indexes of the cells used.
     */
    std::set<HFIndexPath *>* _indices;
    
    /**
     * vector with all cell positions
     */
    std::vector<float> _vCellsPositions;
    std::vector<float> _hCellsPositions;

    /**
     * cells that are currently in the collectionView
     */
    Vector<HFCollectionViewCell*> _cellsUsed;

    /**
     * free list of cells
     */
    Vector<HFCollectionViewCell*> _cellsFreed;

    bool _isDirty;

    /**
     * vertical direction of cell filling
     */
    VerticalFillOrder _vordering;

    HFCollectionViewCell *_touchedCell;

private:
    
    void _updateContentSize();
    void _updateContentSize(int column);
    void _updateCellPositions();
    long __indexFromOffset(Vec2 offset, int column);
    long _indexFromOffset(Vec2 offset, int column);
    Vec2 _offsetFromIndex(const HFIndexPath& indexPath);
    Vec2 __offsetFromIndex(const HFIndexPath& indexPath);

    void _updateCellAtIndex(const HFIndexPath& indexPath);
    HFCollectionViewCell *_cellAtIndex(const HFIndexPath& indexPath);
    void _moveCellOutOfSight(HFCollectionViewCell *cell);
    void _setIndexForCell(const HFIndexPath& indexPath, HFCollectionViewCell *cell);
    void _addCellIfNecessary(HFCollectionViewCell * cell);

    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;

public:
    HFCollectionView();
    virtual ~HFCollectionView();

    bool initWithSize(Size size, Node* container = NULL);
    static HFCollectionView* create(HFCollectionViewDataSource* dataSource, Size size, Node *container = nullptr);

//    void selectItemAtIndexPath(HFIndexPath* indexPath, bool animated);
//    void deselectItemAtIndexPath(HFIndexPath* indexPath, bool animated);
    
    HFCollectionViewCell *dequeueCell();
    void reloadData(); // discard the dataSource and delegate data and requery as necessary

//    HFIndexPath *indexPathForItemAtPoint(Point point);
//    HFIndexPath *indexPathForCell(HFCollectionViewCell* cell);

//    HFCollectionViewCell *cellForItemAtIndexPath(HFIndexPath* indexPath);
    
    HFCollectionViewDataSource* getDataSource() { return _dataSource; }
    void setDataSource(HFCollectionViewDataSource* source) { _dataSource = source; }
    HFCollectionViewDelegate* getDelegate() { return _delegate; }
    void setDelegate(HFCollectionViewDelegate* delegate) { _delegate = delegate; }

//    HFCollectionViewCell *dequeueReusableCell(__String* identifier, HFIndexPath* indexPath);

    /**
     * determines how cell is ordered and filled in the view.
     */
    void setVerticalFillOrder(VerticalFillOrder order);

    // Overrides
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidScrollInColumn(ScrollView* view, int column);

};

#endif /* HFCollectionView_hpp */
