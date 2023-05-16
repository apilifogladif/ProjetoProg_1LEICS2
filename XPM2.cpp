#include "XPM2.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm> 
#include <string>
#include <cmath>
#include <map>
#include <vector>

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
        Image* image = new Image(w, h);
        
        // create a map to associate a char to a color
        std::map<char, Color> idsColors;
        
        for (int i = 0; i < n; i++) {
            getline(in, line);
            std::istringstream iss(line);

            // info given: only c ("color") entries
            char charColor, entry;
            std::string hexColor;
            iss >> charColor >> entry >> hexColor;
            // take the # out
            hexColor = hexColor.substr(1, 6);

            // the color is given as an hexadecimal number, so we have to convert to (red, green, blue)
            Color c;
            int value = 0;
            for (int i = 0; i < 6; i++) {
                if (isalpha(hexColor[i])) {
                    char aux = tolower(hexColor[i]);
                    value = aux - 'a' + 10;
                }
                else {
                    value = hexColor[i] - '0';
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
                image->at(col, l) = it->second;
            }
        }
        return image;
    }

    void saveToXPM2(const std::string& file, const Image* image) {
        std::ofstream out(file);

        // XPM2 header
        out << "! XPM2" << std::endl;
        // image dimensions
        int width = image->width();
        int height = image->height();
        // create a set to keep the colors used in image
        std::vector<Color> idsColors;
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                const Color& c = image->at(w, h);
                bool exists = false;
                int s = idsColors.size();
                //TODO use find function of algorithm
                for (int i = 0; i < s; i++) {
                    if (idsColors[i].red() == c.red() && idsColors[i].blue() == c.blue() && idsColors[i].green() == c.green()) {
                        exists = true;
                    }
                }
                if (!exists) {
                    idsColors.push_back(c);
                }
            }
        }
        // delete image;
        // number of colors
        int s = idsColors.size();
        // dimensions and color information
        out << std::to_string(width) << " " << std::to_string(height) << " " << std::to_string(s) << " 1" << std::endl;
        // assign a unique character to each color in the image
        //first char that will be used, position 33 in ASCII table, the others will be 33 + index in vector idsColors
        char charPos = '!';

        // write the color definitions
        for (int i = 0; i < s; i++) {
            const Color& c = idsColors[i];
            // convert the color to hexadecimal format
            std::stringstream hexColor;
            hexColor << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c.red());
            hexColor << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c.green());
            hexColor << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c.blue());
            // write the color definition line
            out << charPos++ << " c #" << hexColor.str() << "\n";
        }
        // write the image pixels
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                const Color& c = image->at(w, h);
                for (int i = 0; i < s; i++) {
                    charPos = '!';
                    if (idsColors[i].red() == c.red() && idsColors[i].blue() == c.blue() && idsColors[i].green() == c.green()) {
                        charPos += i;
                        out << charPos;
                    }
                }
            }
            out << "\n";
        }
        out.close();
    }
}
