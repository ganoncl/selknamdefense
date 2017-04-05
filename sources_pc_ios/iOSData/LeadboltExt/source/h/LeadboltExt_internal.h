/*
 * Internal header for the LeadboltExt extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef LEADBOLTEXT_INTERNAL_H
#define LEADBOLTEXT_INTERNAL_H

#include "s3eTypes.h"
#include "LeadboltExt.h"
#include "LeadboltExt_autodefs.h"

/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult LeadboltExtInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult LeadboltExtInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void LeadboltExtTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void LeadboltExtTerminate_platform();

s3eResult loadStartAd_platform(const char* sectionid, const char* reengagementid, const char* audioid);

s3eResult loadAd_platform(const char* sectionid);

s3eResult destroyAd_platform(const char* sectionid);

s3eResult pauseAd_platform(const char* sectionid);

s3eResult resumeAd_platform(const char* sectionid);

s3eResult loadAudioAd_platform(const char* sectionid);

s3eResult shakeMotionStarted_platform(const char* sectionid);

s3eResult shakeMotionEnded_platform(const char* sectionid);

s3eResult loadReEngagement_platform(const char* sectionid);

s3eResult reEngagementLoaded_platform(const char* sectionid);

s3eResult loadBanner_platform(const char* sectionid);

s3eResult loadBannerWithMargin_platform(const char* sectionid, int margin);

s3eResult setLandscapeMode_platform(const char* newMode);

s3eResult setLocationControl_platform(const char* newLocationControl);


#endif /* !LEADBOLTEXT_INTERNAL_H */
