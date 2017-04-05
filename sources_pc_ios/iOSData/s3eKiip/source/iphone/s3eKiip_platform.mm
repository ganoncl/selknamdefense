/*
 * iphone-specific implementation of the s3eKiip extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
 
// Kiip uses ARC, at this time Marmalade doesn't support CLANG, this is a horrible-horrible hack to redefine strong as retain to get the Kiip headers to include
#define strong retain

#include "s3eKiip_internal.h"
#include "s3eEdk.h"
#import <UIKit/UIApplication.h>
#import "Kiip.h"
#import "KPPoptart.h"

// Helper functions
// convert an NSString to a utf8 char* caller must dispose of memory using s3eEdkFreeOS
static char* CopyToCString(NSString* string)
{
    if (string)
    {
        const char* pStr = [string UTF8String];
        char* pCopy = (char*) s3eEdkMallocOS(strlen(pStr) + 1);
        if (pCopy)
        {
            strcpy(pCopy, pStr);
            return pCopy;
        }
    }
    return NULL;
}

// we need to marshall data for the C++ callback and deallocate when the callback is finished
// marmalade callbacks are queued until the next yield and this will be called at that point
static void s3eKiipReceivedContentDeallocator(uint32 extID, int32 notification, void *systemData, void *instance, int32 returnCode, void *completeData)
{
    if (!systemData) return;
    s3eKiipReceivedContent *content = (s3eKiipReceivedContent*)systemData;
    if (content->content) s3eEdkFreeOS((void*)content->content);
    if (content->transactionID)s3eEdkFreeOS((void*)content->transactionID);
    if (content->signature) s3eEdkFreeOS((void*)content->signature);
    s3eEdkFreeOS(content);
}

// Simple Kiip Delegate
//----------------------------------------------------------------------------

@interface s3eKiipDelegate : NSObject <KiipDelegate>
@end // s3eKiipDelegate interface

@implementation s3eKiipDelegate

//#define MODAL_ERROR_DIALOGS

//
- (void)showError:(NSError *)error
{
#ifdef MODAL_ERROR_DIALOGS
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"KiipSDK Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
    [alert show];
    [alert release];
#endif
}

- (void) kiip:(Kiip *)kiip didStartSessionWithPoptart:(KPPoptart *)poptart error:(NSError *)error
{
    NSLog(@"kiip:didStartSessionWithPoptart:%@ error:%@", poptart, error);

    if (s3eEdkCallbacksIsRegistered(S3E_EXT_KIIP_HASH,S3EKIIP_SESSION_START))
    {
        char *errorMsg = NULL;
        if (error) {
            errorMsg = CopyToCString(error.localizedDescription);
        }
        s3eEdkCallbacksEnqueue(S3E_EXT_KIIP_HASH,S3EKIIP_SESSION_START,errorMsg,((errorMsg)?strlen(errorMsg)+1:0));
        if (errorMsg) {
            s3eEdkFreeOS(errorMsg);
        }
    }
    else if (error) {
        [self showError:error];
    }
    [poptart show];
}

- (void) kiip:(Kiip *)kiip didEndSessionWithError:(NSError *)error
{
    NSLog(@"kiip:didEndSessionWithError:%@", error);

    if (s3eEdkCallbacksIsRegistered(S3E_EXT_KIIP_HASH,S3EKIIP_SESSION_END))
    {
        char *errorMsg = NULL;
        if (error)
            errorMsg = CopyToCString(error.localizedDescription);
        s3eEdkCallbacksEnqueue(S3E_EXT_KIIP_HASH,S3EKIIP_SESSION_END,errorMsg,((errorMsg)?strlen(errorMsg)+1:0));
        if (errorMsg)
            s3eEdkFreeOS(errorMsg);
    }
    else if (error) // assume C++ callback has handled the error if the callback was set
        [self showError:error]; // otherwise display something
}

- (void) kiip:(Kiip *)kiip didReceiveContent:(NSString *)contentId quantity:(int)quantity transactionId:(NSString *)transactionId signature:(NSString *)signature
{
    NSLog(@"kiip:didReceiveContent:%@ quantity:%d transactionId:%@ signature:%@", contentId, quantity, transactionId, signature);
    if (s3eEdkCallbacksIsRegistered(S3E_EXT_KIIP_HASH,S3EKIIP_CONTENT_RECEIVED))
    {
        // marshall the strings into our custom structure
        s3eKiipReceivedContent *cont = (s3eKiipReceivedContent*)s3eEdkMallocOS(sizeof(s3eKiipReceivedContent));
        cont->content = (contentId)? CopyToCString(contentId):NULL;
        cont->transactionID = (transactionId)? CopyToCString(transactionId):NULL;
        cont->signature = (signature)? CopyToCString(signature):NULL;
        cont->quantity = quantity;

        // queue the callback and specify our deallocator which kicks in after the callback completes - the deallocator isn't
        // invoked until well after the scope of this function exits.
        // the sysDataSize is zero which means Marmalade won't make a copy
        s3eEdkCallbacksEnqueue(S3E_EXT_KIIP_HASH,S3EKIIP_CONTENT_RECEIVED,cont,0,NULL,false,s3eKiipReceivedContentDeallocator,cont);
    }
}


@end // s3eKiipDelegate implementation

s3eResult s3eKiipInit_platform()
{
    // Add any platform-specific initialisation code here
	// This code is automatically called the first time any Kiip function is called
    return S3E_RESULT_SUCCESS;
}

static Kiip *gKiip; // global Kiip singleton

void s3eKiipTerminate_platform()
{
    if (gKiip)
    {
        NSLog(@"s3eKiip released");
        [gKiip.delegate release]; // we are C++ and not self so explicitly release the delegate
        gKiip.delegate = nil;
        [gKiip release];
        gKiip = nil;
    }
}

s3eResult s3eKiipInitWithKeys_platform(const char* appKey, const char* appSecret)
{
    if (gKiip)
    {
        NSLog(@"s3eKiipInit called more than once");
        return S3E_RESULT_ERROR;
    }

    NSString* nsAppKey = [[NSString alloc] initWithUTF8String:appKey];        //[NSString stringWithUTF8String: appKey];
    NSString* nsAppSecret = [[NSString alloc] initWithUTF8String:appSecret];  // [NSString stringWithUTF8String: appSecret];
    NSLog(@"s3eKiipInit called with %@ , %@",nsAppKey,nsAppSecret);

    @try {
        gKiip = [[Kiip alloc] initWithAppKey:nsAppKey andSecret:nsAppSecret];
    }
    @catch (NSException *exception) { // catch all exceptions so Marmalade keeps going
        NSLog(@"s3eKiipInitWithKeys: Caught %@: %@", [exception name], [exception reason]);
        NSLog(@"StacK: %@", [exception callStackSymbols]);
        return S3E_RESULT_ERROR;
    }

    if (gKiip)
    {
        gKiip.delegate = [[s3eKiipDelegate alloc] init];
        [Kiip setSharedInstance:gKiip];
    }

    return ((gKiip)?S3E_RESULT_SUCCESS:S3E_RESULT_ERROR);
}

void s3eKiipRegisterCallback_platform(s3eKiipCallbackType callbackID, s3eCallback callbackFn, void* userData)
{
}

s3eResult s3eKiipSaveMoment_platform(const char* momentID)
{
    if ((gKiip)&&(momentID))
    {
        @try {
            NSString *momentId = [NSString stringWithUTF8String: momentID];
            [gKiip saveMoment:momentId withCompletionHandler:^(KPPoptart *poptart, NSError *error)
             {
                 if (s3eEdkCallbacksIsRegistered(S3E_EXT_KIIP_HASH,S3EKIIP_SAVE_MOMENT_COMPLETION))
                 {
                     char *errorMsg = NULL;
                     if (error)
                         errorMsg = CopyToCString(error.localizedDescription);
                     s3eEdkCallbacksEnqueue(S3E_EXT_KIIP_HASH,S3EKIIP_SAVE_MOMENT_COMPLETION,errorMsg,((errorMsg)?strlen(errorMsg)+1:0)); // invoke the C++ callback
                     if (errorMsg)
                         s3eEdkFreeOS(errorMsg);
                 }
                 else if (error)
                 { // be helpful ?
#ifdef MODAL_ERROR_DIALOGS
                     UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                     [alert show];
                     [alert release];
#endif
                 }

                 // Since we've implemented this block, Kiip will no longer show the poptart automatically
                 [poptart show];
             }];
        }
        @catch (NSException *exception) { // catch all exceptions so Marmalade keeps going
            NSLog(@"s3eKiipSaveMoment: Caught %@: %@", [exception name], [exception reason]);
            NSLog(@"StacK: %@", [exception callStackSymbols]);
            return S3E_RESULT_ERROR;
        }
    }
    return S3E_RESULT_ERROR;
}

s3eResult s3eKiipSaveMomentWithValue_platform(const char* momentID, double* value)
{
        if ((gKiip)&&(momentID))
    {
        @try {
            NSString *momentId = [NSString stringWithUTF8String: momentID];
            [gKiip saveMoment:momentId value:*value withCompletionHandler:^(KPPoptart *poptart, NSError *error)
             {
                 if (s3eEdkCallbacksIsRegistered(S3E_EXT_KIIP_HASH,S3EKIIP_SAVE_MOMENT_COMPLETION))
                 {
                     char *errorMsg = NULL;
                     if (error)
                         errorMsg = CopyToCString(error.localizedDescription);
                     s3eEdkCallbacksEnqueue(S3E_EXT_KIIP_HASH,S3EKIIP_SAVE_MOMENT_COMPLETION,errorMsg,((errorMsg)?strlen(errorMsg)+1:0)); // invoke the C++ callback
                     if (errorMsg)
                         s3eEdkFreeOS(errorMsg);
                 }
                 else if (error)
                 { // be helpful ?
#ifdef MODAL_ERROR_DIALOGS
                     UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                     [alert show];
                     [alert release];
#endif
                 }

                 // Since we've implemented this block, Kiip will no longer show the poptart automatically
                 [poptart show];
             }];
        }
        @catch (NSException *exception) { // catch all exceptions so Marmalade keeps going
            NSLog(@"s3eKiipSaveMomentWithValue: Caught %@: %@", [exception name], [exception reason]);
            NSLog(@"StacK: %@", [exception callStackSymbols]);
            return S3E_RESULT_ERROR;
        }
    }
    return S3E_RESULT_ERROR;
}

void s3eKiipSetAutorotate_platform(bool shouldAutoRotate)
{
	if (gKiip)
    {
        gKiip.shouldAutoRotate = shouldAutoRotate;
    }
}

bool s3eKiipGetAutorotate_platform()
{
    if (gKiip)
        return gKiip.shouldAutoRotate;
    return false;
}

void s3eKiipSetInterfaceOrientation_platform(s3eUIInterfaceOrientation orientation)
{
	if (gKiip)
        gKiip.interfaceOrientation = (UIInterfaceOrientation)orientation;
}

s3eUIInterfaceOrientation s3eKiipGetInterfaceOrientation_platform()
{
    if (gKiip)
        return (s3eUIInterfaceOrientation)gKiip.interfaceOrientation;
    return s3eUIInterfaceOrientationUnknown;
}

void s3eKiipSetEmailAddress_platform(const char* email)
{
	if ((gKiip)&&(email))
    {
        NSString *nsemail = [NSString stringWithUTF8String:email];
        gKiip.email = nsemail;
    }
}

void s3eKiipSetGender_platform(const char* gender)
{
	if ((gKiip)&&(gender))
    {
        NSString *nsgender = [NSString stringWithUTF8String:gender];
        gKiip.gender = nsgender;
    }
}

void s3eKiipSetBirthday_platform(const char* dob)
{
	if ((gKiip)&&(dob))
    {
        NSString *nsdob = [NSString stringWithUTF8String:dob];
        NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];

        [dateFormat setDateFormat:@"yyyy-MM-dd"];
        gKiip.birthday = [dateFormat dateFromString:nsdob];
        
        NSLog(@"s3eKiipSetBirthday %@",gKiip.birthday);
        [dateFormat release];
    }
}
