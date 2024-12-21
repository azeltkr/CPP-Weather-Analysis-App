#ifndef WEATHER_DATA_PARSER_H
#define WEATHER_DATA_PARSER_H

#include <string>
#include <map>
#include <vector>

// function to parse weather data from a CSV file.
// groups temperatures by the given country and time frame (e.g., year-month-date).
std::map<std::string, std::vector<double>> parseWeatherData(const std::string& filename, const std::string& countryCode);

#endif