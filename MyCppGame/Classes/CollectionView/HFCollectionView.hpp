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

struct HFIndexPath {
    int section;
    int row;
};

class HFCollectionView;
class HFCollectionViewCell;

class HFCollectionViewDataSource
{
public:
    virtual ~HFCollectionViewDataSource() {}

    // Required
    // --------
    
    virtual int numberOfItemsInSection(HFCollectionView* collectionView, int section);

    // The cell that is returned must be retrieved from a call to -dequeueReusableCellWithReuseIdentifier:forIndexPath:
    virtual HFCollectionViewCell* cellForItemAtIndexPath(HFCollectionView* collectionView, HFIndexPath *index);
  
    
    // Optional
    // --------
    
    virtual int numberOfSectionsInCollectionView(HFCollectionView * collectionView) = 0;

};

USING_NS_CC_EXT;

class HFCollectionViewDataDelegate : public ScrollViewDelegate
{
public:
    // Optional
    // --------
    virtual void didSelectItemAtIndexPath(HFCollectionView* collectionView, HFIndexPath* indexPath) = 0;
    virtual void didHighlightItemAtIndexPath(HFCollectionView* collectionView, HFIndexPath* indexPath) = 0;
    virtual void didUnhighlightItemAtIndexPath(HFCollectionView* collectionView, HFIndexPath* indexPath) = 0;


};

class HFCollectionView : public HFView, public ScrollView
{
protected:
    HFView* _backgroundView;
    HFCollectionViewDataSource* _dataSource;
    HFCollectionViewDataDelegate* _delegate;

    
    // These properties control whether items can be selected, and if so, whether multiple items can be simultaneously selected.
    bool _allowsSelection; // default is YES
    bool _allowsMultipleSelection; // default is NO
    
    Vector<HFIndexPath*> indexPathsForSelectedItems;

    void selectItemAtIndexPath(HFIndexPath* indexPath, bool animated);
    void deselectItemAtIndexPath(HFIndexPath* indexPath, bool animated);
    
    void reloadData(); // discard the dataSource and delegate data and requery as necessary

    // Information about the current state of the collection view.
    
    int numberOfSections();
    int numberOfItemsInSection(int section);
    
    HFIndexPath *indexPathForItemAtPoint(Point point);
    HFIndexPath *indexPathForCell(HFCollectionViewCell* cell);

    HFCollectionViewCell *cellForItemAtIndexPath(HFIndexPath* indexPath);
    Vector<HFCollectionViewCell*> visibleCells;
    Vector<HFIndexPath*> indexPathsForVisibleItems;
    
};

#endif /* HFCollectionView_hpp */
