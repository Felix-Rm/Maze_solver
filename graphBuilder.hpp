#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <map>
#include <list>

#include "node.hpp"
#include "point.hpp"

class GraphBuilder {
 public:
  static std::pair<Pt, Pt> build(std::map<Pt, Node*>* nodes, cv::Mat* maze, long long* count_white = nullptr) {
    cv::Vec3b black = cv::Vec3b(0, 0, 0);
    cv::Vec3b white = cv::Vec3b(255, 255, 255);
    cv::Vec3b not_present = cv::Vec3b(-1, -1, -1);

    long long white_squares = 0;
    std::pair<Pt, Pt> start_end = std::pair<Pt, Pt>(Pt(-1, -1, 0), Pt(-1, -1, 0));

    for (int row = 0; row < maze->rows; row++) {
      for (int col = 0; col < maze->cols; col++) {
        Pt p = Pt(col, row, maze->cols);
        cv::Vec3b pixel = maze->at<cv::Vec3b>(p);

        if (pixel == white) {
          white_squares++;
          cv::Vec3b top = row == 0 ? black : maze->at<cv::Vec3b>(cv::Point(col, row - 1));
          cv::Vec3b bottom = row == maze->rows - 1 ? black : maze->at<cv::Vec3b>(cv::Point(col, row + 1));
          cv::Vec3b left = col == 0 ? black : maze->at<cv::Vec3b>(cv::Point(col - 1, row));
          cv::Vec3b right = col == maze->cols - 1 ? black : maze->at<cv::Vec3b>(cv::Point(col + 1, row));

          if (top != bottom || left != right || (left == white && right == white && top == white && bottom == white)) {
            int type = row == 0 ? Node::START : row == maze->rows - 1 ? Node::END : Node::NODE;
            log(std::cout << "creating node at " << p.x << " " << p.y << " with type = " << type << "\n");

            if (type == Node::START)
              start_end.first = p;
            else if (type == Node::END)
              start_end.second = p;

            Node* this_node = new Node(col, row, type);
            nodes->insert(std::pair<Pt, Node*>(p, this_node));

            std::map<Pt, Node*>::iterator other_node_it;

            for (int x = p.x - 1; x >= 0; x--) {
              if (((cv::Vec3b)maze->at<cv::Vec3b>(cv::Point(x, p.y))) == black) break;
              if ((other_node_it = nodes->find(Pt(x, p.y, maze->cols))) != nodes->end()) {
                log(std::cout << "linking h " << p.x << " " << p.y << " with " << x << " " << p.y << "\n");
                linkNodes(other_node_it->second, this_node, p.x - x);
                break;
              }
            }

            for (int y = p.y - 1; y >= 0; y--) {
              if (((cv::Vec3b)maze->at<cv::Vec3b>(cv::Point(p.x, y))) == black) break;
              if ((other_node_it = nodes->find(Pt(p.x, y, maze->cols))) != nodes->end()) {
                log(std::cout << "linking v " << p.x << " " << p.y << " with " << p.x << " " << y << "\n");
                linkNodes(other_node_it->second, this_node, p.y - y, false);
                break;
              }
            }

            log(std::cout << "\n");
          }
        }
      }
    }
    if (count_white) *count_white = white_squares;
    return start_end;
  };

  static void linkNodes(Node* n1, Node* n2, int d, bool horizontal = true) {
    if (horizontal) {
      n1->right.connected = n2;
      n1->right.length = d;
      n2->left.connected = n1;
      n2->left.length = d;
    } else {
      n1->down.connected = n2;
      n1->down.length = d;
      n2->up.connected = n1;
      n2->up.length = d;
    }
  }
};