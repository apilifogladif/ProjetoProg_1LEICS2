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
        //se a cor dos pixeis for (r1, g1, b1) altera para (r2, g2, b2)
        int r1, g1, b1, r2, g2, b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
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
        //prencher um retângulo com w de comprimento e h de largura apartir da posição (x, y) com  a cor (r, g, b)
        int x, y, w, h, r, g, b;
        input >> x >> y >> w >> h >> r >> g >> b;
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
        // a segunda metade da imagem cortada na horizontal é o espelho da primera metade
        int w = image->width();
        int h = image->height();
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w/2; x++) {
                Color& og = image->at(x, y);
                Color& r = image->at(w - 1 - x, y);
                r.red() = og.red();
                r.green() = og.green();
                r.blue() = og.blue();
            }
        }
    }
    void Script::v_mirror() {
        // a segunda metade da imagem cortada na vertical é o espelho da primera metade
        int w = image->width();
        int h = image->height();
        for (int y = 0; y < h/2; y++) {
            for (int x = 0; x < w; x++) {
                Color& og = image->at(x, y);
                Color& r = image->at(x, h - 1 - y);
                r.red() = og.red();
                r.green() = og.green();
                r.blue() = og.blue();
            }
        }
    }
    void Script::add() {
        string filename; 
        int r, g, b, x, y;
        input >> filename >> r >> g >> b >> x >> y;

        // carregar imagem do ficheiro png
        Image *copia = loadFromPNG(filename)

        // determinar dimensões da outra imagem
        int width = copia->width();
        int height = copia->height();

        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                // cor do pixel da outra imagem
                Color& pixel = outraImagem->at(w, h);
                // se cor do pixel da outra imagem não for uma cor neutra, copiar pixel da imagem atual
                if (pixel.r != r || pixel.g != g || pixel.b != b) {
                    // coordenadas do pixel na imagem atual
                    int atualW = x + w;
                    int atualH = y + h;
                    // copiar pixel para a imagem atual
                    image->at(atualW, atualH) = pixel;
                }
            }   
        }
        
        // eliminar outra imagem
        delete copia;
    }
