//
//  HFCollectionView.cpp
//  Hungry & Foolish
//
//  Created by Javier Fuchs on 9/21/15.
//
//

#include "HFCollectionView.hpp"
#include "HFCollectionViewCell.hpp"


HFCollectionView::HFCollectionView()
: _touchedCell(nullptr)
, _backgroundView(nullptr)
, _dataSource(nullptr)
, _delegate(nullptr)
, _allowsSelection(true)
, _allowsMultipleSelection(false)
, _indices(nullptr)
, _oldDirection(Direction::NONE)
, _vordering(VerticalFillOrder::BOTTOM_UP)
{
    
}

HFCollectionView::~HFCollectionView()
{
    CC_SAFE_DELETE(_backgroundView);
    CC_SAFE_DELETE(_indices);
}


HFCollectionView* HFCollectionView::create(HFCollectionViewDataSource* dataSource, Size size, Node *container)
{
    HFCollectionView *collection = new (std::nothrow) HFCollectionView();
    if (collection->initWithViewSize(size, container))
    {
        ((ScrollView*)collection)->autorelease();
        collection->setDataSource(dataSource);
        collection->_updateCellPositions();
        collection->_updateContentSize();

        return collection;
    }
    
    return nullptr;
}

bool HFCollectionView::initWithSize(Size size, Node *container)
{
    if (ScrollView::initWithViewSize(size, container))
    {
        CC_SAFE_DELETE(_indices);
        _indices        = new std::set<ssize_t>();
        this->setDirection(Direction::VERTICAL);

        ScrollView::setDelegate(this);
        return true;
    }
    return false;
}

void HFCollectionView::setVerticalFillOrder(VerticalFillOrder fillOrder)
{
    if (_vordering != fillOrder)
    {
        _vordering = fillOrder;
        if (!_cellsUsed.empty())
        {
            this->reloadData();
        }
    }
}


//void HFCollectionView::selectItemAtIndexPath(HFIndexPath* indexPath, bool animated)
//{
//    
//}
//
//void HFCollectionView::deselectItemAtIndexPath(HFIndexPath* indexPath, bool animated)
//{
//    
//}


void HFCollectionView::reloadData()
{
    _oldDirection = Direction::NONE;
    
    for(const auto &cell : _cellsUsed) {
        if(_delegate != nullptr) {
            _delegate->collectionCellWillRecycle(this, cell);
            cell->setHighlighted(false);
            cell->setSelected(false);
        }
        
        _cellsFreed.pushBack(cell);
        
        cell->reset();
        if (cell->getParent() == this->getContainer())
        {
            this->getContainer()->removeChild(cell, true);
        }
    }
    
    _indices->clear();
    _cellsUsed.clear();
    
    this->_updateCellPositions();
    this->_updateContentSize();
    if (_dataSource->numberOfItemsInSection(this, 0) > 0)
    {
        this->scrollViewDidScroll(this);
    }
}


// Information about the current state of the collection view.

//HFIndexPath *HFCollectionView::indexPathForItemAtPoint(Point point)
//{
//    return nullptr;
//}
//
//HFIndexPath *HFCollectionView::indexPathForCell(HFCollectionViewCell* cell)
//{
//    return nullptr;
//}
//
//HFCollectionViewCell *HFCollectionView::cellForItemAtIndexPath(HFIndexPath* indexPath)
//{
//    return nullptr;
//}


#pragma mark -
#pragma mark private

void HFCollectionView::_updateContentSize()
{
    Size size = Size::ZERO;
    ssize_t cellsCount = _dataSource->numberOfItemsInSection(this, 0);
    
    if (cellsCount > 0)
    {
        float maxPosition = _vCellsPositions[cellsCount];
        
        switch (this->getDirection())
        {
            case Direction::HORIZONTAL:
                size = Size(maxPosition, _viewSize.height);
                break;
            default:
                size = Size(_viewSize.width, maxPosition);
                break;
        }
    }
    
    ((ScrollView*)this)->setContentSize(size);
    
    if (_oldDirection != _direction)
    {
        if (_direction == Direction::HORIZONTAL)
        {
            this->setContentOffset(Vec2(0,0));
        }
        else
        {
            this->setContentOffset(Vec2(0,this->minContainerOffset().y));
        }
        _oldDirection = _direction;
    }
    
}


