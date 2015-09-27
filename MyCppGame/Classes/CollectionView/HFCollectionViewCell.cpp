//
//  HFCollectionViewCell.cpp
//  Hungry & Foolish
//
//  Created by Javier Fuchs on 9/21/15.
//
//

#include "HFCollectionViewCell.hpp"

HFCollectionReusableView::HFCollectionReusableView()
: _reuseIdentifier(__String::create("HF"))
{
}

void HFCollectionReusableView::prepareForReuse()
{
}

void HFCollectionReusableView::setReuseIdentifier(__String *reuseIdentifier)
{
    _reuseIdentifier = reuseIdentifier;
}


HFView *HFCollectionViewCell::contentView()
{
    return nullptr;
}
    
bool HFCollectionViewCell::isSelected()
{
    return false;
}

void HFCollectionViewCell::setSelected(bool value)
{
    _selected = value;
}

bool HFCollectionViewCell::isHighlighted()
{
    return false;
}

void HFCollectionViewCell::setHighlighted(bool value)
{
    _highlighted = value;
}
    

HFView *HFCollectionViewCell::backgroundView()
{
    return nullptr;
}

HFView *HFCollectionViewCell::selectedBackgroundView()
{
    return nullptr;
}

void HFCollectionViewCell::reset()
{
    _indexPath.row = (int)CC_INVALID_INDEX;
    _indexPath.column = (int)CC_INVALID_INDEX;
}

const HFIndexPath& HFCollectionViewCell::getIndexPath() const
{
    return _indexPath;
}

void HFCollectionViewCell::setIndexPath(const HFIndexPath& indexPath)
{
    _indexPath = indexPath;
}
