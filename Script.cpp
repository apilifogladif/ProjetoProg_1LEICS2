#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert") {
                invert();
                continue;
            }
            if (command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }
            if (command == "replace") {
                replace();
                continue;
            }
            if (command == "fill") {
                fill();
                continue;
            }
            if (command == "h_mirror") {
                h_mirror();
                continue;
            }
            if (command == "v_mirror") {
                v_mirror();
                continue;
            }
            if (command == "add") {
                add();
                continue;
            }
            if (command == "crop") {
                crop();
                continue;
            }
            if (command == "rotate_left") {
                rotate_left();
                continue;
            }
            if (command == "rotate_right") {
                rotate_right();
                continue;
            }
        }
    }



    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert() {
        // Transforms each individual pixel (r, g, b) to (255-r,255-g,255-b)
        int width = image->width();
        int height = image->height();
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& c = image->at(w, h);
                rgb_value& r = c.red();
                r = 255 - r;
                rgb_value& g = c.green();
                g = 255 - g;
                rgb_value& b = c.blue();
                b = 255 - b;
            }
        }
    }
    void Script::to_gray_scale() {
        //Transforms each individual pixel (r, g, b) to (v, v, v) where v = (r + g + b)/3.
        int width = image->width();
        int height = image->height();
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& c = image->at(w, h);
                rgb_value& r = c.red();
                rgb_value& g = c.green();
                rgb_value& b = c.blue();
                rgb_value v = (r + g + b)/3; 
                r = v;
                g = v;
                b = v;
            }
        }
    }
    void Script::replace() {
        int r1, g1, b1, r2, g2, b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        int width = image->width();
        int height = image->height();
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& c = image->at(w, h);
                rgb_value& r = c.red();
                rgb_value& g = c.green();
                rgb_value& b = c.blue();
                if (r == r1 && g == g1 && b == b1) {
                    r = r2;
                    g = g2;
                    b = b2;
                }
            }
        }
    }
}
