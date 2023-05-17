#include "Color.hpp"

namespace prog {
    Color::Color() { // by default, the color should correspond to black
        r_ = 0;
        g_ = 0;
        b_ = 0;
    }

    // assigns to the private value the respective input color c 
    Color::Color(const Color& c) {
        this -> r_ = c.red();
        this -> g_ = c.green();
        this -> b_ = c.blue();
    }

    // assigns the correspondent input to the private value
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        this -> r_ = red;
        this -> g_ = green;
        this -> b_ = blue;
    }

    // these 3 functions return the r, g and b values, but you can not change them
    rgb_value Color::red() const {
        return r_;
    }
    rgb_value Color::green() const {
        return g_;
    }
    rgb_value Color::blue() const {
        return b_;
    }
    
    // these 3 functions return the r, g and b values, but you can modify them
    rgb_value& Color::red()  {
      return r_;
    }
    rgb_value& Color::green()  {
      return g_;
    }
    rgb_value& Color::blue()  {
      return b_;
    }
}
