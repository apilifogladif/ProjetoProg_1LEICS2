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
            /*if (command == "median_filter") {
                median_filter();
                continue;
            }*/
            
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
        // inverter horizontalmente
        int w = image->width();
        int h = image->height();
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w/2; x++) {
                Color& r = image->at(x, y);
                Color& og = image->at(w - 1 - x, y);
                swap(r, og);
            }
        }
    }
    void Script::v_mirror() {
        // inverter verticalmente
        int w = image->width();
        int h = image->height();
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h / 2; y++) {
                Color& r = image->at(x, y);
                Color& og = image->at(x, h-1-y);
                swap(r, og);
            }
        }
    }
    void Script::add() {
        string filename; 
        int r, g, b, x, y;
        input >> filename >> r >> g >> b >> x >> y;

        // carregar imagem do ficheiro png
        Image *copia = loadFromPNG(filename);

        // determinar dimensões da outra imagem
        int width = copia->width();
        int height = copia->height();

        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                // cor do pixel da outra imagem
                Color& pixel = copia->at(w, h);
                // se cor do pixel da outra imagem não for uma cor neutra, copiar pixel da imagem atual
                if (pixel.red() != r || pixel.green() != g || pixel.blue() != b) {
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
    void Script::crop() {
        // Cortar imagem para um retângulo de dimensões (x, y), largura w, e altura h
        int x, y, w, h;
        input >> x >> y >> w >> h;
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

        // Mudar imagem atual pela imagem rodada
        delete image;
        image = novaImagem;
    }


    /*void median_filter(){
        int ws;
        input >> ws;
    }*/
}
