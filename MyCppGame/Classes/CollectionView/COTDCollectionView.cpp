//
//  COTDCollectionView.cpp
//  MyCppGame
//
//  Created by Javier Fuchs on 9/18/15.
//
//
// based on https://github.com/LanouTechnology/CCCollectionView/blob/master/CCCollectionView/CCCollectionView.cpp


#include "COTDCollectionView.h"
#include "COTDCollectionViewCell.h"

COTDCollectionView::COTDCollectionView()
: _multipleTouchEnabled(false)
, _cellsSelected(nullptr)
, _touchedCell(nullptr)
, _indices(nullptr)
, _selectedIndices(nullptr)
, _dataSource(nullptr)
, _delegate(nullptr)
, _backgroundView(nullptr)
, _oldDirection(Direction::NONE)
, _isUsedCellsDirty(false)
, _row(0)
, _col(0)
{
    
}


COTDCollectionView::~COTDCollectionView()
{
    CC_SAFE_DELETE(_indices);
    CC_SAFE_DELETE(_selectedIndices);
    CC_SAFE_RELEASE(_cellsSelected);
}


COTDCollectionView* COTDCollectionView::create()
{
    return COTDCollectionView::create(nullptr, nullptr, Size::ZERO);
}

COTDCollectionView* COTDCollectionView::create(COTDCollectionViewDataSource* dataSource, COTDCollectionViewDelegate* delegate, Size size)
{
    return COTDCollectionView::create(dataSource, delegate, size, NULL);
}

COTDCollectionView* COTDCollectionView::create(COTDCollectionViewDataSource* dataSource, COTDCollectionViewDelegate* delegate, Size size, Node *container)
{
    COTDCollectionView *collection = new COTDCollectionView();
    collection->initWithViewSize(size, container);
    collection->autorelease();
    collection->setDataSource(dataSource);
    collection->setDelegate(delegate);
    collection->updateCellPositions();
    collection->updateContentSize();
    
    return collection;
}



bool COTDCollectionView::initWithViewSize(Size size, Node* container/* = NULL*/)
{
    if (ScrollView::initWithViewSize(size,container))
    {
        CC_SAFE_DELETE(_indices);
        _indices        = new std::set<ssize_t>();
        _selectedIndices = new std::set<ssize_t>();
        this->setDirection(Direction::VERTICAL);
        ScrollView::setDelegate(this);
        return true;
    }
    return false;
}


void COTDCollectionView::setBackGroundViewWithFile(const char *fileName)
{
    if(_backgroundView)
    {
        Texture2D *texture2D = Director::getInstance()->getTextureCache()->addImage(fileName);
        _backgroundView->setTexture(texture2D);
        Size size = texture2D->getContentSize();
        _backgroundView->setTextureRect(Rect(0, 0, size.width, size.height));
    }
    else
    {
        _backgroundView = Sprite::create(fileName);
        Point position = Point(this->getViewSize().width*0.5f, this->getViewSize().height*0.5f);
        _backgroundView->cocos2d::Node::setPosition(position);
        _backgroundView->setAnchorPoint(Point(0.5,0.5));
        _backgroundView->setGlobalZOrder(-1);
        this->insertChild(_backgroundView, 0);
    }
}


void COTDCollectionView::updateCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    long countOfItems = _dataSource->numberOfCellsInCollection(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }
    
    COTDCollectionViewCell* cell = this->cellAtIndex(idx);
    if (cell)
    {
        this->moveCellOutOfSight(cell);
    }
    cell = _dataSource->collectionCellAtIndex(this, idx);
    this->setIndexForCell(idx, cell);
    this->addCellIfNecessary(cell);
}

void COTDCollectionView::insertCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    long countOfItems = _dataSource->numberOfCellsInCollection(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }
    
    long newIdx = 0;
    
    auto cell = cellAtIndex(idx);
    if (cell)
    {
        newIdx = _cellsUsed.getIndex(cell);
        for (long i = newIdx; i < _cellsUsed.size(); i++)
        {
            cell = _cellsUsed.at(i);
            this->setIndexForCell(cell->getIdx()+1, cell);
        }
    }
    
    cell = _dataSource->collectionCellAtIndex(this, idx);
    this->setIndexForCell(idx, cell);
    this->addCellIfNecessary(cell);
    
    this->updateCellPositions();
    this->updateContentSize();
}


