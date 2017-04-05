/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#define IW_USE_SYSTEM_STDLIB
#include "s3eFlurry_internal.h"
#include "IwDebug.h"
#import "Flurry/FlurryAPI.h"
#import <UIKit/UIKit.h>

// Globals
static bool s_BatchEvents = false;



// Private Function implementations
//----------------------------------------------------------------------------

static NSMutableDictionary *CreateParamDictionary(const char* params)
{
    NSString* paramsString = [NSString stringWithUTF8String:params];
    NSArray* paramsArray = [paramsString componentsSeparatedByString:@"|"];

    int numParams = [paramsArray count] / 2;
    NSMutableDictionary* _params = NULL;
    if (numParams > 0)
    {
        _params = [NSMutableDictionary dictionaryWithCapacity:numParams];

        for (int t = 0; t < numParams; t++)
        {
            NSString* _key = [paramsArray objectAtIndex:t*2];
            NSString* _value = [paramsArray objectAtIndex:t*2+1];
            [_params setObject:_value forKey:_key];
        }
    }

    return _params;
}

// Public Function implementations
//----------------------------------------------------------------------------

s3eResult s3eFlurryInit_platform()
{
    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void s3eFlurryTerminate_platform()
{
    // Add any platform-specific termination code here
}

void s3eFlurryStartSession_platform(const char* apiKey)
{
    IwTrace(FLURRY_VERBOSE,("\n * s3eFlurryStartSession_platform... *\nAPI KEY:"));

    NSString* _apiKey = [NSString stringWithFormat: @"%s", apiKey];
    [Flurry startSession:_apiKey];
}

void s3eFlurryEndSession_platform()
{
    IwTrace(FLURRY_VERBOSE,("\n * s3eFlurryEndSession_platform"));
}

void s3eFlurryLogEvent_platform(const char* eventName)
{
    NSString* _eventName = [NSString stringWithFormat: @"%s", eventName];

    [Flurry logEvent:_eventName timed:NO]; 
}

void s3eFlurryLogEventTimed_platform(const char* eventName)
{
    NSString* _eventName = [NSString stringWithFormat: @"%s", eventName];

    [Flurry logEvent:_eventName timed:YES];
}

void s3eFlurryLogEventParams_platform(const char* eventName, const char* eventParams)
{
 
    IwTrace(FLURRY_VERBOSE,("\n * s3eFlurryLogEventParams_platform... *\n"));
    
    NSString* _eventName = [NSString stringWithFormat: @"%s", eventName];
    NSMutableDictionary* _params = CreateParamDictionary(eventParams);

    [Flurry logEvent:_eventName withParameters:_params timed:NO];
}

void s3eFlurryLogEventParamsTimed_platform(const char* eventName, const char* eventParams)
{
    NSString* _eventName = [NSString stringWithFormat: @"%s", eventName];
    NSMutableDictionary* _params = CreateParamDictionary(eventParams);

    [Flurry logEvent:_eventName withParameters:_params timed:YES];
}

void s3eFlurryEndTimedEvent_platform(const char* eventName, const char* eventParams)
{
    NSString *_eventName = [NSString stringWithFormat: @"%s", eventName];
    NSMutableDictionary* _params = CreateParamDictionary(eventParams);

    [Flurry endTimedEvent:_eventName withParameters:_params];
}

void s3eFlurryLogError_platform(const char* errorName, const char* message)
{
    NSString *_errorName = [NSString stringWithFormat: @"%s", errorName];
    NSString *_message = [NSString stringWithFormat: @"%s", message];

    [Flurry logError:_errorName message:_message exception:NULL];
}

void s3eFlurrySetUserID_platform(const char* userId)
{
    NSString *_userId = [NSString stringWithFormat: @"%s", userId];

    [Flurry setUserID:_userId];
}

void s3eFlurrySetAge_platform(int32 age)
{
    [Flurry setAge:age];
}

void s3eFlurryCountPageView_platform()
{
    [Flurry logPageView];
}

void s3eFlurrySetSessionReportsOnCloseEnabled_platform(s3eBool sendSessionReportsOnClose)
{
    BOOL _sendSessionReportsOnClose = (sendSessionReportsOnClose == S3E_TRUE) ? YES : NO;

    [Flurry setSessionReportsOnCloseEnabled:_sendSessionReportsOnClose];
}

void s3eFlurrySetSessionReportsOnPauseEnabled_platform(s3eBool sendSessionReportsOnPause)
{
    BOOL _sendSessionReportsOnPause = (sendSessionReportsOnPause == S3E_TRUE) ? YES : NO;

    [Flurry setSessionReportsOnPauseEnabled:_sendSessionReportsOnPause];
}
