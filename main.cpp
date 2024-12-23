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

// function to display candlestick plot for a specific range
// [ASSISTED IMPLEMENTATION]
void displayPlotForRange(const std::vector<Candlestick>& candlesticks, int startYear, int endYear, int minTemp, int maxTemp) {
    std::vector<Candlestick> filteredCandlesticks;

    // dynamically filter candlesticks by the specified year range
    for (const auto& candle : candlesticks) {
        int year = std::stoi(candle.date.substr(0, 4)); // extract year from the date
        if (year >= startYear && year <= endYear) {
            filteredCandlesticks.push_back(candle);
        }
    }

    if (filteredCandlesticks.empty()) {
        // handle case where no data is available for the range
        std::cout << "No data available for the range " << startYear << "-" << endYear << ".\n";
        return;
    }

    // dynamically determine the actual range for filtered data
    int actualStartYear = std::stoi(filteredCandlesticks.front().date.substr(0, 4));
    int actualEndYear = std::stoi(filteredCandlesticks.back().date.substr(0, 4));

    std::cout << "\nCandlestick Plot for " << actualStartYear << "-" << actualEndYear << ":\n";
    plotCandlesticks(filteredCandlesticks, minTemp, maxTemp);
}

// main function
// [UPDATED FOR FILTER AND PROMPT FUNCTIONALITY]
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

        bool isFiltered = false; // flag to track if data was filtered

        // step 4: ask the user if they want to apply filters
        char applyFilters;
        std::cout << "\nWould you like to apply additional filters? [Y/N]: ";
        std::cin >> applyFilters;

        // step 5: apply filters if the user chooses to
        if (applyFilters == 'Y' || applyFilters == 'y') {
            int minTemp, maxTemp;
            std::cout << "Enter minimum temperature: ";
            std::cin >> minTemp;
            std::cout << "Enter maximum temperature: ";
            std::cin >> maxTemp;

            std::string startDate, endDate;
            std::cout << "Enter start date (YYYY-MM-DD): ";
            std::cin >> startDate;
            std::cout << "Enter end date (YYYY-MM-DD): ";
            std::cin >> endDate;

            // apply date and temperature range filters
            data = filterByDateRange(data, startDate, endDate); // call filterByDateRange
            data = filterByTemperatureRange(data, minTemp, maxTemp); // call filterByTemperatureRange
            isFiltered = true; // mark data as filtered
        }

        // step 6: compute candlestick data
        auto candlesticks = computeCandlestickData(data);

        // display yearly candlestick data table
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

        // step 7: prompt user based on filter status
        if (isFiltered) {
            // if data was filtered
            char viewPlot;
            std::cout << "\nWould you like to view the candlestick plot of the filtered data? [Y/N]: ";
            std::cin >> viewPlot;

            if (viewPlot == 'Y' || viewPlot == 'y') {
                // plot the candlestick chart for the entire filtered dataset
                int minTemp = -10, maxTemp = 40; // default temperature range for the plot
                std::cout << "\nGenerating candlestick plot for the filtered data:\n";
                displayPlotForRange(candlesticks, 1980, 2019, minTemp, maxTemp);
            } else {
                std::cout << "Candlestick plot generation skipped.\n";
            }
        } else {
            // if data was not filtered
            int startYear, endYear;
            std::cout << "\nEnter the start year that you would like to view the candlestick plot (e.g., 1980): ";
            std::cin >> startYear;
            endYear = startYear + 9; // cap range to 10 years

            if (startYear < 1980 || endYear > 2019) {
                std::cerr << "Error: Year range must be within 1980-2019.\n";
                return 1;
            }

            // display the candlestick plot for the specified range
            displayPlotForRange(candlesticks, startYear, endYear, -10, 40);
        }

    } catch (const std::exception& e) {
        // handle exceptions
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}





