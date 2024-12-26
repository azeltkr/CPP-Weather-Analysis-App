#include "../include/Candlestick.h"

// constructor implementation for the Candlestick class.
// initializes the candlestick with the given data.
Candlestick::Candlestick(const std::string& date, double open, double high, double low, double close)
    : date(date), open(open), high(high), low(low), close(close) {}

