#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <map>
#include <list>
#include <chrono>

#define log(x)
//#define log(x) x;

#include "node.hpp"
#include "point.hpp"

#include "graphBuilder.hpp"

#include "alg_LeftTurn.hpp"
#include "alg_Astar.hpp"

using namespace std;

void displayImage(cv::Mat* mat, string name) {
  cv::Mat display;
  cv::resize(*mat, display, cv::Size(600, 600), 0, 0, cv::InterpolationFlags::INTER_NEAREST);
  cv::namedWindow(name, cv::WINDOW_NORMAL);
  cv::imshow(name, display);
  cv::waitKey();
}

int main(int argc, char* argv[]) {
  map<Pt, Node*> nodes;
  long long white_squares;
  chrono::time_point<chrono::high_resolution_clock> time_start, time_end;
  chrono::duration<int64_t, std::nano> time_elapsed;

  string dirname = argc > 1 ? argv[2] : "";
  string filename = argv[1];

  cv::Mat maze = cv::imread(dirname + filename);

  cout << "starting..." << endl;
  time_start = chrono::high_resolution_clock::now();

  pair<Pt, Pt> start_end = GraphBuilder::build(&nodes, &maze, &white_squares);

  time_end = chrono::high_resolution_clock::now();
  time_elapsed = time_end - time_start;
  long long node_count = nodes.size();

  cout << "done" << endl;
  cout << "compressed search space from " << white_squares << " to " << node_count << " delta: " << (node_count - white_squares) << endl;
  cout << "time elapsed: " << chrono::duration_cast<chrono::seconds>(time_elapsed).count() << "s = " << chrono::duration_cast<chrono::microseconds>(time_elapsed).count() << "us" << endl;
  cout << endl;

  for (map<Pt, Node*>::iterator i = nodes.begin(); i != nodes.end(); i++) {
    cv::Vec3b color;
    if (i->second->type == Node::START) color = cv::Vec3b(0, 255, 0);
    if (i->second->type == Node::END) color = cv::Vec3b(0, 0, 255);
    if (i->second->type == Node::NODE) color = cv::Vec3b(255, 0, 0);

    maze.at<cv::Vec3b>(cv::Point(i->second->x, i->second->y)) = color;

    // for (int j = 0; j < 4; j++) {
    //   Node* other = i->second->getCorridor(j)->connected;
    //   if (other != nullptr) cv::line(maze, cv::Point(i->second->x, i->second->y), cv::Point(other->x, other->y), cv::Scalar(100, 100, 100));
    // }
  }

  displayImage(&maze, "maze_graph");

  cout << "starting..." << endl;

  time_start = chrono::high_resolution_clock::now();

  list<Node*> path = Astar::solve(&nodes, &start_end);

  time_end = chrono::high_resolution_clock::now();
  time_elapsed = time_end - time_start;

  cout << "done" << endl;
  cout << "path length = " << path.size() << endl;
  cout << "time elapsed: " << chrono::duration_cast<chrono::seconds>(time_elapsed).count() << "s = " << chrono::duration_cast<chrono::microseconds>(time_elapsed).count() << "us" << endl;

  Node* other = *path.begin();
  for (list<Node*>::iterator i = ++path.begin(); i != path.end(); i++) {
    cv::line(maze, cv::Point((*i)->x, (*i)->y), cv::Point(other->x, other->y), cv::Scalar(0, 255, 0));
    other = *i;
  }

  displayImage(&maze, "maze_graph");

  cv::imwrite(dirname + "solved_" + filename, maze);
}