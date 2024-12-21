#include <iostream>
#include "include/Candlestick.h"
#include "include/WeatherDataParser.h"
#include "include/WeatherDataProcessor.h"

int main() {
    try {
        auto data = parseWeatherData("data/weather_data.csv", "GB");  // Parse the data for Great Britain

        // Compute candlestick data
        auto candlesticks = computeCandlestickData(data);

        // Display the candlestick data
        std::cout << "Date\t\tOpen\tHigh\tLow\tClose\n";
        for (const auto& candle : candlesticks) {
            std::cout << candle.date << "\t" << candle.open << "\t" << candle.high
                      << "\t" << candle.low << "\t" << candle.close << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

