/*
 * Internal header for the s3eKiip extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef S3EKIIP_INTERNAL_H
#define S3EKIIP_INTERNAL_H

#include "s3eTypes.h"
#include "s3eKiip.h"
#include "s3eKiip_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult s3eKiipInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult s3eKiipInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void s3eKiipTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void s3eKiipTerminate_platform();
s3eResult s3eKiipInitWithKeys_platform(const char* appKey, const char* appSecret);

void s3eKiipRegisterCallback_platform(s3eKiipCallbackType callbackID, s3eCallback callbackFn, void* userData);

s3eResult s3eKiipSaveMoment_platform(const char* momentID);

s3eResult s3eKiipSaveMomentWithValue_platform(const char* momentID, double* value);

void s3eKiipSetAutorotate_platform(bool shouldAutoRotate);

bool s3eKiipGetAutorotate_platform();

void s3eKiipSetInterfaceOrientation_platform(s3eUIInterfaceOrientation orientation);

s3eUIInterfaceOrientation s3eKiipGetInterfaceOrientation_platform();

void s3eKiipSetEmailAddress_platform(const char* email);

void s3eKiipSetGender_platform(const char* gender);

void s3eKiipSetBirthday_platform(const char* dob);


#endif /* !S3EKIIP_INTERNAL_H */