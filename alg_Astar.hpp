#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <map>
#include <list>
#include <vector>
#include <math.h>

#include "node.hpp"
#include "point.hpp"

class Astar {
 public:
  static std::list<Node*> solve(std::map<Pt, Node*>* nodes, std::pair<Pt, Pt>* start_end) {
    std::list<Node*> path;

    std::vector<Node*> open_set;
    std::vector<Node*> closed_set;

    Node* start = nodes->find(start_end->first)->second;
    Node* end = nodes->find(start_end->second)->second;

    open_set.push_back(start);

    while (true) {
      if (open_set.size() > 0) {
        log(std::cout << "open set: " << open_set.size() << std::endl);
        log(std::cout << "closed set: " << closed_set.size() << std::endl);

        Node* best = *open_set.begin();
        std::vector<Node*>::iterator best_iter = open_set.begin();

        for (std::vector<Node*>::iterator i = open_set.begin(); i != open_set.end(); i++) {
          if ((*i)->value_f < best->value_f) {
            best = *i;
            best_iter = i;
          }
        }

        if (best->type == Node::END) {
          // solution found
          Node* current = best;
          while (current->previous != nullptr) {
            path.push_back(current);
            current = current->previous;
            log(std::cout << current->x << " " << current->y << '\n');
          }

          return path;
        }

        closed_set.push_back(best);
        open_set.erase(best_iter);

        for (int i = 0; i < 4; i++) {
          Node::corridor_t* c = best->getCorridor(i);
          if (c->connected != nullptr && std::find(closed_set.begin(), closed_set.end(), c->connected) == closed_set.end()) {
            log(std::cout << "found valid neighbour\n");
            int temp_g = best->value_g + dist(best, c->connected);
            bool found_better = false;

            if (std::find(open_set.begin(), open_set.end(), c->connected) != open_set.end()) {
              log(std::cout << "neighbour is in open set\n");
              if (temp_g < c->connected->value_g) {
                log(std::cout << "better g found\n");
                c->connected->value_g = temp_g;
                found_better = true;
              }
            } else {
              log(std::cout << "neighbour is not in open set\n");
              c->connected->value_g = temp_g;
              found_better = true;
              open_set.push_back(c->connected);
            }

            if (found_better) {
              c->connected->value_h = dist(c->connected, end);
              c->connected->value_g = c->length;
              c->connected->value_f = c->connected->value_g + c->connected->value_h;
              c->connected->previous = best;
            }
          }
        }
        log(std::cout << "\n");

      } else {
        // no solution
        log(std::cout << "no solution\n");
        return std::list<Node*>();
      }
    }

    // just for the case we get here?
    log(std::cout << "shit hit the fan\n");
    return std::list<Node*>();
  };

  static inline double dist(Node* n1, Node* n2) { return std::sqrt(std::pow(n1->x - n2->x, 2) + std::pow(n1->y - n2->y, 2)); }
};
