//
//  HFCollectionViewCell.hpp
//  Hungry & Foolish
//
//  Created by Javier Fuchs on 9/21/15.
//
//

#ifndef HFCollectionViewCell_hpp
#define HFCollectionViewCell_hpp

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#include "HFView.hpp"
#include "HFCollectionView.hpp"


class HFCollectionReusableView : public HFView
{
protected:
    __String *_reuseIdentifier;
public:
    // Override point.
    // Called by the collection view before the instance is returned from the reuse queue.
    // Subclassers must call super.
    void prepareForReuse();
    
    __String *reuseIdentifier();
    void setReuseIdentifier(__String *reuseIdentifier);
    
    HFCollectionReusableView();
};


class HFCollectionViewCell : public HFCollectionReusableView
{
protected:
    HFView *_contentView; // add custom subviews to the cell's contentView
    
    // Cells become highlighted when the user touches them.
    // The selected state is toggled when the user lifts up from a highlighted cell.
    // Override these methods to provide custom UI for a selected or highlighted state.
    // The collection view may call the setters inside an animation block.
    bool _selected;
    bool _highlighted;

    // The background view is a subview behind all other views.
    // If selectedBackgroundView is different than backgroundView, it will be placed above the background view and animated in on selection.
    HFView *_backgroundView;
    HFView *_selectedBackgroundView;

public:
    HFView *contentView();

    bool isSelected();
    bool isHighlighted();

// The background view is a subview behind all other views.
// If selectedBackgroundView is different than backgroundView, it will be placed above the background view and animated in on selection.
    HFView *backgroundView();
    HFView *selectedBackgroundView();

};


#endif /* HFCollectionViewCell_hpp */