void COTDCollectionView::removeCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    
    long uCountOfItems = _dataSource->numberOfCellsInCollection(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }
    
    ssize_t newIdx = 0;
    
    COTDCollectionViewCell* cell = this->cellAtIndex(idx);
    if (!cell)
    {
        return;
    }
    
    newIdx = _cellsUsed.getIndex(cell);
    
    this->moveCellOutOfSight(cell);
    
    _indices->erase(idx);
    this->updateCellPositions();
    
    for (ssize_t i = _cellsUsed.size()-1; i > newIdx; i--)
    {
        cell = _cellsUsed.at(i);
        this->setIndexForCell(cell->getIdx()-1, cell);
    }
}


void COTDCollectionView::reloadData()
{
    _oldDirection = Direction::NONE;
    
    for(const auto &cell : _cellsUsed)
    {
        if(_delegate != NULL)
        {
            _delegate->collectionCellWillRecycle(this, cell);
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
    
    this->updateCellPositions();
    this->updateContentSize();
    if (_dataSource->numberOfCellsInCollection(this) > 0)
    {
        this->scrollViewDidScroll(this);
    }
}

void COTDCollectionView::setDelegate(COTDCollectionViewDelegate *delegate)
{
    _delegate = delegate;
    _leftSpace = _delegate->leftSideSpaceForCollection(this);
    _upSpace = _delegate->upSideSpaceForCollection(this);
    
    _col = (this->getViewSize().width-_leftSpace*2)/_delegate->collectionCellSizeForIndex(this, 0).width;
    _horizonalSpace = (this->getViewSize().width-_leftSpace*2-_col*delegate->collectionCellSizeForIndex(this, 0).width)/(_col-1);
    _verticalSpace = _horizonalSpace;
    _row = _dataSource->numberOfCellsInCollection(this)/_col+1;
}

void COTDCollectionView::setDataSource(COTDCollectionViewDataSource *dataSource)
{
    _dataSource = dataSource;
}

void COTDCollectionView::setMultipleSelectEnabled(bool flag)
{
    if(flag)
    {
        _multipleTouchEnabled = true;
        _cellsSelected = __Array::createWithCapacity(2);
        _cellsSelected->retain();
    }
    else
    {
        _multipleTouchEnabled = false;
        _cellsSelected->removeAllObjects();
        _selectedIndices->clear();
        CC_SAFE_RELEASE(_cellsSelected);
    }
}

#pragma mark - Cell

COTDCollectionViewCell* COTDCollectionView::dequeueCell()
{
    COTDCollectionViewCell *cell;
    
    if (_cellsFreed.empty()) {
        cell = NULL;
    } else {
        cell = _cellsFreed.at(0);
        cell->retain();
        _cellsFreed.erase(0);
        cell->autorelease();
    }
    return cell;
}


COTDCollectionViewCell* COTDCollectionView::cellAtIndex(ssize_t idx)
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


#pragma mark - Behaviour
void COTDCollectionView::scrollViewDidScroll(ScrollView* view)
{
    long countOfItems = _dataSource->numberOfCellsInCollection(this);
    if (0 == countOfItems)
    {
        return;
    }
    
    if (_isUsedCellsDirty)
    {
        _isUsedCellsDirty = false;
        std::sort(_cellsUsed.begin(), _cellsUsed.end(), [](COTDCollectionViewCell *a, COTDCollectionViewCell *b) -> bool{
            return a->getIdx() < b->getIdx();
        });
    }
    
    if(_delegate != NULL) {
        _delegate->scrollViewDidScroll(this);
    }
    
    ssize_t startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    Point offset = this->getContentOffset() * -1;
    maxIdx = MAX(countOfItems-1, 0);
    
    offset.y = offset.y + _viewSize.height/this->getContainer()->getScaleY();
    startIdx = this->_indexFromOffset(offset);
    if (startIdx == CC_INVALID_INDEX)
    {
        startIdx = countOfItems - 1;
    }
    
    startIdx = startIdx/_col*_col;
    
    offset.y -= _viewSize.height/this->getContainer()->getScaleY();
    offset.x += _viewSize.width/this->getContainer()->getScaleX();
    
    endIdx   = this->_indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX)
    {
        endIdx = countOfItems - 1;
    }
    endIdx = (endIdx/_col+1)*_col-1;
    endIdx = endIdx>countOfItems-1 ? countOfItems-1:endIdx;
#if 0 // For Testing.
    Ref* pObj;
    int i = 0;
    CCARRAY_FOREACH(_cellsUsed, pObj)
    {
        CollectionViewCell* pCell = static_cast<CollectionViewCell*>(pObj);
        log("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(_cellsFreed, pObj)
    {
        CollectionViewCell* pCell = static_cast<CollectionViewCell*>(pObj);
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
            this->moveCellOutOfSight(cell);
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
            this->moveCellOutOfSight(cell);
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
        this->updateCellAtIndex(i);
    }
}


bool COTDCollectionView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);
    
    if(_touches.size() == 1)
    {
        long index;
        Point point;
        
        point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        this->getIndexWithTouchPoint(point);
        //        index = this->_indexFromOffset(point);
        index = this->getIndexWithTouchPoint(point);
        if (index == CC_INVALID_INDEX)
        {
            _touchedCell = NULL;
        }
        else
        {
            _touchedCell  = this->cellAtIndex(index);
        }
        
        if(_touchedCell && _multipleTouchEnabled)
        {
            if(_cellsSelected->containsObject(_touchedCell))
            {
                _cellsSelected->removeObject(_touchedCell);
                _selectedIndices->erase(_touchedCell->getIdx());
            }
            else
            {
                _cellsSelected->addObject(_touchedCell);
                _selectedIndices->insert(_touchedCell->getIdx());
            }
        }
        
        if (_touchedCell && _delegate != NULL)
        {
            _delegate->collectionCellHighlight(this, _touchedCell);
        }
    }
    else if (_touchedCell)
    {
        if(_delegate != NULL)
        {
            _delegate->collectionCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = NULL;
    }
    
    return touchResult;
}


void COTDCollectionView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchMoved(pTouch, pEvent);
    
    if (_touchedCell && isTouchMoved())
    {
        if(_delegate != NULL)
        {
            _delegate->collectionCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = NULL;
    }
}

void COTDCollectionView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return;
    }
    
    if (_touchedCell){
        Rect bb = this->getBoundingBox();
        bb.origin = _parent->convertToWorldSpace(bb.origin);
        
        if (bb.containsPoint(pTouch->getLocation()) && _delegate != NULL)
        {
            _delegate->collectionCellUnhighlight(this, _touchedCell);
            _delegate->collectionCellTouched(this, _touchedCell);
        }
        
        _touchedCell = NULL;
    }
    
    ScrollView::onTouchEnded(pTouch, pEvent);
}

