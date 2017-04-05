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
C++ Shim implementation of the s3eFlurry extension's.

Not intended for modifications

These functions are called by C# to access native interface and from C++ to
access managed interface.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */

namespace s3eFlurryExtension
{

// This is the interface which native code can use to talk to managed code.
[Windows::Foundation::Metadata::WebHostHidden]
public interface class IManagedS3EFlurryAPI : public s3e_native::IManagedS3EAPI
{
    // We shouldn't populate these defacto.  We should only add functions here as and when
    // necessary.
    void s3eFlurryStartSession_managed(Platform::String^ apiKey);
    void s3eFlurryEndSession_managed();
    void s3eFlurryLogEvent_managed(Platform::String^ eventName);
    void s3eFlurryLogEventTimed_managed(Platform::String^ eventName);
    void s3eFlurryLogEventParams_managed(Platform::String^ eventName, Platform::String^ eventParams);
    void s3eFlurryLogEventParamsTimed_managed(Platform::String^ eventName, Platform::String^ eventParams);
    void s3eFlurryEndTimedEvent_managed(Platform::String^ eventName, Platform::String^ eventParams);
    void s3eFlurryLogError_managed(Platform::String^ errorName, Platform::String^ message);
    void s3eFlurrySetUserID_managed(Platform::String^ userId);
    void s3eFlurrySetAge_managed(int age);
    void s3eFlurryCountPageView_managed();
    void s3eFlurrySetSessionReportsOnCloseEnabled_managed(bool sendSessionReportsOnClose);
    void s3eFlurrySetSessionReportsOnPauseEnabled_managed(bool sendSessionReportsOnPause);
};

// This is the class which managed code can use to talk to native code.
[Windows::Foundation::Metadata::WebHostHidden]
public ref class s3eFlurryNativeInterface sealed : s3e_native::INativeS3EAPI
{
    // We shouldn't populate these defacto.  We should only add functions here as and when
    // necessary.
public:
    s3eFlurryNativeInterface() {}
};


[Windows::Foundation::Metadata::WebHostHidden]
public ref class s3eFlurryShim sealed
{
public:

    s3eFlurryShim();

    // We use two stage construction to avoid the use of exceptions.  We
    // don't want to ever register an API more than once (as it's a static
    // below).  If we did this initialization in the ctr then we'd either
    // have to fail silently or throw an exception.
    bool Init(IManagedS3EFlurryAPI^ managedAPI);

    // Unregisters the static interface.
    bool Terminate();

    // Need static getters as we can't have public members in the class.
    static IManagedS3EFlurryAPI^ GetInterface();
private:

    ~s3eFlurryShim();

    // This is the unavoidable point where we need a static.  There's
    // no choice as we're converting to a C-API.
    static IManagedS3EFlurryAPI^ s_ManagedInterface;
};

}