void HFCollectionView::_updateCellPositions()
{
    long cellsCount = _dataSource->numberOfItemsInSection(this, 0);
    _vCellsPositions.resize(cellsCount + 1, 0.0);
    
    if (cellsCount > 0)
    {
        float currentPos = 0;
        Size cellSize;
        for (int i=0; i < cellsCount; i++)
        {
            _vCellsPositions[i] = currentPos;
            cellSize = _dataSource->collectionCellSizeForIndex(this, i);
            switch (this->getDirection())
            {
                case Direction::HORIZONTAL:
                    currentPos += cellSize.width;
                    break;
                default:
                    currentPos += cellSize.height;
                    break;
            }
        }
        _vCellsPositions[cellsCount] = currentPos; // 1 extra value allows us to get right/bottom of the last cell
    }
    
}

long HFCollectionView::_indexFromOffset(Vec2 offset)
{
    long index = 0;
    const long maxIdx = _dataSource->numberOfItemsInSection(this, 0) - 1;
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y;
    }
    index = this->__indexFromOffset(offset);
    if (index != -1)
    {
        index = MAX(0, index);
        if (index > maxIdx)
        {
            index = CC_INVALID_INDEX;
        }
    }
    
    return index;
}

long HFCollectionView::__indexFromOffset(Vec2 offset)
{
    long low = 0;
    long high = _dataSource->numberOfItemsInSection(this, 0) - 1;
    float search;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            search = offset.x;
            break;
        default:
            search = offset.y;
            break;
    }
    
    while (high >= low)
    {
        long index = low + (high - low) / 2;
        float cellStart = _vCellsPositions[index];
        float cellEnd = _vCellsPositions[index + 1];
        
        if (search >= cellStart && search <= cellEnd)
        {
            return index;
        }
        else if (search < cellStart)
        {
            high = index - 1;
        }
        else
        {
            low = index + 1;
        }
    }
    
    if (low <= 0) {
        return 0;
    }
    
    return -1;
}

Vec2 HFCollectionView::_offsetFromIndex(ssize_t index)
{
    Vec2 offset = this->__offsetFromIndex(index);
    
    const Size cellSize = _dataSource->collectionCellSizeForIndex(this, index);
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y - cellSize.height;
    }
    return offset;
}

Vec2 HFCollectionView::__offsetFromIndex(ssize_t index)
{
    Vec2 offset;
    Size  cellSize;
    
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            offset.set(_vCellsPositions[index], 0.0f);
            break;
        default:
            offset.set(0.0f, _vCellsPositions[index]);
            break;
    }
    
    return offset;
}


void HFCollectionView::_updateCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    long countOfItems = _dataSource->numberOfItemsInSection(this, 0);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }
    
    HFCollectionViewCell* cell = this->_cellAtIndex(idx);
    if (cell)
    {
        this->_moveCellOutOfSight(cell);
    }
    cell = _dataSource->collectionViewCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
}


HFCollectionViewCell *HFCollectionView::_cellAtIndex(ssize_t idx)
{
    if (_indices->find(idx) != _indices->end())
    {
        for (const auto& cell : _cellsUsed)
        {
            if (cell->getIdx() == idx)
            {
                return cell;
            }
        }
    }
    
    return nullptr;
}

void HFCollectionView::_moveCellOutOfSight(HFCollectionViewCell *cell)
{
    if (_delegate != nullptr)
    {
        _delegate->collectionCellWillRecycle(this, cell);
        cell->setHighlighted(false);
        cell->setSelected(false);
    }
    
    _cellsFreed.pushBack(cell);
    _cellsUsed.eraseObject(cell);
    _isDirty = true;
    
    _indices->erase(cell->getIdx());
    cell->reset();
    
    if (cell->getParent() == this->getContainer())
    {
        this->getContainer()->removeChild(cell, true);;
    }
}


void HFCollectionView::_setIndexForCell(ssize_t index, HFCollectionViewCell *cell)
{
    cell->setAnchorPoint(Vec2(0.0f, 0.0f));
    cell->setPosition(this->_offsetFromIndex(index));
    cell->setIdx(index);
}

void HFCollectionView::_addCellIfNecessary(HFCollectionViewCell * cell)
{
    if (cell->getParent() != this->getContainer())
    {
        this->getContainer()->addChild(cell);
    }
    _cellsUsed.pushBack(cell);
    _indices->insert(cell->getIdx());
    _isDirty = true;
}

