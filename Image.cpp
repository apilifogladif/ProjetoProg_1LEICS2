#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill) : width_(w), height_(h), pixels_(w * h, fill) {}
  // inicializar pixel matrix com o tamanho w * h (largura vezes altura), e cada elemento é definido com a cor definida (fill)
  
  Image::~Image() {} //Destrutor

  int Image::width() const
  {
    return width_; 
  }
  int Image::height() const
  {
    return height_;
  }

  // TODO: remove this DUMMY_color variable once you have appropriate fields for representing image pixels. (feito)

  Color& Image::at(int x, int y) // versão mutável
  {
    return pixels_[y*width_ + x]; // y*widht_ + x: cada linha vem depois da linha anterior, e para mover uma linha para baixo temos de saltar o valor de uma linha de largura de pixeis
                                  // calcular índice do pixel no vetor com base das suas coordenadas
  }

  const Color& Image::at(int x, int y) const // versão apenas de leitura
  {
    return pixels_[y*width_ + x];
  }
}
