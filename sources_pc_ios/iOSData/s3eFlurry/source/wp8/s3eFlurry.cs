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
C# implementation of the s3eFlurry extension.

Add wp8-specific functionality here.

These functions are called via Shim class from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
using Microsoft.Phone.Controls;

using s3e_native;
using s3eFlurryExtension;

using System.Collections.Generic;
using FlurryWP8SDK;

namespace s3eFlurryManaged
{
    public class s3eFlurryManaged : IManagedS3EFlurryAPI, IManagedS3EEDKAPI
    {

        public bool ExtRegister(IS3EAPIManager apiManager, object mainPage)
        {
            try
            {
                // Keep a reference to the API Manager in order to call other
                // APIs.
                m_APIManager = apiManager;
                m_MainPage = mainPage as PhoneApplicationPage;

                // Add the managed API to the API Manager
                if (!m_APIManager.RegisterManagedAPI("s3eFlurry", this))
                    throw new System.Exception("Can't register Managed API");

                // Add the native API to the API manager.  Note that we don't own the native
                // interface, the API Manager does.  We want the two notions of managed and
                // native interface to be separate as there may be cases where we only want
                // one not the other.  It's only a matter of convenience that we create both
                // APIs in this ctr
                m_Native = new s3eFlurryNativeInterface();
                if (!m_APIManager.RegisterNativeAPI("s3eFlurry", m_Native))
                    throw new System.Exception("Can't register Native API");

                // Create a Shim so we can pass the Managed interface down to native
                m_Shim = new s3eFlurryShim();

                // Pass the managed interface down
                m_Shim.Init(this);
            }
            catch (System.Exception e)
            {
                m_APIManager = null;
                m_Shim = null;
                m_Native = null;
                System.Windows.MessageBox.Show("Failed to register s3eFlurry : " + e.Message);
                return false;
            }

            return true;
        }
        IS3EAPIManager m_APIManager = null;
        PhoneApplicationPage m_MainPage = null;
        s3eFlurryShim m_Shim = null;
        s3eFlurryNativeInterface m_Native = null;

        // managed platform functionality
        private List<FlurryWP8SDK.Models.Parameter> ParamsToMap(string str)
        {
            List<FlurryWP8SDK.Models.Parameter> res = new List<FlurryWP8SDK.Models.Parameter>();
            List<string> lst = new List<string>(str.Split('|'));

            if (0 != lst.Count % 2)
                lst.Add("");

            for (int i = 0; i < lst.Count - 1; i += 2)
                res.Add(new FlurryWP8SDK.Models.Parameter(lst[i], lst[i + 1]));

            return res;
        }

        public void s3eFlurryStartSession_managed(string apiKey)
        {
            FlurryWP8SDK.Api.StartSession(apiKey);
        }

        public void s3eFlurryEndSession_managed()
        {
            FlurryWP8SDK.Api.EndSession();
        }

        public void s3eFlurryLogEvent_managed(string eventName)
        {
            FlurryWP8SDK.Api.LogEvent(eventName);
        }

        public void s3eFlurryLogEventTimed_managed(string eventName)
        {
            FlurryWP8SDK.Api.LogEvent(eventName, true);
        }

        public void s3eFlurryLogEventParams_managed(string eventName, string eventParams)
        {
            FlurryWP8SDK.Api.LogEvent(eventName, ParamsToMap(eventParams));
        }

        public void s3eFlurryLogEventParamsTimed_managed(string eventName, string eventParams)
        {
            FlurryWP8SDK.Api.LogEvent(eventName, ParamsToMap(eventParams), true);
        }

        public void s3eFlurryEndTimedEvent_managed(string eventName, string eventParams)
        {
            FlurryWP8SDK.Api.EndTimedEvent(eventName, ParamsToMap(eventParams));
        }

        public void s3eFlurryLogError_managed(string errorName, string message)
        {
            FlurryWP8SDK.Api.LogError(errorName, new System.Exception(message));
        }

        public void s3eFlurrySetUserID_managed(string userId)
        {
            FlurryWP8SDK.Api.SetUserId(userId);
        }

        public void s3eFlurrySetAge_managed(int age)
        {
            FlurryWP8SDK.Api.SetAge(age);
        }

        public void s3eFlurryCountPageView_managed()
        {
            FlurryWP8SDK.Api.LogPageView();
        }

        public void s3eFlurrySetSessionReportsOnCloseEnabled_managed(bool sendSessionReportsOnClose)
        {
            // not implemented
        }

        public void s3eFlurrySetSessionReportsOnPauseEnabled_managed(bool sendSessionReportsOnPause)
        {
            // not implemented
        }
    }
}
