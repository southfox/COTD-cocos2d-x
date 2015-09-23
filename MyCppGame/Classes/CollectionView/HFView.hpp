//
//  HFView.hpp
//  Hungry & Foolish
//
//  Created by Javier Fuchs on 9/21/15.
//
//

#ifndef HFView_hpp
#define HFView_hpp

#include "cocos2d.h"
#include "base/CCDirector.h"

USING_NS_CC;

class HFView : public Node
{
protected:
    // default is YES. if set to NO, user events (touch, keys) are ignored and removed from the event queue.
    bool _userInteractionEnabled;
    int _tag;                // default is 0
    Rect _frame;

private:
    bool init(const Rect& frame);
    
public:
    static HFView* initWithFrame(const Rect& frame);

};

#endif /* HFView_hpp */
