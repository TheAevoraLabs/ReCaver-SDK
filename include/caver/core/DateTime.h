#ifndef CAVER_DATETIME_H
#define CAVER_DATETIME_H

#include <string>

namespace Caver {

namespace Proto {
class DateTime;
}

class DateTime {
public:
    DateTime() : m_seconds(0.0) {}
    DateTime(double seconds) : m_seconds(seconds) {}

    static DateTime Now();
    static DateTime DateWithSecondsSinceReferenceDate(double seconds);

    void LoadFromProtobufMessage(const Proto::DateTime& message);
    void SaveToProtobufMessage(Proto::DateTime* message);

    double SecondsSinceDate(const DateTime& other);
    std::string FormattedTimeIntervalSinceDate(const DateTime& other);

    double m_seconds;  // +0
};

double TimerSecondsSinceReferenceDate();

} // namespace Caver

#endif // CAVER_DATETIME_H