void COTDCollectionView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchCancelled(pTouch, pEvent);
    
    if (_touchedCell)
    {
        if(_delegate != NULL)
        {
            _delegate->collectionCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = NULL;
    }
}

#pragma mark - private
void COTDCollectionView::updateCellPositions()
{
    long cellsCount = _dataSource->numberOfCellsInCollection(this);
    _vCellsPositions.resize(cellsCount + 1, Point(0,0));
    
    if (cellsCount > 0)
    {
        float xOffset = 0;
        float yOffset = 0;
        Size cellSize;
        for (int i=0; i < cellsCount+1; i++)
        {
            cellSize = _delegate->collectionCellSizeForIndex(this, 0);
            if(i%_col == 0)
            {
                xOffset = 0;
            }
            int horizontalIndex = i%_col;
            int verticalIndex = i/_col;
            xOffset = _leftSpace + horizontalIndex*(cellSize.width+_horizonalSpace);
            yOffset = _upSpace + verticalIndex*(cellSize.height+_verticalSpace);
            //            _vCellsPositions[i]->x = xOffset;
            //            _vCellsPositions[i]->y = yOffset;
            _vCellsPositions[i] = Point(xOffset, yOffset);
        }
        //        _vCellsPositions[cellsCount] = currentPos;//1 extra value allows us to get right/bottom of the last cell
    }
}


