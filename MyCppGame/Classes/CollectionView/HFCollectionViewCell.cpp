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
    _idx = CC_INVALID_INDEX;
}

ssize_t HFCollectionViewCell::getIdx() const
{
    return _idx;
}

void HFCollectionViewCell::setIdx(ssize_t idx)
{
    _idx = idx;
}
