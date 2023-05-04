#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
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
            if (command == "median_filter") {
                median_filter();
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
                c.red() = 255 - c.red();
                c.green() = 255 - c.green();
                c.blue() = 255 - c.blue();
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
                rgb_value v = (c.red() + c.green() + c.blue())/3; 
                c.red() = v;
                c.green() = v;
                c.blue() = v;                
            }
        }
    }
    void Script::replace() {
        //if the color of the pixel is (r1, g1, b1) it changes to (r2, g2, b2)
        int r1, g1, b1, r2, g2, b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;      //user input of the color that will be replaced and the new color 
        int width = image->width();
        int height = image->height();
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
        //fill a rectangle with width w and height h starting on coordinates (x, y) with color (r, g, b)
        int x, y, w, h, r, g, b;
        input >> x >> y >> w >> h >> r >> g >> b;       //user input of the starting coordinates, width, height and color to fill of the rectangle
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
            for (int x = 0; x < w/2; x++) {
                Color& color_a = image->at(x, y);
                Color& color_b = image->at(w - 1 - x, y);
                swap(color_a, color_b);
            }
        }
    }
    void Script::v_mirror() {
        // vertical invertion
        int w = image->width();
        int h = image->height();
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h / 2; y++) {
                Color& color_a = image->at(x, y);
                Color& color_b = image->at(x, h-1-y);
                swap(color_a, color_b);
            }
        }
    }
    void Script::add() {
        //Copy all pixels from image stored in PNG file filename, except pixels in that image with “neutral” color (r, g, b),
        // to the rectangle of the current image with top-left corner (x, y) of the current image. 
        string filename; 
        int r, g, b, x, y;
        //user input of the name of the PNG file, "neutral" color and top-left color of the current image
        input >> filename >> r >> g >> b >> x >> y;

        // load new image from PNG file
        Image *copia = loadFromPNG(filename);

        // dimensions of that new image
        int width = copia->width();
        int height = copia->height();

        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                // pixel color of the new image
                Color& pixel = copia->at(w, h);
                // if pixel color not equal to the "neutral" color (obtained from the user input), copy pixel of the current image
                if (pixel.red() != r || pixel.green() != g || pixel.blue() != b) {
                    //coordinates of pixel in current image
                    int atualW = x + w;
                    int atualH = y + h;
                    //copy pixel to current image
                    image->at(atualW, atualH) = pixel;
                }
            }   
        }
        
        // delete the other image
        delete copia;
    }
    void Script::crop() {
        // crop an image to a rectangle with w of width, h of height and (x, y) of starting coordinates
        int x, y, w, h;
        input >> x >> y >> w >> h;
        // 
        // Criar uma nova imagem com as medidas que queremos
        Image* novaImagem = new Image(w, h);
        // Copiar pixeis para o retângulo para a nova imagem
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                Color& c = image->at(x + i, y + j);
                novaImagem->at(i, j) = c;
            }
        }
        // Alterar imagem atual para a nova imagem cortada
        delete image;
        image = novaImagem;
    }
    
    void Script::rotate_left() {
        int width = image->width();
        int height = image->height();

        // Criar nova imagem com as dimensões trocadas
        Image* novaImagem = new Image(height, width);

        // Copiar pixeis em ordem "rodada"
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& pixelOriginal = image->at(w, h);
                Color& pixelRodado = novaImagem->at(h, width - w - 1);
                pixelRodado = pixelOriginal;
            }
        }

        // Mudar imagem atual pela imagem rodada
        delete image;
        image = novaImagem;
    }

    void Script::rotate_right() {
        int width = image->width();
        int height = image->height();

        // Criar nova imagem com as dimensões trocadas
        Image* novaImagem = new Image(height, width, Color());

        // Copiar pixeis em ordem "rodada"
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                Color& pixelOriginal = image->at(w, h);
                Color& pixelRodado = novaImagem->at(height - h - 1, w);
                pixelRodado = pixelOriginal;
            }
        }
        delete image;
        image = novaImagem;
    }

    void Script::median_filter(){
        int ws;
        input >> ws; //input sempre impar 3,5,7,...
        int w = image->width();
        int h = image->height(); //altura e largura da imagem dada
        
        if (ws % 2 == 0 || ws < 3) {
            return;
        }
        
        // Create a new image to store the filtered pixels
        Image* filteredImage = new Image(w, h);

        for(int y = 0; y < h; y++){ //y -- altura
            for(int x = 0; x < w; x++){ //x -- largura

                // Extract all the pixels within the window boundaries
                vector<int> reds, greens, blues;
                for (int j = max(0, y - ws/2); j <= min(h - 1, y + ws/2); j++) {
                    for (int i = max(0, x - ws/2); i <= min(w - 1, x + ws/2); i++) {
                        // Get the RGB values of the pixel at (i, j)
                        int r, g, b;
                        Color& c = image->at(i, j);
                        r = c.red();
                        g = c.green();
                        b = c.blue();

                        // Store the RGB values in separate vectors
                        reds.push_back(r);
                        greens.push_back(g);
                        blues.push_back(b);
                    }
                }
                // Sort the RGB values in ascending order and calculate the median value
                sort(reds.begin(), reds.end());
                sort(greens.begin(), greens.end());
                sort(blues.begin(), blues.end());

                int idxmedio = reds.size() / 2;
                int mr, mg, mb;     //mr->medianRed
				if (reds.size() % 2 == 0) {
					mr = (reds[idxmedio] + reds[idxmedio - 1])/2;
					mg = (greens[idxmedio] + greens[idxmedio - 1])/2;
					mb = (blues[idxmedio] + blues[idxmedio - 1])/2;
				}
				else {
					mr = reds[idxmedio];
					mg = greens[idxmedio];
					mb = blues[idxmedio];
				}

                // Assign the median pixel value to the corresponding pixel position in the filtered image
                //filteredImage->set_pixel(x, y, medianR, medianG, medianB);
                Color& c = filteredImage->at(x, y);
                c.red() = mr;
                c.green() = mg;
                c.blue() = mb;
            }
        }

        // Clean up memory
        delete image;
        image = filteredImage;
    }

}
