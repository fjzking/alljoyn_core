#ifndef _ALLJOYN_ALLJOYNSTD_H
#define _ALLJOYN_ALLJOYNSTD_H
/**
 * @file
 * This file provides definitions for AllJoyn interfaces
 */

/******************************************************************************
 * Copyright 2009-2011, Qualcomm Innovation Center, Inc.
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

#include <qcc/platform.h>

#include <alljoyn/InterfaceDescription.h>

/*!
 * \def QCC_MODULE
 * Internal usage
 */
#define QCC_MODULE  "ALLJOYN"

/** Daemon-to-daemon protocol version number */
#define ALLJOYN_PROTOCOL_VERSION  2

namespace ajn {


namespace org {
namespace alljoyn {
/** Interface definitions for org.alljoyn.Bus */
namespace Bus {

extern const char* ErrorName;                     /**< Standard AllJoyn error name */
extern const char* ObjectPath;                    /**< Object path */
extern const char* InterfaceName;                 /**< Interface name */
extern const char* WellKnownName;                 /**< Well known bus name */

/** Interface definitions for org.alljoyn.Bus.Peer.* */
namespace Peer {
extern const char* ObjectPath;                         /**< Object path */
namespace HeaderCompression {
extern const char* InterfaceName;                      /**<Interface name */
}
namespace Authentication {
extern const char* InterfaceName;                      /**<Interface name */
}
namespace Session {
extern const char* InterfaceName;                      /**<Interface name */
}
}
}

/** Interface definitions for org.alljoyn.Daemon */
namespace Daemon {

extern const char* ErrorName;                     /**< Standard AllJoyn error name */
extern const char* ObjectPath;                    /**< Object path */
extern const char* InterfaceName;                 /**< Interface name */
extern const char* WellKnownName;                 /**< Well known bus name */

namespace Debug {
extern const char* ObjectPath;                    /**< Object path */
extern const char* InterfaceName;                 /**< Interface name */
}
}

QStatus CreateInterfaces(BusAttachment& bus);          /**< Create the org.alljoyn.* interfaces and sub-interfaces */
}
}

/**
 * @anchor BindSessionPortReplyAnchor
 * @name org.alljoyn.Bus.BindSessionPort
 *  Interface: org.alljoyn.Bus
 *  Method: UINT32 disposition SessionPort outPort BindSessionPort(SessionPort inPort, bool isMultipoint, SessionOpts opts)
 *
 * Create a named session for other bus nodes to join.
 *
 * In params:
 *  inPort       - Session Port number to bind to or SESSION_PORT_ANY to have daemon allocate an available port number.
 *  isMultipoint - true iff session supports more than two participants.
 *  opts         - Session options
 *
 * Out params:
 *  disposition  - BindSessionPort return value (see below).
 *  outPort      - Bound Session port. (equal to inPort if inPort != SESSION_PORT_ANY)
 */
// @{
/* org.alljoyn.Bus.BindSessionPort */
#define ALLJOYN_BINDSESSIONPORT_REPLY_SUCCESS         1   /**< BindSessionPort reply: Success */
#define ALLJOYN_BINDSESSIONPORT_REPLY_ALREADY_EXISTS  2   /**< BindSessionPort reply: SessionPort already exists */
#define ALLJOYN_BINDSESSIONPORT_REPLY_FAILED          3   /**< BindSessionPort reply: Failed */
#define ALLJOYN_BINDSESSIONPORT_REPLY_INVALID_OPTS    4   /**< BindSessionPort reply: Invalid SessionOpts */
// @}

/**
 * @anchor UnbindSessionPortReplyAnchor
 * @name org.alljoyn.Bus.UnbindSessionPort
 *  Interface: org.alljoyn.Bus
 *  Method: UINT32 disposition UnbindSessionPort(SessionPort port)
 *
 * Cancel a session port binding.
 *
 * In params:
 *  inPort       - Session Port number to unbind.
 *
 * Out params:
 *  disposition  - UnbindSessionPort return value (see below).
 */
// @{
/* org.alljoyn.Bus.UnbindSessionPort */
#define ALLJOYN_UNBINDSESSIONPORT_REPLY_SUCCESS   1   /**< UnbindSessionPort reply: Success */
#define ALLJOYN_UNBINDSESSIONPORT_REPLY_BAD_PORT  2   /**< UnbindSessionPort reply: Unknown session port */
#define ALLJOYN_UNBINDSESSIONPORT_REPLY_FAILED    3   /**< UnbindSessionPort reply: Failed */
// @}

/**
 * @anchor JoinSessionReplyAnchor
 * @name org.alljoyn.Bus.JoinSession
 *  Interface: org.alljoyn.Bus
 *  Method: UINT32 status, UINT32 sessionId, SessionOpts outOpts JoinSession(String sessionHost, SessionPort sessionPort, SessionOptions inOpts)
 *
 * Send a JoinSession request to a remote bus name.
 *
 * In params:
 *  sessionHost   - Bus name of endpoint that is hosting the session.
 *  sessionPort   - Session port number bound by sessionHost.
 *  inOpts - Desired session options.
 *
 * Out params:
 *  status      - JoinSession return value (see below).
 *  sessionId   - Session id.
 *  outOpts     - Acutal (final) session options.
 */
// @{
/* org.alljoyn.Bus.JoinSession */
#define ALLJOYN_JOINSESSION_REPLY_SUCCESS              1   /**< JoinSession reply: Success */
#define ALLJOYN_JOINSESSION_REPLY_NO_SESSION           2   /**< JoinSession reply: Session with given name does not exist */
#define ALLJOYN_JOINSESSION_REPLY_UNREACHABLE          3   /**< JoinSession reply: Failed to find suitable transport */
#define ALLJOYN_JOINSESSION_REPLY_CONNECT_FAILED       4   /**< JoinSession reply: Connect to advertised address */
#define ALLJOYN_JOINSESSION_REPLY_REJECTED             5   /**< JoinSession reply: The session creator rejected the join req */
#define ALLJOYN_JOINSESSION_REPLY_BAD_SESSION_OPTS     6   /**< JoinSession reply: Failed due to session option incompatibilities */
#define ALLJOYN_JOINSESSION_REPLY_ALREADY_JOINED       7   /**< JoinSession reply: Caller has already joined this session */
#define ALLJOYN_JOINSESSION_REPLY_FAILED              10   /**< JoinSession reply: Failed for unknown reason */
// @}

/**
 * @anchor LeaveSessionReplyAnchor
 * @name org.alljoyn.Bus.LeaveSession
 *  Interface: org.alljoyn.Bus
 *  Method: void LeaveSession(UINT32 sessionId)
 *
 * Leave a previously joined session.
 *
 * In params:
 *  sessionId    - Id of session to leave.
 */
#define ALLJOYN_LEAVESESSION_REPLY_SUCCESS            1   /**< LeaveSession reply: Success */
#define ALLJOYN_LEAVESESSION_REPLY_NO_SESSION         2   /**< LeaveSession reply: Session with given name does not exist */
#define ALLJOYN_LEAVESESSION_REPLY_FAILED             3   /**< LeaveSession reply: Failed for unspecified reason */

/**
 * @name org.alljoyn.Bus.AdvertiseName
 *  Interface: org.alljoyn.Bus
 *  Method: UINT32 AdvertiseName(String wellKnownName)
 *
 *  Request the local daemon to advertise the already obtained well-known attachment name to other
 *  AllJoyn instances that might be interested in connecting to the named service.
 *
 *  wellKnownName = Well-known name of the attachment that wishes to be advertised to remote AllJoyn instances.
 *
 *  Returns a status code (see below) indicating success or failure.
 */
// @{
/* org.alljoyn.Bus.AdvertiseName */
#define ALLJOYN_ADVERTISENAME_REPLY_SUCCESS               1   /**< AdvertiseName reply: Success */
#define ALLJOYN_ADVERTISENAME_REPLY_ALREADY_ADVERTISING   2   /**< AdvertiseName reply: This endpoint has already requested advertising this name */
#define ALLJOYN_ADVERTISENAME_REPLY_FAILED                3   /**< AdvertiseName reply: Advertise failed */
// @}

/**
 * @name org.alljoyn.Bus.CancelAdvertise
 *  Interface: org.alljoyn.Bus
 *  Method: CancelAdvertiseName(String wellKnownName)
 *
 *  wellKnownName = Well-known name of the attachment that should end advertising.
 *
 *  Request the local daemon to stop advertising the well-known attachment name to other
 *  AllJoyn instances. The well-known name must have previously been advertised via a call
 *  to org.alljoyn.Bus.Advertise().
 *
 *  Returns a status code (see below) indicating success or failure.
 */
// @{
/* org.alljoyn.Bus.CancelAdvertise */
#define ALLJOYN_CANCELADVERTISENAME_REPLY_SUCCESS         1   /**< CancelAdvertiseName reply: Success */
#define ALLJOYN_CANCELADVERTISENAME_REPLY_FAILED          2   /**< CancelAdvertiseName reply: Advertise failed */
// @}

/**
 * @name org.alljoyn.Bus.FindAdvertisedName
 *  Interface: org.alljoyn.Bus
 *  Method: FindAdvertisedName(String wellKnownNamePrefix)
 *
 *  wellKnownNamePrefix = Well-known name prefix of the attachment that client is interested in.
 *
 *  Register interest in a well-known attachment name being advertised by a remote AllJoyn instance.
 *  When the local AllJoyn daemon receives such an advertisement it will send an org.alljoyn.Bus.FoundAdvertisedName
 *  signal. This attachment can then choose to ignore the advertisement or to connect to the remote Bus by
 *  calling org.alljoyn.Bus.Connect().
 *
 *  Returns a status code (see below) indicating success or failure.
 */
// @{
/* org.alljoyn.Bus.FindAdvertisedName */
#define ALLJOYN_FINDADVERTISEDNAME_REPLY_SUCCESS                1   /**< FindAdvertisedName reply: Success */
#define ALLJOYN_FINDADVERTISEDNAME_REPLY_ALREADY_DISCOVERING    2   /**< FindAdvertisedName reply: This enpoint has already requested discover for name */
#define ALLJOYN_FINDADVERTISEDNAME_REPLY_FAILED                 3   /**< FindAdvertisedName reply: Failed */
// @}

/**
 * @name org.alljoyn.Bus.CancelFindAdvertisedName
 *  Interface: org.alljoyn.Bus
 *  Method: CancelFindAdvertisedName(String wellKnownName)
 *
 *  wellKnownName = Well-known name of the attachment that client is no longer interested in.
 *
 *  Cancel interest in a well-known attachment name that was previously included in a call
 *  to org.alljoyn.Bus.FindAdvertisedName().
 *
 *  Returns a status code (see below) indicating success or failure.
 */
// @{
/* org.alljoyn.Bus.CancelDiscover */
#define ALLJOYN_CANCELFINDADVERTISEDNAME_REPLY_SUCCESS          1   /**< CancelFindAdvertisedName reply: Success */
#define ALLJOYN_CANCELFINDADVERTISEDNAME_REPLY_FAILED           2   /**< CancelFindAdvertisedName reply: Failed */
// @}

/**
 * @name org.alljoyn.Bus.GetSessionFd
 *  Interface: org.alljoyn.Bus
 *  Method: Handle GetSessionFd(uint32_t sessionId)
 *
 *  sessionId - Existing sessionId for a streaming (non-message based) session.
 *
 *  Get the socket descriptor for an existing session that was created or joined with
 *  traffic type equal to RAW_UNRELIABLE or RAW_RELIABLE.
 *
 *  Returns the socket descriptor request or an error response
 */
}

#undef QCC_MODULE

#endif
