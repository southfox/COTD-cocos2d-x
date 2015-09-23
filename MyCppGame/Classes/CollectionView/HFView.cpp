//
//  HFView.cpp
//  Hungry & Foolish
//
//  Created by Javier Fuchs on 9/21/15.
//
//

#include "HFView.hpp"

USING_NS_CC;


HFView* HFView::initWithFrame(const Rect& frame)
{
    HFView* view = new HFView();
    if (view && view->init(frame))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}


bool HFView::init(const Rect& frame)
{
    _frame = frame;
    return true;
}
