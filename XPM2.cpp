#include "XPM2.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cmath>
#include <map>

namespace prog {
    Image* loadFromXPM2(const std::string& file) {
        std::ifstream in(file);
        std::string line;

        // info given: c == 1 is always true
        int w, h, n, c;

        // the first line is "! XPM2" and we don't need it
        getline(in, line);

        // in the second line of the file there is the info about the dimensions of the file, number of colors, and char per color
        getline(in, line);
        std::istringstream iss(line);
        iss >> w >> h >> n >> c;

        // create a new image with the given dimensions
        Image* newImage = new Image(w, h);
        
        // create a map to associate a char to a color
        std::map<char, Color> idsColors;
        
        for (int i = 0; i < n; i++) {
            getline(in, line);
            std::istringstream iss(line);

            // info given: only c ("color") entries
            char charColor, entry;
            std::string hexColor;
            iss >> charColor >> entry >> hexColor;

            // the color is given as an hexadecimal number, so we have to convert to (red, green, blue)
            Color c;
            // take the # out
            hexColor = hexColor.substr(1, 6);
            int value = 0;
            for (int i = 0; i < 6; i++) {
                if (isalpha(hexColor[i])) {
                    char aux = tolower(hexColor[i]);
                    value = aux - 87;
                }
                else if (hexColor[i] == 0) {
                    value = 0;
                }
                else {
                    value = hexColor[i] - 48;
                }
                switch (i) {
                    case 0:
                        c.red() = value * 16;
                        break;
                    case 1:
                        c.red() += value;
                        break;
                    case 2:
                        c.green() = value * 16;
                        break;
                    case 3:
                        c.green() += value;
                        break;
                    case 4:
                        c.blue() = value * 16;
                        break;
                    case 5:
                        c.blue() += value;
                        break;
                }
            }
            
            idsColors[charColor] = c;
        }

        for (int l = 0; l < h; l++) {       // lines
            getline(in, line);
            std::istringstream iss(line);
            for (int col = 0; col < w; col++) {   // columns
                // char in the line l and column c
                char charPos;
                iss >> charPos;
                // find in map idsColors the color the correspond to the char
                auto it = idsColors.find(charPos);
                newImage->at(col, l) = it->second;
            }
        }
        return newImage;
    }

    void saveToXPM2(const std::string& file, const Image* image) {

    }
}
