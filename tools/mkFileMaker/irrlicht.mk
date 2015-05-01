LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := irrlicht_static

LOCAL_MODULE_FILENAME := libirrlicht

LOCAL_SRC_FILES := \
$(CPP_FILES)
    
	
LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/. \
                           $(LOCAL_PATH)/.. \
                           $(LOCAL_PATH)/bzip \
                           $(LOCAL_PATH)/libpng 

include $(BUILD_STATIC_LIBRARY)

