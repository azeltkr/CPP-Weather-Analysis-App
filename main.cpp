#include <iostream>
#include <iomanip> // for std::fixed and std::setprecision
#include <string>
#include <map>
#include "include/WeatherDataParser.h"
#include "include/WeatherDataProcessor.h"
#include "include/Candlestick.h"

// predefined list of countries and their prefixes
// [DONE WITHOUT ASSISTANCE]
std::map<std::string, std::string> countryMap = {
    {"AT", "Austria"}, {"BE", "Belgium"}, {"BG", "Bulgaria"}, {"CH", "Switzerland"},
    {"CY", "Cyprus"}, {"CZ", "Czech Republic"}, {"DE", "Germany"}, {"DK", "Denmark"},
    {"EE", "Estonia"}, {"ES", "Spain"}, {"FI", "Finland"}, {"FR", "France"},
    {"GB", "United Kingdom"}, {"GR", "Greece"}, {"HR", "Croatia"}, {"HU", "Hungary"},
    {"IE", "Ireland"}, {"IT", "Italy"}, {"LT", "Lithuania"}, {"LU", "Luxembourg"},
    {"LV", "Latvia"}, {"MT", "Malta"}, {"NL", "Netherlands"}, {"PL", "Poland"},
    {"PT", "Portugal"}, {"RO", "Romania"}, {"SE", "Sweden"}, {"SI", "Slovenia"},
    {"SK", "Slovakia"}
};

// main function
// [DONE WITHOUT ASSISTANCE]
int main() {
    try {
        // step 1: display the list of available countries
        std::cout << "Available countries:\n";
        for (const auto& [code, name] : countryMap) {
            std::cout << code << " - " << name << "\n";
        }

        // step 2: prompt the user to input a country code
        std::string countryCode;
        std::cout << "\nEnter the 2-character country code (e.g., GB for United Kingdom): ";
        std::cin >> countryCode;

        // validate the country code
        if (countryMap.find(countryCode) == countryMap.end()) {
            std::cerr << "Error: Invalid country code. Please select a valid code from the list.\n";
            return 1;
        }

        // display the selected country
        std::cout << "You selected: " << countryMap[countryCode] << "\n";

        // step 3: parse the weather data for the selected country
        auto data = parseWeatherData("data/weather_data.csv", countryCode);

        // step 4: apply year and temperature range filters
        int startYear, endYear;
        std::cout << "Enter start year (1980-2019): ";
        std::cin >> startYear;
        std::cout << "Enter end year (1980-2019): ";
        std::cin >> endYear;

        data = filterByYearRange(data, startYear, endYear);

        int minTemp, maxTemp;
        std::cout << "Enter minimum temperature: ";
        std::cin >> minTemp;
        std::cout << "Enter maximum temperature: ";
        std::cin >> maxTemp;
        data = filterByTemperatureRange(data, minTemp, maxTemp);

        // step 5: compute candlestick data
        auto candlesticks = computeCandlestickData(data);

        // step 6: display the yearly candlestick data in a table
        std::cout << "\nYearly Candlestick Data for " << countryMap[countryCode] << ":\n";
        std::cout << "Date\t\tOpen\tHigh\tLow\tClose\n";
        for (const auto& candle : candlesticks) {
            std::cout << candle.date << "\t"
                      << std::fixed << std::setprecision(2)
                      << candle.open << "\t"
                      << candle.high << "\t"
                      << candle.low << "\t"
                      << candle.close << "\n";
        }

        // step 7: generate candlestick plot for filtered data
        char viewPlot;
        std::cout << "\nWould you like to view the candlestick plot of the filtered data? [Y/N]: ";
        std::cin >> viewPlot;

        if (viewPlot == 'Y' || viewPlot == 'y') {
            displayPlotForRange(candlesticks, startYear, endYear, minTemp, maxTemp);
        } else {
            std::cout << "Candlestick plot generation skipped.\n";
        }

        // step 8: predict future temperatures
        char predictTemp;
        std::cout << "\nWould you like to predict future temperatures based on historical data? [Y/N]: ";
        std::cin >> predictTemp;

        if (predictTemp == 'Y' || predictTemp == 'y') {
            int predictStartYear, predictEndYear;
            std::cout << "Enter the start year for prediction (e.g., 2025): ";
            std::cin >> predictStartYear;
            std::cout << "Enter the end year for prediction (e.g., 2030): ";
            std::cin >> predictEndYear;

            // collect historical years and averages
            std::vector<int> years;
            std::vector<double> averages;
            for (const auto& candle : candlesticks) {
                int year = std::stoi(candle.date.substr(0, 4));
                years.push_back(year);
                averages.push_back(candle.close); // use 'close' temperature as yearly average
            }

            // calculate slope and intercept
            auto [slope, intercept] = calculateLinearRegression(years, averages);

            // predict temperatures
            auto predictions = predictTemperatures(predictStartYear, predictEndYear, slope, intercept);

            // display predictions
            std::cout << "\nPredicted Temperatures:\n";
            std::cout << "Year\tPredicted Average Temperature\n";
            for (const auto& [year, predictedValue] : predictions) {
                std::cout << year << "\t" << std::fixed << std::setprecision(2) << predictedValue << "\n";
            }
        }


    } catch (const std::exception& e) {
        // handle exceptions
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}









