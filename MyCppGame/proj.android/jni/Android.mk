LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
		   ../../Classes/Model/COTDImage.cpp \
		   ../../Classes/Model/COTDUserImage.cpp \
		   ../../Classes/Scenes/COTDGridScene.cpp \
		   ../../Classes/Scenes/COTDMainScene.cpp \
		   ../../Classes/Singleton/COTDGoogle.cpp \
		   ../../Classes/Singleton/COTDParse.cpp \
		   ../../Classes/Util/COTDDateType.cpp \
		   ../../Classes/Util/COTDLog.cpp

LOCAL_C_INCLUDES := 	$(LOCAL_PATH)/../../Classes \
			$(LOCAL_PATH)/../../Classes/Model \
			$(LOCAL_PATH)/../../Classes/Scenes \
			$(LOCAL_PATH)/../../Classes/Singleton \
			$(LOCAL_PATH)/../../Classes/Util \
			$(LOCAL_PATH)/../../cocos2d/cocos/network

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
