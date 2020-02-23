#ifndef node_hpp
#define node_hpp

class Node {
 public:
  typedef struct {
    int length;
    Node* connected = nullptr;
  } corridor_t;

  enum types { START, END, NODE };
  static types types;

  enum dirs { UP, DOWN, LEFT, RIGHT };
  static dirs dirs;

  corridor_t left;
  corridor_t right;
  corridor_t up;
  corridor_t down;

  int x = -1, y = -1;
  int type = Node::NODE;

  float value_f = 0;
  float value_g = 0;
  float value_h = 0;
  Node* previous = nullptr;

  Node(){

  };

  Node(int _x, int _y) {
    x = _x;
    y = _y;
  };

  Node(int _x, int _y, int _type) {
    x = _x;
    y = _y;
    type = _type;
  }

  corridor_t* getCorridor(int i) {
    if (i == Node::UP)
      return &up;
    else if (i == Node::DOWN)
      return &down;
    else if (i == Node::LEFT)
      return &left;
    else
      return &right;
  }

  void print() { std::cout << x << " " << y << " " << type << std::endl; }
};

#endif