void COTDCollectionView::updateContentSize()
{
    Size size = Size::ZERO;
    ssize_t cellsCount = _dataSource->numberOfCellsInCollection(this);
    
    if (cellsCount > 0)
    {
        float maxPosition = 0;
        Size cellSize = _delegate->collectionCellSizeForIndex(this, 0);
        ssize_t row = (cellsCount/_col);
        if(cellsCount%_col!=0)
        {
            row+=1;
        }
        maxPosition = _upSpace + row*(cellSize.height+_verticalSpace);
        size = Size(_viewSize.width, maxPosition);
    }
    this->setContentSize(size);
    
    if (_oldDirection != _direction)
    {
        this->setContentOffset(Point(0,this->minContainerOffset().y));
        _oldDirection = _direction;
    }
}

void COTDCollectionView::addCellIfNecessary(COTDCollectionViewCell * cell)
{
    if (cell->getParent() != this->getContainer())
    {
        this->getContainer()->addChild(cell);
    }
    _cellsUsed.pushBack(cell);
    _indices->insert(cell->getIdx());
    _isUsedCellsDirty = true;
}

Point COTDCollectionView::offsetFromIndex(ssize_t index)
{
    Point offset = this->_offsetFromIndex(index);
    
    const Size cellSize = _delegate->collectionCellSizeForIndex(this, index);
    offset.y = this->getContainer()->getContentSize().height - offset.y - cellSize.height;
    return offset;
}

Point COTDCollectionView::_offsetFromIndex(ssize_t index)
{
    Point offset;
    Size  cellSize;
    offset = Point(_vCellsPositions[index].x, _vCellsPositions[index].y);
    return offset;
}

long COTDCollectionView::indexFromOffset(Point offset)
{
    long index = 0;
    const long maxIdx = _dataSource->numberOfCellsInCollection(this) - 1;
    
    offset.y = this->getContainer()->getContentSize().height - offset.y;
    index = this->_indexFromOffset(offset);
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

long COTDCollectionView::_indexFromOffset(Point offset)
{
    long low = 0;
    long high = _dataSource->numberOfCellsInCollection(this) - 1;
    float search;
    search = offset.y;
    while (high >= low)
    {
        long index = low + (high - low) / 2;
        float cellStart = _vCellsPositions[index].y;
        float cellEnd = _vCellsPositions[index + 1].y;
        
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


void COTDCollectionView::moveCellOutOfSight(COTDCollectionViewCell *cell)
{
    if(_delegate != NULL) {
        _delegate->collectionCellWillRecycle(this, cell);
    }
    
    _cellsFreed.pushBack(cell);
    _cellsUsed.eraseObject(cell);
    _isUsedCellsDirty = true;
    
    _indices->erase(cell->getIdx());
    cell->reset();
    
    if (cell->getParent() == this->getContainer())
    {
        this->getContainer()->removeChild(cell, true);;
    }
}


void COTDCollectionView::setIndexForCell(ssize_t index, COTDCollectionViewCell *cell)
{
    cell->setAnchorPoint(Point(0.0f, 0.0f));
    cell->setPosition(this->offsetFromIndex(index));
    cell->setIdx(index);
}

ssize_t COTDCollectionView::getIndexWithTouchPoint(Point point)
{
    for (int i=0; i<_cellsUsed.size(); i++)
    {
        COTDCollectionViewCell* cell = _cellsUsed.at(i);
        Rect rect = cell->getImageView()->getTextureRect();
        AffineTransform affineTransform = cell->getNodeToParentAffineTransform();
        rect = RectApplyAffineTransform(rect, affineTransform);
        if(rect.containsPoint(point))
        {
            return cell->getIdx();
            break;
        }
    }
    return -1;
}