#pragma mark -
#pragma mark ScrollView overrides
void HFCollectionView::scrollViewDidScroll(ScrollView* view)
{
    long countOfItems = _dataSource->numberOfItemsInSection(this, 0);
    if (0 == countOfItems)
    {
        return;
    }
    
    if (_isDirty)
    {
        _isDirty = false;
        std::sort(_cellsUsed.begin(), _cellsUsed.end(), [](HFCollectionViewCell *a, HFCollectionViewCell *b) -> bool{
            return a->getIdx() < b->getIdx();
        });
    }
    
    if(_delegate != nullptr) {
        _delegate->scrollViewDidScroll(this);
    }
    
    ssize_t startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    Vec2 offset = this->getContentOffset() * -1;
    maxIdx = MAX(countOfItems-1, 0);
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = offset.y + _viewSize.height/this->getContainer()->getScaleY();
    }
    startIdx = this->_indexFromOffset(offset);
    if (startIdx == CC_INVALID_INDEX)
    {
        startIdx = countOfItems - 1;
    }
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y -= _viewSize.height/this->getContainer()->getScaleY();
    }
    else
    {
        offset.y += _viewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += _viewSize.width/this->getContainer()->getScaleX();
    
    endIdx   = this->_indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX)
    {
        endIdx = countOfItems - 1;
    }
    
#if 0 // For Testing.
    Ref* pObj;
    int i = 0;
    CCARRAY_FOREACH(_cellsUsed, pObj)
    {
        HFCollectionViewCell* pCell = static_cast<HFCollectionViewCell*>(pObj);
        log("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(_cellsFreed, pObj)
    {
        HFCollectionViewCell* pCell = static_cast<HFCollectionViewCell*>(pObj);
        log("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif
    
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.at(0);
        idx = cell->getIdx();
        
        while(idx < startIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.at(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.back();
        idx = cell->getIdx();
        
        while(idx <= maxIdx && idx > endIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.back();
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    
    for (long i = startIdx; i <= endIdx; i++)
    {
        if (_indices->find(i) != _indices->end())
        {
            continue;
        }
        this->_updateCellAtIndex(i);
    }
}




void HFCollectionView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!((ScrollView*)this)->isVisible())
    {
        return;
    }
    
    if (_touchedCell)
    {
        Rect bb = ((ScrollView*)this)->getBoundingBox();
        Node* parent = ((ScrollView*)this)->getParent();
        bb.origin = parent->convertToWorldSpace(bb.origin);
        
        if (bb.containsPoint(pTouch->getLocation()) && _delegate != nullptr)
        {
            _delegate->collectionCellUnhighlight(this, _touchedCell);
            _touchedCell->setHighlighted(false);
            _delegate->collectionCellTouched(this, _touchedCell);
            _touchedCell->setSelected(true);
        }
        
        _touchedCell = nullptr;
    }
    
    ScrollView::onTouchEnded(pTouch, pEvent);
}

bool HFCollectionView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    for (Node *c = (ScrollView*)this; c != nullptr; c = c->getParent())
    {
        if (!c->isVisible())
        {
            return false;
        }
    }
    
    bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);
    
    if(_touches.size() == 1)
    {
        long index;
        Vec2 point;
        
        point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        
        index = this->_indexFromOffset(point);
        if (index == CC_INVALID_INDEX)
        {
            _touchedCell = nullptr;
        }
        else
        {
            _touchedCell = this->_cellAtIndex(index);
        }
        
        if (_touchedCell && _delegate != nullptr)
        {
            _delegate->collectionCellHighlight(this, _touchedCell);
            _touchedCell->setHighlighted(true);
        }
    }
    else if (_touchedCell)
    {
        if(_delegate != nullptr)
        {
            _delegate->collectionCellUnhighlight(this, _touchedCell);
            _touchedCell->setHighlighted(false);
        }
        
        _touchedCell = nullptr;
    }
    
    return touchResult;
}

void HFCollectionView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchMoved(pTouch, pEvent);
    
    if (_touchedCell && isTouchMoved())
    {
        if(_delegate != nullptr)
        {
            _delegate->collectionCellUnhighlight(this, _touchedCell);
            _touchedCell->setHighlighted(false);
        }
        
        _touchedCell = nullptr;
    }
}

void HFCollectionView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchCancelled(pTouch, pEvent);
    
    if (_touchedCell)
    {
        if(_delegate != nullptr)
        {
            _delegate->collectionCellUnhighlight(this, _touchedCell);
            _touchedCell->setHighlighted(false);
        }
        
        _touchedCell = nullptr;
    }
}
