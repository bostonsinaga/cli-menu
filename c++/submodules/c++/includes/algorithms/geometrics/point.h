#ifndef __MINI_TOOLS__ALGORITHMS__GEOMETRICS__POINT_H__
#define __MINI_TOOLS__ALGORITHMS__GEOMETRICS__POINT_H__

namespace mini_tools {
namespace algorithms {
namespace geometrics {

  class Point {
  protected:
    double x = 0, y = 0;

  public:
    double X() { return x; }
    double Y() { return y; }
    void X(double val) { x = val; }
    void Y(double val) { y = val; }

    Point() {}

    Point(double num) {
      x = num; y = num;
    }

    Point(double x_in, double y_in) {
      x = x_in; y = y_in;
    }

    double gradient();
    double gradientTo(Point pt);

    double hypotenuseZero();
    double hypotenuseTo(Point pt);

    Point operator+(Point pt);
    Point operator+(double num);

    Point operator-(Point pt);
    Point operator-(double num);

    Point operator*(Point pt);
    Point operator*(double num);

    Point operator/(Point pt);
    Point operator/(double num);

    void operator+=(Point pt);
    void operator+=(double num);

    void operator-=(Point pt);
    void operator-=(double num);

    void operator*=(Point pt);
    void operator*=(double num);

    void operator/=(Point pt);
    void operator/=(double num);

    bool operator==(Point pt);
    bool operator==(double num);

    bool operator!=(Point pt);
    bool operator!=(double num);

    bool operator<(Point pt);
    bool operator<(double num);

    bool operator<=(Point pt);
    bool operator<=(double num);

    bool operator>(Point pt);
    bool operator>(double num);

    bool operator>=(Point pt);
    bool operator>=(double num);

  private:
    Point divideCopy(Point &pt);
    Point divideCopy(double &num);
    void divideSelf(Point &pt);
    void divideSelf(double &num);
  };
}}}

#endif // __MINI_TOOLS__ALGORITHMS__GEOMETRICS__POINT_H__