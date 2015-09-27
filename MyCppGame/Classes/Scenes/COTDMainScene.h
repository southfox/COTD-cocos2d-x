#ifndef __COTDMAIN_SCENE_H__
#define __COTDMAIN_SCENE_H__

#include "cocos2d.h"
#include <assets-manager/Downloader.h>

#include <strstream>

class COTDImage;
class CCActivityIndicator;

USING_NS_CC;
USING_NS_CC_EXT;

class COTDMain : public Layer
{
protected:
    std::shared_ptr<Downloader> _downloader;
    CCActivityIndicator *_activityIndicator;
    MenuItemLabel *_likeButton;
    MenuItemLabel *_gridButton;

private:
    MenuItemImage* createCloseButton();
    MenuItemLabel* createLikeButton();
    MenuItemLabel* createGridButton();
    void configureTitle();
    void configureImage(const std::string &imageName);
    void configureMenu();

    // Google
    void searchGoogle();
    void googleSearchCallback(bool succeeded,
                              const std::string& link,
                              const std::string& thumbnailLink,
                              const std::string& title,
                              std::strstream& error);

    void createSpinner();
    void onSpinnerStart();
    void onSpinnerEnd();

    // Parse.com
    void queryParse();

    // a selector callback
    void menuLikeCallback(Ref* pSender);
    void menuGridCallback(Ref* pSender);
    void onExit();
    void menuCloseCallback(Ref* pSender);

    void download(const COTDImage *currentUserImage);
    void onDownloadError(const Downloader::Error &error);
    void onDownloadProgress(double total, double downloaded, const std::string &url, const std::string &customId);
    void onDownloadSuccess(const std::string &srcUrl, const std::string &storagePath, const std::string &customId);

    void onUpdateImage(bool succeeded, std::strstream& error);
    void onLikeCurrentImage(bool succeeded, std::strstream& error);
    

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(COTDMain);
    
};

#endif // __COTDMAIN_SCENE_H__
