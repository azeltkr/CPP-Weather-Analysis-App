#include "../include/WeatherDataParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

// parses weather data from the dataset.
// [DONE WITH SOME ASSISTANCE]
std::map<std::string, std::vector<double>> parseWeatherData(const std::string& filename, const std::string& countryCode) {
    std::ifstream file(filename); 
    std::map<std::string, std::vector<double>> data; // map to store temperature data grouped by year
    std::string line; // variable to hold each line from the file

    // check if the file is open successfully
    if (!file.is_open()) {
        throw std::runtime_error("could not open file: " + filename); // throw an error if the file can't be opened
    }

    // read the header line (first line in the csv) to find the column index for the given country
    std::getline(file, line);
    std::stringstream headerStream(line); // stream to process the header line
    std::string columnName; // variable to hold column names
    int countryColumnIndex = -1; // index of the temperature column for the specified country
    int currentIndex = 0; // keeps track of the current column index while reading

    // loop through the header columns to find the correct column for the specified country
    while (std::getline(headerStream, columnName, ',')) { // split by commas
        if (columnName == countryCode + "_temperature") { // check if the column matches the country's temperature
            countryColumnIndex = currentIndex; // store the index of this column
            break; // exit the loop once the column is found
        }
        currentIndex++; // move to the next column
    }

    // throw an error if the specified country's column was not found
    if (countryColumnIndex == -1) {
        throw std::runtime_error("temperature column for country " + countryCode + " not found.");
    }

    // process each subsequent line in the file (actual data rows)
    while (std::getline(file, line)) { // read each line one by one
        std::stringstream lineStream(line); // stream to process the current line
        std::string timestamp, value; // variables to hold the timestamp and temperature value
        double temperature = 0.0; // variable to store the converted temperature

        // loop through columns in the current row to find the relevant data
        for (int i = 0; i <= countryColumnIndex; ++i) {
            std::getline(lineStream, value, ','); 
            if (i == 0) { 
                timestamp = value; // store the timestamp (first column in the row)
            }
        }

        try {
            temperature = std::stod(value); // convert the temperature value from string to double
        } catch (const std::exception&) {
            continue; // skip this row if the temperature value is invalid
        }

        // extract the year from the timestamp (first 4 characters of the timestamp)
        std::string year = timestamp.substr(0, 4); // e.g., from '1980-01-01T00:00:00Z' extract '1980'

        // group the temperature by year
        data[year].push_back(temperature); // add the temperature to the vector for the corresponding year
    }

    // return the grouped temperature data
    return data;
}



