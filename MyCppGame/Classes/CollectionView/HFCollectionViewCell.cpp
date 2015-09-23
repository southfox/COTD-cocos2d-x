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

bool HFCollectionViewCell::isHighlighted()
{
    return false;
}
    

HFView *HFCollectionViewCell::backgroundView()
{
    return nullptr;
}

HFView *HFCollectionViewCell::selectedBackgroundView()
{
    return nullptr;
}
