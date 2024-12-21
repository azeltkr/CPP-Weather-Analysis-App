#include "../include/WeatherDataProcessor.h"
#include "../include/Candlestick.h"
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

std::vector<Candlestick> computeCandlestickData(const std::map<std::string, std::vector<double>>& data) {
    std::vector<Candlestick> candlesticks;
    double previousClose = 0.0;  // To store the previous year's closing value

    for (const auto& [year, temps] : data) {
        double high = *std::max_element(temps.begin(), temps.end());  // Highest temperature
        double low = *std::min_element(temps.begin(), temps.end());   // Lowest temperature
        double close = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size();  // Average temperature

        double open = previousClose;  // Open is the previous year's closing value
        candlesticks.emplace_back(year + "-01-01", open, high, low, close);

        previousClose = close;  // Update the previousClose for the next year
    }

    return candlesticks;
}
