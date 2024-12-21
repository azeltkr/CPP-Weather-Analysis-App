#ifndef CANDLESTICK_H
#define CANDLESTICK_H

#include <string>

/* the Candlestick class represents a single candlestick entry
or a specific time frame (e.g., year-month-date) with temperature data. */
class Candlestick {
public:
    std::string date;  // the time frame (e.g., "1980" for a year).
    double open;       // average temperature from the previous time frame.
    double high;       // highest temperature during this time frame.
    double low;        // lowest temperature during this time frame.
    double close;      // average temperature for the current time frame.

    // constructor to initialize a Candlestick object.
    Candlestick(const std::string& date, double open, double high, double low, double close);
};

#endif
