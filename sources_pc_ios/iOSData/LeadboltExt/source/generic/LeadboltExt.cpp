/*
Generic implementation of the LeadboltExt extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "LeadboltExt_internal.h"
s3eResult LeadboltExtInit()
{
    //Add any generic initialisation code here
    return LeadboltExtInit_platform();
}

void LeadboltExtTerminate()
{
    //Add any generic termination code here
    LeadboltExtTerminate_platform();
}

s3eResult loadStartAd(const char* sectionid, const char* reengagementid, const char* audioid)
{
	return loadStartAd_platform(sectionid, reengagementid, audioid);
}

s3eResult loadAd(const char* sectionid)
{
	return loadAd_platform(sectionid);
}

s3eResult destroyAd(const char* sectionid)
{
	return destroyAd_platform(sectionid);
}

s3eResult pauseAd(const char* sectionid)
{
	return pauseAd_platform(sectionid);
}

s3eResult resumeAd(const char* sectionid)
{
	return resumeAd_platform(sectionid);
}

s3eResult loadAudioAd(const char* sectionid)
{
	return loadAudioAd_platform(sectionid);
}

s3eResult shakeMotionStarted(const char* sectionid)
{
	return shakeMotionStarted_platform(sectionid);
}

s3eResult shakeMotionEnded(const char* sectionid)
{
	return shakeMotionEnded_platform(sectionid);
}

s3eResult loadReEngagement(const char* sectionid)
{
	return loadReEngagement_platform(sectionid);
}

s3eResult reEngagementLoaded(const char* sectionid)
{
	return reEngagementLoaded_platform(sectionid);
}

s3eResult loadBanner(const char* sectionid)
{
	return loadBanner_platform(sectionid);
}

s3eResult loadBannerWithMargin(const char* sectionid, int margin)
{
	return loadBannerWithMargin_platform(sectionid, margin);
}

s3eResult setLandscapeMode(const char* newMode)
{
	return setLandscapeMode_platform(newMode);
}

s3eResult setLocationControl(const char* newLocationControl)
{
	return setLocationControl_platform(newLocationControl);
}

