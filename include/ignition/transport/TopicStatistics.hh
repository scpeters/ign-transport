/*
 * Copyright (C) 2020 Open Source Robotics Foundation
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
#ifndef IGN_TRANSPORT_TOPICSTATISTICS_HH_
#define IGN_TRANSPORT_TOPICSTATISTICS_HH_

#include <cmath>
#include <sstream>

namespace ignition
{
  namespace transport
  {
    /// \brief Metadata for a publication.
    struct PublicationMetadata
    {
      uint64_t stamp = 0;
      uint64_t seq = 0;
    };

    /// \brief Computes the rolling average, min, max, and standard
    /// deviation for a set of samples.
    class Statistics
    {
      /// \brief Update with a new sample.
      /// \param[in] _stat New statistic sample.
      public: void Update(double _stat)
      {
        // Increase the sample count.
        this->count++;

        // Update the rolling average
        const double currentAvg = this->average;
        this->average = currentAvg +
          (_stat - currentAvg) / this->count;

        // Store the min and max.
        this->min = std::min(this->min, _stat);
        this->max = std::max(this->max, _stat);

        // Update the variance, used to calculate the standard deviation,
        // using Welford's algorithm described at
        // https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford%27s_online_algorithm
        this->sumSquareMeanDist += (_stat - currentAvg) *
          (_stat - this->average);
      }

      /// \brief Get the average value.
      /// \return the average value.
      public: double Avg() const
      {
        return this->average;
      }

      /// \brief Get the standard deviation.
      /// \return The standard deviation.
      public: double StdDev() const
      {
        return std::sqrt(this->sumSquareMeanDist / this->count);
      }

      /// \brief Get the minimum sample value.
      /// \return The minimum sample value.
      public: double Min() const
      {
        return this->min;
      }

      /// \brief Get the maximum sample value.
      /// \return The maximum sample value.
      public: double Max() const
      {
        return this->max;
      }

      /// \brief Count of the samples.
      private: uint64_t count = 0;

      /// \brief Average value.
      private: double average = 0;

      /// \brief Sum of the squared mean distance between samples. This is
      /// used to calculate the standard deviation.
      private: double sumSquareMeanDist = 0;

      /// \brief Minimum sample.
      private: double min = std::numeric_limits<double>::max();

      /// \brief Maximum sample.
      private: double max = std::numeric_limits<double>::min();
    };

    /// \brief Encapsulates statistics for a single topic. The set of
    /// statistics include:
    ///
    /// 1. Number of dropped messages.
    /// 2. Publication statistics: The publication hz rate, standard
    ///    deviation between publications, min time between publications, and
    ///    max time between publications.
    /// 3. Receive statistics: The receive hz rate, standard
    ///    deviation between receiving messages, min time between receiving
    ///    messages, and max time between receiving messages.
    ///
    /// Publication statistics utilize time stamps generated by the
    /// publisher. Receive statistics use time stamps generated by the
    /// subscriber.
    class TopicStatistics
    {
      /// \brief Update the topic statistics.
      /// \param[in] _sender Address of the sender.
      /// \param[in] _meta Metadata for the publication.
      public: void Update(const std::string &_sender,
                          const PublicationMetadata &_meta)
      {
        // Current wall time
        uint64_t now =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::steady_clock::now().time_since_epoch()).count();

        if (this->prevPublicationStamp != 0)
        {
          this->publication.Update(_meta.stamp - this->prevPublicationStamp);
          this->receive.Update(now - this->prevReceiveStamp);

          if (this->seq[_sender] + 1 != _meta.seq)
          {
            this->droppedMsgCount++;
          }
        }

        this->prevPublicationStamp = _meta.stamp;
        this->prevReceiveStamp = now;

        this->seq[_sender] = _meta.seq;
      }

      /// \brief Generation a YAML string with the set of statistics.
      /// \return A YAML formatted string with the statistics.
      public: std::string YamlString() const
      {
        std::ostringstream stream;
        stream << "dropped_message_count: " << this->droppedMsgCount << "\n";
        stream << "publication_statistics:\n";
        stream << "  avg_hz: " << 1.0 / (this->publication.Avg() * 1e-3) << "\n"
               << "  std_deviation: " << this->publication.StdDev() << "\n"
               << "  min: " << this->publication.Min() << "\n"
               << "  max: " << this->publication.Max() << "\n";
        stream << "receive_statistics:\n";
        stream << "  avg_hz: " << 1.0 / (this->receive.Avg() * 1e-3) << "\n"
               << "  std_deviation: " << this->receive.StdDev() << "\n"
               << "  min: " << this->receive.Min() << "\n"
               << "  max: " << this->receive.Max() << "\n";
        return stream.str();
      }

      /// \brief Map of address to sequence numbers. This is used to
      /// identify dropped messages.
      private: std::map<std::string, uint64_t> seq;

      /// \brief Statistics for the publisher.
      private: Statistics publication;

      /// \brief Statistics for the subscriber.
      private: Statistics receive;

      /// \brief Total number of dropped messages.
      private: uint64_t droppedMsgCount = 0;

      /// \brief Previous publication time stamp.
      private: uint64_t prevPublicationStamp = 0;

      /// \brief Previous received time stamp.
      private: uint64_t prevReceiveStamp = 0;
    };
  }
}
#endif
