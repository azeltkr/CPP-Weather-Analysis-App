#include "../include/WeatherDataProcessor.h"
#include "../include/Candlestick.h"
#include <iostream> // for std::cout
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <numeric> // for std::accumulate

// function to compute candlestick data from temperature data
// [done without assistance]
std::vector<Candlestick> computeCandlestickData(const std::map<std::string, std::vector<double>>& data) {
    std::vector<Candlestick> candlesticks;
    double previousClose = 0.0;

    for (const auto& [year, temps] : data) {
        double high = *std::max_element(temps.begin(), temps.end()); // highest temperature
        double low = *std::min_element(temps.begin(), temps.end());  // lowest temperature
        double close = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size(); // avg temp

        double open = previousClose; // use previous year's close as the current year's open
        candlesticks.emplace_back(year + "-01-01", open, high, low, close); // add to candlestick list

        previousClose = close; // update previous close for next iteration
    }

    return candlesticks;
}

// function to filter data by date range
// [assisted implementation]
std::map<std::string, std::vector<double>> filterByYearRange(
    const std::map<std::string, std::vector<double>>& data,
    int startYear, int endYear) {
    std::map<std::string, std::vector<double>> filteredData;

    for (const auto& [date, temperatures] : data) {
        int year = std::stoi(date.substr(0, 4)); // extract year from date
        if (year >= startYear && year <= endYear) {
            filteredData[date] = temperatures; // add data to filtered results
        }
    }

    return filteredData;
}


// function to filter data by temperature range
// [assisted implementation]
std::map<std::string, std::vector<double>> filterByTemperatureRange(
    const std::map<std::string, std::vector<double>>& data,
    int minTemp, int maxTemp) {
    std::map<std::string, std::vector<double>> filteredData;

    for (const auto& [date, temperatures] : data) {
        std::vector<double> filteredTemps;
        for (double temp : temperatures) {
            if (temp >= minTemp && temp <= maxTemp) { // include temps in the range
                filteredTemps.push_back(temp);
            }
        }
        if (!filteredTemps.empty()) {
            filteredData[date] = filteredTemps; // add filtered data
        }
    }

    return filteredData;
}


void displayPlotForRange(const std::vector<Candlestick>& candlesticks, int startYear, int endYear, int minTemp, int maxTemp) {
    std::vector<Candlestick> filteredCandlesticks;

    // filter candlesticks by the specified year range
    for (const auto& candle : candlesticks) {
        int year = std::stoi(candle.date.substr(0, 4)); // extract year from the date
        if (year >= startYear && year <= endYear) {
            filteredCandlesticks.push_back(candle);
        }
    }

    if (filteredCandlesticks.empty()) {
        std::cout << "No data available for the range " << startYear << "-" << endYear << ".\n";
        return;
    }

    // display the candlestick plot
    std::cout << "\nCandlestick Plot for " << startYear << "-" << endYear << ":\n";
    plotCandlesticks(filteredCandlesticks, minTemp, maxTemp);
}

// function to calculate linear regression coefficients (slope and intercept)
// slope = (n * Σ(xy) - Σx * Σy) / (n * Σ(x^2) - (Σx)^2)
// intercept = (Σy - slope * Σx) / n
std::pair<double, double> calculateLinearRegression(const std::vector<int>& years, const std::vector<double>& values) {
    int n = years.size();
    if (n == 0) throw std::runtime_error("Insufficient data for prediction");

    double sumX = std::accumulate(years.begin(), years.end(), 0.0);
    double sumY = std::accumulate(values.begin(), values.end(), 0.0);
    double sumXY = 0.0, sumX2 = 0.0;

    for (size_t i = 0; i < years.size(); ++i) {
        sumXY += years[i] * values[i];
        sumX2 += years[i] * years[i];
    }

    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    return {slope, intercept};
}

// function to predict future temperatures based on linear regression
std::vector<std::pair<int, double>> predictTemperatures(int startYear, int endYear, double slope, double intercept) {
    std::vector<std::pair<int, double>> predictions;

    for (int year = startYear; year <= endYear; ++year) {
        double predictedValue = slope * year + intercept;
        predictions.emplace_back(year, predictedValue);
    }

    return predictions;
}