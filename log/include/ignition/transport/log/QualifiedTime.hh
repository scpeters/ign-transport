#ifndef IGNITION_TRANSPORT_LOG_QUALIFIEDTIME_HH_
#define IGNITION_TRANSPORT_LOG_QUALIFIEDTIME_HH_

#include <chrono>
#include <memory>

#include <ignition/transport/log/Export.hh>

namespace ignition
{
  namespace transport
  {
    namespace log
    {
      //////////////////////////////////////////////////
      /// \brief Since time is coninuous, it may be difficult to know the exact
      /// time stamp of a desired message. The QualifiedTime class provides a
      /// way to tailor how a time stamp is interpreted by the message query.
      ///
      /// Note that the value of this time object may be interpreted as a
      /// relative time or as an absolute time stamp depending on the context in
      /// which it gets used.
      class IGNITION_TRANSPORT_LOG_VISIBLE QualifiedTime
      {
        /// \brief The Qualifier determines the behavior of how a message is
        /// selected.
        public: enum Qualifier
        {
          /// \brief Get either an exact time match, or the closest time before
          /// the specified time stamp.
          OrClosestBefore = 0,

          /// \brief Get the closest time before the specified time stamp.
          ClosestBefore,

          /// \brief Get either an exact time match, or the closest time after
          /// the specified time stamp.
          OrClosestAfter,

          /// \brief Get the closest time after the specified time stamp.
          ClosestAfter
        };

        /// \brief Convenient typedef for our time representation. Note that any
        /// std::chrono::duration type can be implicitly converted to this type.
        /// E.g. you can pass a
        ///
        /// \code
        /// std::chrono::seconds(value)
        /// std::chrono::milliseconds(value)
        /// std::chrono::minutes(value)
        /// // ... ect ...
        /// \endcode
        ///
        /// into any function that accepts this Time type.
        using Time = std::chrono::nanoseconds;

        /// \brief Construct a qualified time specifier.
        /// \param[in] _time The time stamp that is used as the focal point of
        /// this qualified time.
        /// \param[in] _qualifier The qualifier that determines the exact
        /// interpretation of the _time value.
        public: QualifiedTime(const Time &_time,
                              Qualifier _qualifier = OrClosestBefore);

        /// \brief Default constructor. The time will be treated as
        /// indeterminate. This means that the QualifiedTime object will be
        /// taken to indicate that no time has been specified at all.
        /// \sa IsIndeterminate()
        public: QualifiedTime( /* Indeterminate */ );

        /// \brief Copy constructor.
        /// \param[in] _other Another QualifiedTime
        public: QualifiedTime(const QualifiedTime &_other);

        /// \brief Copy assignment operator.
        /// \param[in] _other Another QualifiedTime
        /// \return Reference to this object
        public: QualifiedTime & operator=(const QualifiedTime &_other);

        /// \brief Default move constructor
        public: QualifiedTime(QualifiedTime &&) = default;

        /// \brief Default move operator
        /// \return Reference to this object
        public: QualifiedTime & operator=(QualifiedTime &&) = default;

        /// \brief Indicates whether this QualifiedTime object is indeterminate.
        ///
        /// When an indeterminate time is used as the end of a range, it implies
        /// that the range should go on endlessly. When used as the beginning of
        /// a range, it means that the range should extend as far into the past
        /// as possible.
        ///
        /// \return true if this QualifiedTime is indeterminate, or false if it
        /// does have a determined time.
        public: bool IsIndeterminate() const;

        /// \brief Get the time stamp for this qualified time, unless the time
        /// is indeterminate.
        /// \return A pointer to the time value that is specified by this
        /// QualifiedTime object, or a nullptr if this QualifiedTime is
        /// indeterminate.
        public: const Time *GetTime() const;

        /// \brief Get the qualifier for this qualified time, unless the time
        /// is indeterminate.
        /// \return A pointer to the qualifier that is specified by this
        /// QualifiedTime object, or a nullptr if this QualifiedTime is
        /// indeterminate.
        public: const Qualifier *GetQualifier() const;

        /// \brief Set the time that this QualifiedTime object represents.
        /// \param[in] _time The time stamp that is used as the focal point of
        /// this qualified time.
        /// \param[in] _qualifier The qualifier that determines the exact
        /// interpretation of the _time value.
        public: void SetTime(const Time &_time,
                             Qualifier _qualifier = OrClosestBefore);

