/*
 * iphone-specific implementation of the LeadboltExt extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "LeadboltExt_internal.h"
#include "LeadboltOverlay.h"
#include "s3eEdk_iphone.h"


s3eResult LeadboltExtInit_platform()
{
    // Add any platform-specific initialisation code here
    
    return S3E_RESULT_SUCCESS;
}

void LeadboltExtTerminate_platform()
{
    // Add any platform-specific termination code here
}

s3eResult loadStartAd_platform(const char* sectionid, const char* reengagementid, const char* audioid)
{
    NSString *sectionId = [NSString stringWithUTF8String:sectionid];
    NSString *reengagementId = [NSString stringWithUTF8String:reengagementid];
    NSString *audioId = [NSString stringWithUTF8String:audioid];
    
    LeadboltOverlay* myOverlay = [LeadboltOverlay createAdWithSectionid:sectionId view:s3eEdkGetUIView()];
    [myOverlay loadStartAd:reengagementId withAudio:audioId];
    
    return S3E_RESULT_SUCCESS;
}

s3eResult loadAd_platform(const char* sectionid)
{
    NSString *sectionId = [NSString stringWithUTF8String:sectionid];

    LeadboltOverlay* myOverlay = [LeadboltOverlay createAdWithSectionid:sectionId view:s3eEdkGetUIView()];
    [myOverlay loadAd];

    return S3E_RESULT_SUCCESS;
}

s3eResult destroyAd_platform(const char* sectionid)
{
    return S3E_RESULT_SUCCESS;
}

s3eResult pauseAd_platform(const char* sectionid)
{
    return S3E_RESULT_SUCCESS;
}

s3eResult resumeAd_platform(const char* sectionid)
{
    return S3E_RESULT_SUCCESS;
}

s3eResult loadAudioAd_platform(const char* sectionid)
{
    NSString *sectionId = [NSString stringWithUTF8String:sectionid];
    
    LeadboltOverlay* myOverlay = [LeadboltOverlay createAdWithSectionid:sectionId view:s3eEdkGetUIView()];
    [myOverlay loadAudioAd];    

    [myOverlay shakeMotionStarted];
    return S3E_RESULT_SUCCESS;
}

s3eResult shakeMotionStarted_platform(const char* sectionid)
{
    return S3E_RESULT_SUCCESS;
}

s3eResult shakeMotionEnded_platform(const char* sectionid)
{
    NSString *sectionId = [NSString stringWithUTF8String:sectionid];
    
    LeadboltOverlay* myOverlay = [LeadboltOverlay createAdWithSectionid:sectionId view:s3eEdkGetUIView()];
    [myOverlay loadAudioAd];
    //[myOverlay pauseAd];
    
    [myOverlay shakeMotionEnded];
    return S3E_RESULT_SUCCESS;
}

s3eResult loadReEngagement_platform(const char* sectionid)
{
    NSString *sectionId = [NSString stringWithUTF8String:sectionid];
    
    LeadboltOverlay* myOverlay = [LeadboltOverlay createAdWithSectionid:sectionId view:s3eEdkGetUIView()];
    [myOverlay loadReEngagement];
    return S3E_RESULT_SUCCESS;
}

s3eResult reEngagementLoaded_platform(const char* sectionid)
{
    return S3E_RESULT_SUCCESS;
}

s3eResult loadBanner_platform(const char* sectionid)
{
    NSString *sectionId = [NSString stringWithUTF8String:sectionid];
    
    LeadboltOverlay* myOverlay = [LeadboltOverlay createAdWithSectionid:sectionId view:s3eEdkGetUIView()];
    [myOverlay setAdditionalDockingMargin:0];
    [myOverlay loadAd];
    return S3E_RESULT_SUCCESS;
}

s3eResult loadBannerWithMargin_platform(const char* sectionid, int margin)
{
    NSString *sectionId = [NSString stringWithUTF8String:sectionid];
    NSNumber *marginNum = [NSNumber numberWithInt:margin];
    
    LeadboltOverlay* myOverlay = [LeadboltOverlay createAdWithSectionid:sectionId view:s3eEdkGetUIView()];
    [myOverlay setAdditionalDockingMargin:marginNum];
    [myOverlay loadAd];
    return S3E_RESULT_SUCCESS;
}


s3eResult setLandscapeMode_platform(const char* newMode)
{
    NSString *modeString = [NSString stringWithUTF8String:newMode];

    [LeadboltOverlay setLandscapeMode:modeString];
    return S3E_RESULT_SUCCESS;
}

s3eResult setLocationControl_platform(const char* newLocationControl)
{
    NSString *locationString = [NSString stringWithUTF8String:newLocationControl];

    [LeadboltOverlay setLocationControl:locationString];
    return S3E_RESULT_SUCCESS;
}


