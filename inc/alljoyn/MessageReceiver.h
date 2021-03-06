#ifndef _ALLJOYN_MESSAGERECEIVER_H
#define _ALLJOYN_MESSAGERECEIVER_H

#include <qcc/platform.h>

#include <alljoyn/Message.h>

/**
 * @file
 * MessageReceiver is a base class implemented by any class
 * which wishes to receive AllJoyn messages
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

namespace ajn {

/**
 * %MessageReceiver is a pure-virtual base class that is implemented by any
 * class that wishes to receive AllJoyn messages from the AllJoyn library.
 *
 * Received messages can be either signals, method_replies or errors.
 */
class MessageReceiver {
  public:
    /** Destructor */
    virtual ~MessageReceiver() { }

    /**
     * MethodHandlers are %MessageReceiver methods which are called by AllJoyn library
     * to forward AllJoyn method_calls to AllJoyn library users.
     *
     * See also these sample file(s): @n
     * basic/basic_service.cc @n
     * secure/DeskTopSharedKSService.cc @n
     * simple/android/service/jni/Service_jni.cpp @n
     * windows/PhotoChat/AllJoynBusLib/AllJoynConnection.cpp @n
     * windows/Service/Service.cpp @n
     *
     * For Windows 8 see also these sample file(s): @n
     * cpp/AllJoynStreaming/src/MediaSource.cc @n
     * cpp/Basic/Basic_Service/BasicService/AllJoynObjects.cpp @n
     * cpp/Secure/Secure/AllJoynObjects.cpp @n
     * csharp/Basic/Basic_Service/BasicService/Common/BasicServiceBusObject.cs @n
     * csharp/BusStress/BusStress/Common/ServiceBusObject.cs @n
     * csharp/Secure/Secure/Common/SecureBusObject.cs @n
     * javascript/Basic/Basic_Service/BasicService/js/AlljoynObjects.js @n
     * javascript/Basic/Basic_Service/BasicService/js/script1.js @n
     * javascript/Secure/Secure/js/Service.js @n
     *
     * @param member    Method interface member entry.
     * @param message   The received method call message.
     */
    typedef void (MessageReceiver::* MethodHandler)(const InterfaceDescription::Member* member, Message& message);

    /**
     * ReplyHandlers are %MessageReceiver methods which are called by AllJoyn library
     * to forward AllJoyn method_reply and error responses to AllJoyn library users.
     *
     * See also these sample file(s): @n
     * windows/Service/Service.cpp @n
     *
     * @param message   The received message.
     * @param context   User-defined context passed to MethodCall and returned upon reply.
     */
    typedef void (MessageReceiver::* ReplyHandler)(Message& message, void* context);

    /**
     * SignalHandlers are %MessageReceiver methods which are called by AllJoyn library
     * to forward AllJoyn received signals to AllJoyn library users.
     *
     * See also these sample file(s): @n
     * basic/signalConsumer_client.cc @n
     * chat/android/jni/Chat_jni.cpp @n
     * chat/linux/chat.cc @n
     * FileTransfer/FileTransferClient.cc @n
     * windows/chat/ChatLib32/ChatClasses.cpp @n
     * windows/chat/ChatLib32/ChatClasses.h @n
     * windows/PhotoChat/AllJoynBusLib/AllJoynConnection.cpp @n
     * windows/PhotoChat/AllJoynBusLib/AllJoynConnection.h @n
     *
     * For Windows 8 see also these sample file(s): @n
     * cpp/AllJoynStreaming/src/MediaSink.cc @n
     * cpp/Basic/Signal_Consumer_Client/SignalConsumerClient/AllJoynObjects.cpp @n
     * cpp/Basic/Signal_Consumer_Client/SignalConsumerClient/AllJoynObjects.h @n
     * cpp/Chat/Chat/AllJoynObjects.cpp @n
     * cpp/Chat/Chat/AllJoynObjects.h @n
     * csharp/Basic/Signal_Consumer_Client/SignalConsumerClient/Common/SignalConsumerBusListener.cs @n
     * csharp/chat/chat/Common/ChatSessionObject.cs @n
     * csharp/FileTransfer/Client/Common/FileTransferBusObject.cs @n
     * csharp/Sessions/Sessions/Common/MyBusObject.cs @n
     * javascript/Basic/Signal_Consumer_Client/SignalConsumerClient/js/AlljoynObjects.js @n
     * javascript/chat/chat/js/alljoyn.js @n
     *
     * @param member    Method or signal interface member entry.
     * @param srcPath   Object path of signal emitter.
     * @param message   The received message.
     */
    typedef void (MessageReceiver::* SignalHandler)(const InterfaceDescription::Member* member, const char* srcPath, Message& message);

};

}

#endif
