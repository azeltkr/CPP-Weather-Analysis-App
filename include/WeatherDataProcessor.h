#ifndef WEATHER_DATA_PROCESSOR_H
#define WEATHER_DATA_PROCESSOR_H

#include <vector>
#include <map>
#include "Candlestick.h"

// function to compute candlestick data from grouped temperature data.
std::vector<Candlestick> computeCandlestickData(const std::map<std::string, std::vector<double>>& data);

// function to filter data by date range
std::map<std::string, std::vector<double>> filterByDateRange(
    const std::map<std::string, std::vector<double>>& data,
    const std::string& startDate, const std::string& endDate);

// function to filter data by temperature range
std::map<std::string, std::vector<double>> filterByTemperatureRange(
    const std::map<std::string, std::vector<double>>& data,
    int minTemp, int maxTemp);

void plotCandlesticks(const std::vector<Candlestick>& candlesticks, int minTemp, int maxTemp);

#endif
