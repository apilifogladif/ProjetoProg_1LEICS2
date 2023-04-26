#ifndef __prog_Image_hpp__
#define __prog_Image_hpp__
#include "Color.hpp"

namespace prog
{
  class Image
  {
  private:
    int width_;
    int height_;
    Color **pixels_;
  public:
    Image(int w, int h, const Color &fill = {255, 255, 255}) :
      width_(w),
      height_(h),
      pixels_(new Color *[h])
    {
      for (int i = 0; i < h; i++) {
        pixels_[i] = new Color[w];
        for (int j = 0; j < w; j++) {
          pixels_[i][j] = fill;
        }
      }
    ~Image() 
    {
      for (int i = 0; i < height_; i++) {
        delete[] pixels_[i];
      }
      delete[] pixels_;
    }
    int width() const
    {
      return width_;
    }
    int height() const {
      return height_;
    }
    Color &at(int x, int y)
    {
      return pixels_[y][x];
    }
    const Color &at(int x, int y) const 
    {
      return pixels_[y][x];
    }
  };
}
#endif
