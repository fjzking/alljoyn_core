/**
 * @file
 * AllJoyn Permission database class
 */

/******************************************************************************
 * Copyright 2010-2011, Qualcomm Innovation Center, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 ******************************************************************************/
#ifndef _ALLJOYN_PERMISSION_DB_H
#define _ALLJOYN_PERMISSION_DB_H

#include "BusEndpoint.h"

using namespace std;
using namespace qcc;

namespace ajn {

class PermissionDB {
  public:
    /**
     * Check whether the endpoint is allowed to use Bluetooth
     * @Param endponit The endpoint to be checked
     * @Return true if allowed
     */
    bool IsBluetoothAllowed(BusEndpoint& endpoint);

    /**
     * Check whether the endpoint is allowed to use WIFI
     * @Param endpoint The endpoint to be checked
     * @Return true if allowed
     */
    bool IsWifiAllowed(BusEndpoint& endpoint);

    /**
     * Check whether the endpoint owns the required permissions
     * @Param uid      The user id of the endpoint to be verified
     * @Param permsReq The list of permissions to be verified
     * @Return true if the endpoint passes the verification
     */
    bool VerifyPeerPermissions(const uint32_t uid, const std::set<qcc::String>& permsReq);

    /**
     * Remove the permission information cache of an enpoint before it exits.
     * @Param endponit The endpoint that will exits
     * @Return ER_OK if successful
     */
    QStatus RemovePermissionCache(BusEndpoint& endpoint);

    /**
     * Add an alias ID to a UnixEndpoint User ID
     * @Param origUID   The unique User ID
     * @Param aliasUID  The alias User ID
     * @Return ER_OK if successfully
     */
    QStatus AddAliasUnixUser(uint32_t origUID, uint32_t aliasUID);

  private:
    /**
     * Check whether the uid owns the required permissions on Android
     * @Param uid        The user Id of an Android app
     * @Param permsReq   The required permissions
     * @Return true if the uid meets the permission requirement
     */
    bool VerifyPermsOnAndroid(const uint32_t uid, const std::set<qcc::String>& permsReq);

    /**
     * Get the unique user ID of an alias user ID
     * @Param userID      The user ID of the endpoint
     * @Return the unique user ID if it exists otherwise keep unchanged
     */
    uint32_t UniqueUserID(uint32_t userID);

    qcc::Mutex permissionDbLock;
    std::map<uint32_t, std::set<qcc::String> > uidPermsMap;          /**< cache the permissions owned by an endpoint identified by user id */
    std::map<uint32_t, uint32_t> uidAliasMap;                        /**< map of alias user id to the unique user id. */
};

}
#endif
