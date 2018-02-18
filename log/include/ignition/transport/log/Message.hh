/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef IGNITION_TRANSPORT_LOG_MESSAGE_HH_
#define IGNITION_TRANSPORT_LOG_MESSAGE_HH_

#include <chrono>
#include <memory>
#include <string>

#include <ignition/transport/log/Export.hh>

namespace ignition
{
  namespace transport
  {
    namespace log
    {
      /// \brief Forward Declarations
      class MessagePrivate;

      /// \brief Represents a message in a bag file
      class IGNITION_TRANSPORT_LOG_VISIBLE Message
      {
        /// \brief Default constructor
        public: Message();

        /// \brief Construct with data
        /// \param[in] _timeRecv time the message was received
        /// \param[in] _data the serialized message
        /// \param[in] _dataLen number of bytes in _data
        /// \param[in] _type the name of the message type
        /// \param[in] _typeLen the length of _type
        /// \param[in] _topic the name of the topic the message was published to
        /// \param[in] _topicLen the length of _topic
        public: Message(
            const std::chrono::nanoseconds &_timeRecv,
            const void *_data, std::size_t _dataLen,
            const char *_type, std::size_t _typeLen,
            const char *_topic, std::size_t _topicLen);

        /// \brief Destructor
        public: ~Message();

        /// \brief Get the message data
        /// \return The raw data for this message
        public: std::string Data() const;

        /// \brief Get the message type as a string
        /// \return The message type name
        public: std::string Type() const;

        /// \brief Get the Topic name as a string
        /// \return The topic for the message
        public: std::string Topic() const;

        /// \brief Return the time the message was received
        /// \return The time the message was received
        public: const std::chrono::nanoseconds &TimeReceived() const;

        /// \brief Private Implementation Pointer
        private: std::unique_ptr<MessagePrivate> dataPtr;
      };
    }
  }
}
#endif
