#include "Color.hpp"

namespace prog {
    Color::Color() { //By default, the color should correspond to black
        r_ = 0 ;
        g_ = 0 ;
        b_ = 0 ;
    }
    Color::Color(const Color& c) {
        this -> r_ = c.red();
        this -> g_ = c.green();
        this -> b_ = c.blue();
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        this -> r_ = red;
        this -> g_ = green;
        this -> b_ = blue;
    }
    rgb_value Color::red() const {
        return r_;
    }
    rgb_value Color::green() const {
        return g_;
    }
    rgb_value Color::blue() const {
        return b_;
    }

    // TODO: remove this DUMMY variable once you have appropriate fields for representing colors.
    rgb_value DUMMY_rgb_value = 0; 

    rgb_value& Color::red()  {
      return DUMMY_rgb_value;
    }
    rgb_value& Color::green()  {
      return DUMMY_rgb_value;
    }
    rgb_value& Color::blue()  {
      return DUMMY_rgb_value;
    }
}
