#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

namespace prog {
    // read color values from a script file
    std::istream& operator>>(std::istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const std::string& filename) :
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
        std::string command;
        while (input >> command) {
            std::cout << "Executing command '" << command << "' ..." << std::endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // other commands require an image to be previously loaded
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
            if (command == "median_filter") {
                median_filter();
                continue;
            }
            if (command == "xpm2_open") {
                xpm2_open();
                continue;
            }
            if (command == "xpm2_save") {
                xpm2_save();
                continue;
            }
        }
    }

    void Script::open() {
        // replace current image (if any) with image read from PNG file
        clear_image_if_any();
        std::string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
	
    void Script::blank() {
        // replace current image (if any) with blank image
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }

    void Script::save() {
        // save current image to PNG file
        std::string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    void Script::xpm2_open() {
        // open xpm2 file
        clear_image_if_any();
        std::string filename;
        input >> filename;
        image = loadFromXPM2(filename);
    }

    void Script::xpm2_save() {
        // save xpm2 file
        std::string filename;
        input >> filename;
        saveToXPM2(filename, image);
    }

    void Script::invert() {
        // transforms each individual pixel (r, g, b) to (255-r,255-g,255-b)
        int width = image->width(); // get image width
        int height = image->height(); // get image height
	// goes through the matrix, pixel by pixel, changing the pixel color to (255-r,255-g,255-b), in order to invert the colors
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& c = image->at(w, h);
                c.red() = 255 - c.red();
                c.green() = 255 - c.green();
                c.blue() = 255 - c.blue();
            }
        }
    }

    void Script::to_gray_scale() {
        // transforms each individual pixel (r, g, b) to (v, v, v) where v = (r + g + b)/3
        int width = image->width();
        int height = image->height();
	// goes through the matrix, pixel by pixel, changing the pixel color to (v, v, v) where v = (r + g + b)/3, in order to put it in a gray scale
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& c = image->at(w, h);
                rgb_value v = (c.red() + c.green() + c.blue())/3; 
                c.red() = v;
                c.green() = v;
                c.blue() = v;                
            }
        }
    }

    void Script::replace() {
        // if the color of the pixel is (r1, g1, b1) it changes to (r2, g2, b2)
        int r1, g1, b1, r2, g2, b2;
        // user input of the color that will be replaced and the new color 
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        int width = image->width();
        int height = image->height();
	// goes through the matrix, pixel by pixel, and if the original color of the pixel (r1, g1, b1), it changes to (r2, g2, b2) 
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& c = image->at(w, h);
                if (c.red() == r1 && c.green() == g1 && c.blue() == b1) {
                    c.red() = r2;
                    c.green() = g2;
                    c.blue() = b2;
                }
            }
        }
    }

    void Script::fill() {
        // fill a rectangle with width w and height h starting on coordinates (x, y) with color (r, g, b)
        int x, y, w, h, r, g, b;
        // user input of the starting coordinates, width, height and color to fill of the rectangle
        input >> x >> y >> w >> h >> r >> g >> b;
	// goes through the matrix, pixel by pixel, filling the pixels with the correspondent color
        for (int wi = x; wi < x + w; wi++) {
            for (int he = y; he < y + h; he++) {
                Color& c = image->at(wi, he);
                c.red() = r;
                c.green() = g;
                c.blue() = b;
            }
        }
    }

    void Script::h_mirror() {
        // horizontal invertion
        int w = image->width();
        int h = image->height();
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w/2; x++) { // w/2 : mirror in the middle
                Color& color_a = image->at(x, y);
		// w - 1 - x : in order to obtain the corresponding pixel on the other side of the image, we have to subtract the image 
		// width by the current position (x) and by one (last position of a line is w - 1)
                Color& color_b = image->at(w - 1 - x, y); 
		// it swaps the values of the two variables
                std::swap(color_a, color_b); 
            }
        }
    }

    void Script::v_mirror() {
        // vertical invertion
        int w = image->width();
        int h = image->height();
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h/2; y++) { // h/2 : mirror in the middle
                Color& color_a = image->at(x, y);
		// h - 1 - y : in order to obtain the corresponding pixel on the other side of the image, we have to subtract the image 
		// height by the current position (y) and by one (last position of a column is h - 1)
                Color& color_b = image->at(x, h - 1 - y);
		// it swaps the values of the two variables
                std::swap(color_a, color_b);
            }
        }
    }

    void Script::add() {
        // copy all pixels from image stored in PNG file filename, except pixels in that image with “neutral” color (r, g, b),
        // to the rectangle of the current image with top-left corner (x, y) of the current image
        std::string filename;
        int r, g, b, x, y;
	    
        // user input of the name of the PNG file, "neutral" color and top-left color of the current image
        input >> filename >> r >> g >> b >> x >> y;

        // load new image from PNG file
        Image *copy = loadFromPNG(filename);

        // dimensions of that new image
        int width = copy->width();
        int height = copy->height();

        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                // pixel color of the new image
                Color& pixel = copy->at(w, h);
                // if pixel color not equal to the "neutral" color (obtained from the user input), copy pixel of the current image
                if (pixel.red() != r || pixel.green() != g || pixel.blue() != b) {
                    // coordinates of pixel in current image
                    int currentW = x + w;
                    int currentH = y + h;
                    // copy pixel to current image
                    image->at(currentW, currentH) = pixel;
                }
            }   
        }
        
        // delete the other image
        delete copy;
    }
	
    void Script::crop() {
        // crop an image to a rectangle with w of width, h of height and (x, y) of starting coordinates
        int x, y, w, h;
	    
        // user input of the starting coordinates, width, height of the rectangle
        input >> x >> y >> w >> h;
	 
        // create a new image with the width and height wanted
        Image* newImage = new Image(w, h);
	    
        // copy all pixels from the rectangle to the new image
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                Color& c = image->at(x + i, y + j);
                newImage->at(i, j) = c;
            }
        }
	    
        // change initial image to new image
        delete image;
        image = newImage;
    }
    
    void Script::rotate_left() {
        int width = image->width();
        int height = image->height();
	    
        // create new image with swapped dimensions
        Image* newImage = new Image(height, width);
	    
        // copy all pixels in the "rotated" order
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& originalPixel = image->at(w, h);
		// width - w - 1 : in order to obtain the corresponding pixel, we have to subtract the image width by the current position (w)
		// and by one (last position of a line is width - 1)
                Color& rotatedPixel = newImage->at(h, width - w - 1);
                rotatedPixel = originalPixel;
            }
        }
	    
        // change initial image to rotated image
        delete image;
        image = newImage;
    }

    void Script::rotate_right() {
        int width = image->width();
        int height = image->height();

        // create new image with swapped dimensions
        Image* newImage = new Image(height, width, Color());

        // copy all pixels in the "rotated" order
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& originalPixel = image->at(w, h);
		// height - h - 1 : in order to obtain the corresponding pixel, we have to subtract the image height by the current position (h)
		// and by one (last position of a column is height - 1)
                Color& rotatedPixel = newImage->at(height - h - 1, w);
                rotatedPixel = originalPixel;
            }
        }
	    
        // change initial image to rotated image
        delete image;
        image = newImage;
    }

    void Script::median_filter(){
        // width and height of the given image
        int w = image->width();
        int h = image->height(); 
        
	int ws;
        input >> ws;
	// user input has to be always odd and >=3
        if (ws % 2 == 0 || ws < 3) { 
            return;
        }
        
        // create a new image to store the filtered pixels
        Image* filteredImage = new Image(w, h);

        for (int y = 0; y < h; y++){ 
            for (int x = 0; x < w; x++){ 
                // extract all the pixels within the window boundaries
                std::vector<int> reds, greens, blues;
                for (int j = std::max(0, y - ws/2); j <= std::min(h - 1, y + ws/2); j++) {
                    for (int i = std::max(0, x - ws/2); i <= std::min(w - 1, x + ws/2); i++) {
                        // get the RGB values of the pixel at (i, j)
                        int r, g, b;
                        Color& c = image->at(i, j);
                        r = c.red();
                        g = c.green();
                        b = c.blue();
                        // store the RGB values in separate vectors
                        reds.push_back(r);
                        greens.push_back(g);
                        blues.push_back(b);
                    }
                }
                // sort the RGB values in ascending order and calculate the median value
                sort(reds.begin(), reds.end());
                sort(greens.begin(), greens.end());
                sort(blues.begin(), blues.end());

                int idxmedio = reds.size() / 2;
                int mr, mg, mb; // mr -> medianRed, mg -> medianGreen, mb -> medianBlue
		if (reds.size() % 2 == 0) {
			mr = (reds[idxmedio] + reds[idxmedio - 1])/2; 
			mg = (greens[idxmedio] + greens[idxmedio - 1])/2;
			mb = (blues[idxmedio] + blues[idxmedio - 1])/2;
		} else {
			mr = reds[idxmedio];
			mg = greens[idxmedio];
			mb = blues[idxmedio];
		}

                // assign the median pixel value to the corresponding pixel position in the filtered image
                Color& c = filteredImage->at(x, y);
                c.red() = mr;
                c.green() = mg;
                c.blue() = mb;
            }
        }

        // clean up memory
        delete image;
        image = filteredImage;
    }

}
