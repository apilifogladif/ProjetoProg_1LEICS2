#include "Image.hpp"

namespace prog
{
  // initialize pixel matrix with the size w * h (width times height), and each pixel is assigned according to its input color (fill)
  Image::Image(int w, int h, const Color &fill) : width_(w), height_(h), pixels_(w * h, fill) {} 
  
  Image::~Image() {} // destrutor

  int Image::width() const // read-only
  {
    return width_; 
  }
  int Image::height() const
  {
    return height_;
  }

  Color& Image::at(int x, int y) // can be modified
  {
    // y*widht_ + x: each line comes after the previous line, and to move it down a line, we have to jump the value of the line's width 
    // to calculate the pixel index in the vector based on its coordinates
    return pixels_[y*width_ + x]; 
  }

  const Color& Image::at(int x, int y) const // read-only
  {
    return pixels_[y*width_ + x];
  }
}
