#include "../include/WeatherDataProcessor.h"
#include "../include/Candlestick.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

// [DONE WITHOUT ASSISTANCE]
std::vector<Candlestick> computeCandlestickData(const std::map<std::string, std::vector<double>>& data) {
    std::vector<Candlestick> candlesticks;
    double previousClose = 0.0;  // to store the previous year's closing value

    for (const auto& [year, temps] : data) {
        double high = *std::max_element(temps.begin(), temps.end());  // highest temperature
        double low = *std::min_element(temps.begin(), temps.end());   // lowest temperature
        double close = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size();  // average temperature

        double open = previousClose;  // open is the previous year's closing value
        candlesticks.emplace_back(year + "-01-01", open, high, low, close);

        previousClose = close;  // update the previousClose for the next year
    }

    return candlesticks;
}

// [ASSISTED IMPLEMENTATION]
// function to filter data by date range
std::map<std::string, std::vector<double>> filterByDateRange(
    const std::map<std::string, std::vector<double>>& data,
    const std::string& startDate, const std::string& endDate) {
    std::map<std::string, std::vector<double>> filteredData;
    for (const auto& [date, temperatures] : data) {
        if (date >= startDate && date <= endDate) {  // check if date falls within the specified range
            filteredData[date] = temperatures;
        }
    }
    return filteredData;
}

// [ASSISTED IMPLEMENTATION]
// function to filter data by temperature range
std::map<std::string, std::vector<double>> filterByTemperatureRange(
    const std::map<std::string, std::vector<double>>& data,
    int minTemp, int maxTemp) {
    std::map<std::string, std::vector<double>> filteredData;
    for (const auto& [date, temperatures] : data) {
        std::vector<double> filteredTemps;
        for (double temp : temperatures) {
            if (temp >= minTemp && temp <= maxTemp) {  // check if temperature falls within the range
                filteredTemps.push_back(temp);
            }
        }
        if (!filteredTemps.empty()) {
            filteredData[date] = filteredTemps;
        }
    }
    return filteredData;
}

// function to plot candlestick data as a text-based chart
void plotCandlesticks(const std::vector<Candlestick>& candlesticks, int minTemp, int maxTemp) {
    const int plotWidth = candlesticks.size();
    const int plotHeight = maxTemp - minTemp + 1;
    const int columnSpacing = 6; // spacing between candlesticks

    // initialize a grid for the plot
    std::vector<std::string> grid(plotHeight, std::string(plotWidth * columnSpacing, ' '));

    for (size_t i = 0; i < candlesticks.size(); ++i) {
        const Candlestick& candle = candlesticks[i];

        // map temperature values to grid positions
        int highPos = std::max(0, maxTemp - static_cast<int>(std::round(candle.high)));
        int lowPos = std::max(0, maxTemp - static_cast<int>(std::round(candle.low)));
        int openPos = std::max(0, maxTemp - static_cast<int>(std::round(candle.open)));
        int closePos = std::max(0, maxTemp - static_cast<int>(std::round(candle.close)));
        int column = i * columnSpacing + columnSpacing / 2;

        // draw the vertical stalk (high to low)
        for (int row = highPos; row <= lowPos; ++row) {
            grid[row][column] = '|';
        }

        // draw the horizontal box (open to close)
        for (int row = std::min(openPos, closePos); row <= std::max(openPos, closePos); ++row) {
            grid[row][column - 1] = '+';
            grid[row][column] = '+';
            grid[row][column + 1] = '+';
        }
    }

    // render the plot grid with y-axis labels
    for (int row = 0; row < plotHeight; ++row) {
        std::cout << std::string(3 - std::to_string(maxTemp - row).length(), ' ')
                  << maxTemp - row << " | " << grid[row] << "\n";
    }

    // render the x-axis labels
    std::cout << "      ";
    for (size_t i = 0; i < candlesticks.size(); ++i) {
        int labelPos = i * columnSpacing + columnSpacing / 2 - 1;
        std::cout << std::string(labelPos - (6 * i), ' ') << (1980 + i);
    }
    std::cout << "\n";
}
