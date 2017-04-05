/*
 * iphone-specific implementation of the iOSNativeEDK extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "iOSNativeEDK_internal.h"
#import <Social/Social.h>
#include "s3eTypes.h"
#include "s3eEdk.h"
#include "s3eEdk_iphone.h"
#import <UIKit/UIKit.h>
#include "IwDebug.h"
#import <iAd/iAd.h>

s3eResult iOSNativeEDKInit_platform()
{
    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void iOSNativeEDKTerminate_platform()
{
    // Add any platform-specific termination code here
}

s3eResult Social_Twitter_platform(const char* text)
{
    //if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter])
    //{
        SLComposeViewController *tweetSheet = [SLComposeViewController
                                               composeViewControllerForServiceType:SLServiceTypeTwitter];
        [tweetSheet setInitialText:[NSString stringWithUTF8String:text]];
        [s3eEdkGetUIViewController() presentViewController:tweetSheet animated:YES completion:nil];
    //}
    return S3E_RESULT_ERROR;
}

s3eResult Social_Facebook_platform(const char* text)
{
    SLComposeViewController *controller = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
    
    [controller setInitialText:[NSString stringWithUTF8String:text]];
    [s3eEdkGetUIViewController() presentViewController:controller animated:YES completion:Nil];
    
    return S3E_RESULT_ERROR;
}

s3eResult iOS_Orientation_platform(bool bLandscape)
{
    return S3E_RESULT_ERROR;
}
