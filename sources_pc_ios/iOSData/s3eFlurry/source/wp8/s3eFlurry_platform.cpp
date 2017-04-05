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
 * wp8-specific implementation of the s3eFlurry extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include <crtdbg.h>

#define _CRT_PERROR_DEFINED
#include <wrl/client.h>
#include <wrl/module.h>
#undef _CRT_PERROR_DEFINED

#include "s3eFlurry_internal.h"

#include "s3eFlurry_shim.h"

#include <vector>

using namespace s3eFlurryExtension;

s3eResult s3eFlurryInit_platform()
{
    if (s3eFlurryShim::GetInterface() == nullptr)
    {
        return S3E_RESULT_ERROR;
    }

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void s3eFlurryTerminate_platform()
{
    // Add any platform-specific termination code here
}

static Platform::String^ TranslateString(const char* str)
{
    int size = strlen(str);
    if (size <= 0)
        return nullptr;

    std::vector<wchar_t> msg(size + 1);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &msg[0], size);
    return ref new Platform::String(&msg[0]);

}

void s3eFlurryStartSession_platform(const char* apiKey)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    Platform::String^ str = TranslateString(apiKey);
    if (str == nullptr)
        return;
    
    flurry->s3eFlurryStartSession_managed(str);
}

void s3eFlurryEndSession_platform()
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    flurry->s3eFlurryEndSession_managed();
}

void s3eFlurryLogEvent_platform(const char* eventName)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    Platform::String^ str = TranslateString(eventName);
    if (str == nullptr)
        return;
    
    flurry->s3eFlurryLogEvent_managed(str);
}

void s3eFlurryLogEventTimed_platform(const char* eventName)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    Platform::String^ str = TranslateString(eventName);
    if (str == nullptr)
        return;
    
    flurry->s3eFlurryLogEventTimed_managed(str);
}

void s3eFlurryLogEventParams_platform(const char* eventName, const char* eventParams)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    Platform::String^ str1 = TranslateString(eventName);
    Platform::String^ str2 = TranslateString(eventParams);
    if (str1 == nullptr || str2 == nullptr)
        return;
    
    flurry->s3eFlurryLogEventParams_managed(str1, str2);
}

void s3eFlurryLogEventParamsTimed_platform(const char* eventName, const char* eventParams)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    Platform::String^ str1 = TranslateString(eventName);
    Platform::String^ str2 = TranslateString(eventParams);
    if (str1 == nullptr || str2 == nullptr)
        return;
    
    flurry->s3eFlurryLogEventParamsTimed_managed(str1, str2);
}

void s3eFlurryEndTimedEvent_platform(const char* eventName, const char* eventParams)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    Platform::String^ str1 = TranslateString(eventName);
    Platform::String^ str2 = TranslateString(eventParams);
    if (str1 == nullptr || str2 == nullptr)
        return;
    
    flurry->s3eFlurryEndTimedEvent_managed(str1, str2);
}

void s3eFlurryLogError_platform(const char* errorName, const char* message)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    Platform::String^ str1 = TranslateString(errorName);
    Platform::String^ str2 = TranslateString(message);
    if (str1 == nullptr || str2 == nullptr)
        return;
    
    flurry->s3eFlurryLogError_managed(str1, str2);
}

void s3eFlurrySetUserID_platform(const char* userId)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    Platform::String^ str1 = TranslateString(userId);
    if (str1 == nullptr)
        return;
    
    flurry->s3eFlurrySetUserID_managed(str1);
}

void s3eFlurrySetAge_platform(int32 age)
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    flurry->s3eFlurrySetAge_managed(age);
}

void s3eFlurryCountPageView_platform()
{
    IManagedS3EFlurryAPI^ flurry = s3eFlurryShim::GetInterface();
    if (!flurry)
        return;
    
    flurry->s3eFlurryCountPageView_managed();
}

void s3eFlurrySetSessionReportsOnCloseEnabled_platform(s3eBool sendSessionReportsOnClose)
{
    // not implemented
}

void s3eFlurrySetSessionReportsOnPauseEnabled_platform(s3eBool sendSessionReportsOnPause)
{
    // not implemented
}
