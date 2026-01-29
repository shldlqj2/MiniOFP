#ifndef ROUTEFINDER_H
#define ROUTEFINDER_H

#include "NavDB.h"
#include <algorithm>
#include <ios>
#include <limits>
#include <map>
#include <queue>
#include <string>
#include <vector>

namespace OFP {
struct Node {
  std::string id;
  double f_score;
  bool operator>(const Node &other) const { return f_score > other.f_score; }
};

class RouteFinder {
public:
  const NavDB &db;
  RouteFinder(const NavDB &database) : db(database) {}

  double heuristic(const std::string &currID, const std::string &goalID){
    Coordinate p1 = db.getCoordinate(currID);
    Coordinate p2 = db.getCoordinate(goalID);
    return calculateDistance(p1, p2);
  }

  std::vector<std::string> findPath(const std::string &startID,
                                    const std::string &endID) {
    std::map<std::string, double> g_score;
    std::map<std::string, std::string> parent;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    g_score[startID] = 0.0;

    double startF = heuristic(startID, endID);
    pq.push({startID, startF});

    while (!pq.empty()) {
      Node curr = pq.top();
      pq.pop();

      std::string u = curr.id;

      if (u == endID) {
        break;
      }

      const auto *neighbors = db.getNeighbors(u);
      if(!neighbors) continue;

      for(const auto &segment : *neighbors){
        std::string v = segment.toID;
        double weight = segment.distanceNM;

        double tentative_g = g_score[u] + weight;

        if(g_score.find(v) == g_score.end() || tentative_g < g_score[v]){
          g_score[v] = tentative_g;
          parent[v] = u;

          double f = tentative_g + heuristic(v, endID);
          pq.push({v, f});
        }
      }

      std::vector<std::string> path;
      if (parent.find(endID) == parent.end() && startID != endID) {
                return path; // Path not found
            }

            std::string curr = endID;
            while (curr != startID) {
                path.push_back(curr);
                curr = parent[curr];
            }
            path.push_back(startID);
            std::reverse(path.begin(), path.end());

            return path;
        }

      
  }
};

} // namespace OFP

#endif // !ROUTEFINDER_H
