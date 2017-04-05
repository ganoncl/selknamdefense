/*
Generic implementation of the s3eKiip extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "s3eKiip_internal.h"
s3eResult s3eKiipInit()
{
    //Add any generic initialisation code here
    return s3eKiipInit_platform();
}

void s3eKiipTerminate()
{
    //Add any generic termination code here
    s3eKiipTerminate_platform();
}

s3eResult s3eKiipInitWithKeys(const char* appKey, const char* appSecret)
{
	return s3eKiipInitWithKeys_platform(appKey, appSecret);
}

void s3eKiipRegisterCallback(s3eKiipCallbackType callbackID, s3eCallback callbackFn, void* userData)
{
	s3eKiipRegisterCallback_platform(callbackID, callbackFn, userData);
}

s3eResult s3eKiipSaveMoment(const char* momentID)
{
	return s3eKiipSaveMoment_platform(momentID);
}

s3eResult s3eKiipSaveMomentWithValue(const char* momentID, double* value)
{
	return s3eKiipSaveMomentWithValue_platform(momentID, value);
}

void s3eKiipSetAutorotate(bool shouldAutoRotate)
{
	s3eKiipSetAutorotate_platform(shouldAutoRotate);
}

bool s3eKiipGetAutorotate()
{
	return s3eKiipGetAutorotate_platform();
}

void s3eKiipSetInterfaceOrientation(s3eUIInterfaceOrientation orientation)
{
	s3eKiipSetInterfaceOrientation_platform(orientation);
}

s3eUIInterfaceOrientation s3eKiipGetInterfaceOrientation()
{
	return s3eKiipGetInterfaceOrientation_platform();
}

void s3eKiipSetEmailAddress(const char* email)
{
	s3eKiipSetEmailAddress_platform(email);
}

void s3eKiipSetGender(const char* gender)
{
	s3eKiipSetGender_platform(gender);
}

void s3eKiipSetBirthday(const char* dob)
{
	s3eKiipSetBirthday_platform(dob);
}
