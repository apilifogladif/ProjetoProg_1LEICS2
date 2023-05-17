#ifndef __prog_Image_hpp__
#define __prog_Image_hpp__
#include "Color.hpp"
#include <vector>

namespace prog
{
  class Image
  {
  private:
    int width_; 
    int height_; 
    std::vector<Color> pixels_; // pixel matrix
  public:
    Image(int w, int h, const Color &fill = {255, 255, 255}); // default color: white
    ~Image();
    int width() const;
    int height() const;
    Color &at(int x, int y); // modifier
    const Color &at(int x, int y) const; // read-only
  };
}
#endif
