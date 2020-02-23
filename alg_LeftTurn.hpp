#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <map>
#include <list>

#include "node.hpp"
#include "point.hpp"

class LeftTurn {
 public:
  static std::list<Node*> solve(std::map<Pt, Node*>* nodes, std::pair<Pt, Pt>* start_end) {
    // list for found path
    std::list<Node*> path;
    Node* current;
    int dir;
    int priority[4][4];

    // start looking downwards
    dir = Node::DOWN;

    // priority of search directions for each 'walking direction'
    setPriority(priority[0], Node::DOWN, 0);
    setPriority(priority[0], Node::LEFT, 3);
    setPriority(priority[0], Node::UP, 2);
    setPriority(priority[0], Node::RIGHT, 1);

    current = nodes->find(start_end->first)->second;

    while (true) {
      path.push_back(current);

      for (int i = 0; i < 4; i++) {
        Node* new_current = current->getCorridor(priority[dir][i])->connected;
        if (new_current != nullptr) {
          // update current 'walking direction' and node
          current = new_current;
          dir = priority[dir][i];

          break;
        }
      }

      // if current node is an end node we are done
      if (current->type == Node::END) break;
    }

    // push end node
    path.push_back(current);
    return path;
  };

  static void setPriority(int* priority, int for_dir, int shifted_by) {
    int priority_down[4] = {Node::RIGHT, Node::DOWN, Node::LEFT, Node::UP};

    int index = shifted_by;
    for (int i = 0; i < 4; i++) {
      priority[for_dir * 4 + index] = priority_down[i];
      index = (index + 1) % 4;
    }
  }

  static void setNewPoint(Node* n, int new_dir) {}
};