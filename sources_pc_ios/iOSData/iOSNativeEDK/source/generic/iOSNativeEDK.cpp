/*
Generic implementation of the iOSNativeEDK extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "iOSNativeEDK_internal.h"
s3eResult iOSNativeEDKInit()
{
    //Add any generic initialisation code here
    return iOSNativeEDKInit_platform();
}

void iOSNativeEDKTerminate()
{
    //Add any generic termination code here
    iOSNativeEDKTerminate_platform();
}

s3eResult Social_Twitter(const char* text)
{
	return Social_Twitter_platform(text);
}

s3eResult Social_Facebook(const char* text)
{
	return Social_Facebook_platform(text);
}

s3eResult iOS_Orientation(bool bLandscape)
{
	return iOS_Orientation_platform(bLandscape);
}
