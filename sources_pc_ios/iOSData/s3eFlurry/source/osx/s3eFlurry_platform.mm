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
 * osx-specific implementation of the s3eFlurry extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eFlurry_internal.h"

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
}

void s3eFlurryEndSession_platform()
{
}

void s3eFlurryLogEvent_platform(const char* eventName)
{
}

void s3eFlurryLogEventTimed_platform(const char* eventName)
{
}

void s3eFlurryLogEventParams_platform(const char* eventName, const char* eventParams)
{
}

void s3eFlurryLogEventParamsTimed_platform(const char* eventName, const char* eventParams)
{
}

void s3eFlurryEndTimedEvent_platform(const char* eventName, const char* eventParams)
{
}

void s3eFlurryLogError_platform(const char* errorName, const char* message)
{
}

void s3eFlurrySetUserID_platform(const char* userId)
{
}

void s3eFlurrySetAge_platform(int32 age)
{
}

void s3eFlurryCountPageView_platform()
{
}

void s3eFlurrySetSessionReportsOnCloseEnabled_platform(s3eBool sendSessionReportsOnClose)
{
}

void s3eFlurrySetSessionReportsOnPauseEnabled_platform(s3eBool sendSessionReportsOnPause)
{
}