        /// \brief Set this QualifiedTime object to be indeterminate.
        /// \sa IsIndeterminate()
        public: void Clear();

        /// \brief Destructor
        public: ~QualifiedTime();

        /// \internal Implementation class
        private: class Implementation;

        /// \internal PIMPL pointer
        private: std::unique_ptr<Implementation> dataPtr;
      };

      //////////////////////////////////////////////////
      /// \brief The QualifiedTimeRange class provides a pair of qualified times
      /// that represent a range. This is used to specify a desired time range
      /// to the BasicQueryOptions class.
      class IGNITION_TRANSPORT_LOG_VISIBLE QualifiedTimeRange
      {
        /// \brief Construct a time range.
        /// \param[in] _start The beginning of the time range.
        /// \param[in] _finish The end of the time range.
        /// \sa From()
        /// \sa Until()
        /// \sa AllTime()
        public: QualifiedTimeRange(
          const QualifiedTime &_start,
          const QualifiedTime &_finish);

        /// \brief Copy constructor
        /// \param[in] _other Another QualifiedTimeRange
        public: QualifiedTimeRange(const QualifiedTimeRange &_other);

        /// \brief Copy assignment operator
        /// \param[in] _other Another QualifiedTimeRange
        /// \return Reference to this object
        public: QualifiedTimeRange &operator=(const QualifiedTimeRange &_other);

        /// \brief Default move constructor
        public: QualifiedTimeRange(QualifiedTimeRange &&) = default;

        /// \brief Default move assignment
        /// \return Reference to this object
        public: QualifiedTimeRange &operator=(QualifiedTimeRange &&) = default;

        /// \brief Construct a time range that begins at _start and never ends.
        /// \param[in] _start The beginning of the time range.
        /// \return A time range that begins at _start and never ends.
        public: static QualifiedTimeRange From(const QualifiedTime &_start);

        /// \brief Construct a time range that ends at _finish and has no
        /// beginning.
        /// \param[in] _finish The end of the time range.
        /// \return A time range that ends at _finish and has no beginning.
        public: static QualifiedTimeRange Until(const QualifiedTime &_finish);

        /// \brief Construct a time range that has no beginning or end.
        /// \return A time range that has no beginning or end.
        public: static QualifiedTimeRange AllTime();

        /// \brief Get a reference to the start time of this range.
        /// \return A reference to the start time.
        public: const QualifiedTime &GetStart() const;

        /// \brief Get a reference to the end time of this range.
        /// \return A reference to the end time.
        public: const QualifiedTime &GetFinish() const;

        /// \brief Set the start time of this range. Passing in an indeterminate
        /// QualifiedTime (its default constructor) will tell this range to have
        /// no beginning (effectively, it should start at the beginning of the
        /// log).
        /// \param[in] _start The start time of this range.
        /// \return True if the new range is valid. False if the range is now
        /// invalid. The _start value will be accepted either way.
        /// \sa SetRange()
        /// \sa Valid()
        public: bool SetStart(const QualifiedTime &_start);

        /// \brief Set the finish time of this range. Passing in an
        /// indeterminate QualifiedTime (its default constructor) will tell this
        /// range to have no end (effectively, it should not stop until the end
        /// of the log).
        /// \param[in] _finish The finish time of this range.
        /// \return True if this new range is valid. False if the range is now
        /// invalid. The _finish value will be accepted either way.
        /// \sa SetRange()
        /// \sa Valid()
        public: bool SetFinish(const QualifiedTime &_finish);

        /// \brief Set both endpoints of the range.
        /// \param[in] _start The start time of this range.
        /// \param[in] _finish The end time of this range.
        /// \return True if this new range is valid. False if the range is now
        /// invalid. The values for _start and _finish will be accepted either
        /// way.
        /// \sa SetStart()
        /// \sa SetFinish()
        /// \sa Valid()
        public: bool SetRange(const QualifiedTime &_start,
                              const QualifiedTime &_finish);

        /// \brief Check if the range is valid. A valid range means that the
        /// finish time is guaranteed to be later than or coincident with the
        /// start time.
        /// \return True if the range is valid, false if invalid.
        public: bool Valid() const;

        /// \brief Virtual destructor
        public: ~QualifiedTimeRange();

        /// \internal Implementation class
        private: class Implementation;

        /// \internal PIMPL pointer
        private: std::unique_ptr<Implementation> dataPtr;
      };
    }
  }
}

#endif
