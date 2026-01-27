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
  double cost;
  bool operator>(const Node &other) const { return cost > other.cost; }
};

class RouteFinder {
public:
  const NavDB &db;
  RouteFinder(const NavDB &database) : db(database) {}

  std::vector<std::string> findPath(const std::string &startID,
                                    const std::string &endID) {
    std::map<std::string, double> dist;
    std::map<std::string, std::string> parent;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    dist[startID] = 0.0;
    pq.push({startID, 0.0});

    while (!pq.empty()) {
      Node curr = pq.top();
      pq.pop();

      std::string u = curr.id;
      double currCost = curr.cost;

      if (u == endID) {
        break;
      }

      if (currCost > dist[u])
        continue;

      const auto *neighbors = db.getNeighbors(u);
      if (!neighbors)
        continue;

      for (const auto &segment : *neighbors) {
        std::string v = segment.toID;
        double weight = segment.distanceNM;

        if (dist.count(v) == 0 || currCost + weight < dist[v]) {
          dist[v] = currCost + weight;
          parent[v] = u;
          pq.push({v, dist[v]});
        }
      }
    }

    std::vector<std::string> path;
    if (parent.find(endID) == parent.end() && startID != endID) {
      return path;
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
};

} // namespace OFP

#endif // !ROUTEFINDER_H
