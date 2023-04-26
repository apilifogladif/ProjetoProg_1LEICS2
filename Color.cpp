#include "Color.hpp"

namespace prog {
    Color::Color() { //By default, the color should correspond to black
        r_ = 0 ;
        g_ = 0 ;
        b_ = 0 ;
    }

    //gives the private value the value of the respective values of the imput color c
    Color::Color(const Color& c) {
        this -> r_ = c.red();
        this -> g_ = c.green();
        this -> b_ = c.blue();
    }

    //gives the private value the value of the inputs
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        this -> r_ = red;
        this -> g_ = green;
        this -> b_ = blue;
    }

    // this 3 functions return the r,g and b values but you can not change them

    rgb_value Color::red() const {
        return r_;
    }
    rgb_value Color::green() const {
        return g_;
    }
    rgb_value Color::blue() const {
        return b_;
    }

    // TODO: remove this DUMMY variable once you have appropriate fields for representing colors. done
    // this 3 functions return the r,g and b values but you can change them

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
