#ifndef WEATHER_DATA_PROCESSOR_H
#define WEATHER_DATA_PROCESSOR_H

#include <vector>
#include <map>
#include "Candlestick.h"

// function to compute candlestick data from grouped temperature data.
std::vector<Candlestick> computeCandlestickData(const std::map<std::string, std::vector<double>>& data);

#endif
