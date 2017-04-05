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
#include "s3eFlurry_shim.h"

using namespace s3eFlurryExtension;


IManagedS3EFlurryAPI^ s3eFlurryShim::s_ManagedInterface = nullptr;

s3eFlurryShim::s3eFlurryShim()
{
}

s3eFlurryShim::~s3eFlurryShim()
{
    Terminate();
}

bool s3eFlurryShim::Init(IManagedS3EFlurryAPI^ managedAPI)
{
    // It's an error to do this twice.
    if (s_ManagedInterface)
        return false;

    s_ManagedInterface = managedAPI;
    return true;
}

bool s3eFlurryShim::Terminate()
{
    // It's an error to do this twice.
    if (!s_ManagedInterface)
        return false;

    s_ManagedInterface = nullptr;
    return true;
}

IManagedS3EFlurryAPI^ s3eFlurryShim::GetInterface()
{
    return s_ManagedInterface;
}

