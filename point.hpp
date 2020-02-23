#ifndef point_hpp
#define point_hpp

#include <opencv2/opencv.hpp>

class Pt {
 public:
  int x, y;
  long long index;

  Pt(int _x, int _y, int w) {
    x = _x;
    y = _y;
    index = y * w + x;
  };

  operator cv::Point() { return cv::Point(x, y); };
};

namespace std {
template <>
struct less<Pt> {
  bool operator()(const Pt& lhs, const Pt& rhs) const { return lhs.index < rhs.index; }
};
}  // namespace std

#endif