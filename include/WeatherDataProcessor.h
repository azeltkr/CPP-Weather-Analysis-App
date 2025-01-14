#ifndef WEATHER_DATA_PROCESSOR_H
#define WEATHER_DATA_PROCESSOR_H

#include <vector>
#include <map>
#include <string>
#include "Candlestick.h"


// function to compute candlestick data from grouped temperature data.
std::vector<Candlestick> computeCandlestickData(const std::map<std::string, std::vector<double>>& data);

// function to filter data by date range
std::map<std::string, std::vector<double>> filterByYearRange(
    const std::map<std::string, std::vector<double>>& data,
    int startYear, int endYear);


// function to filter data by temperature range
std::map<std::string, std::vector<double>> filterByTemperatureRange(
    const std::map<std::string, std::vector<double>>& data,
    int minTemp, int maxTemp);

// function to plot candlesticks for a range
void displayPlotForRange(const std::vector<Candlestick>& candlesticks, int startYear, int endYear, int minTemp, int maxTemp);

void plotCandlesticks(const std::vector<Candlestick>& candlesticks, int minTemp, int maxTemp);

// function to calculate linear regression coefficients
std::pair<double, double> calculateLinearRegression(const std::vector<int>& years, const std::vector<double>& values);

// function to predict temperatures based on linear regression
std::vector<std::pair<int, double>> predictTemperatures(int startYear, int endYear, double slope, double intercept);

#endif
