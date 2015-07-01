#ifndef DIJKSTRA_HPP_
#define DIJKSTRA_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "array_stack.hpp"
#include "hash_graph.hpp"

namespace ics {


int hash(const std::string& s) {std::hash<std::string> str_hash; return str_hash(s);}


class Info {
  public:
    Info(){}
    int          cost     = std::numeric_limits<int>::max();
    std::string from      = "?";
    bool operator == (const Info& rhs) const {
      return cost == rhs.cost && from == rhs.from;
    }
    bool operator != (const Info& rhs) const {
      return !(*this == rhs);
    }
    friend std::ostream& operator << (std::ostream& outs, const Info& i) {
      outs << "Info[" << i.cost << "," << i.from << "]";
      return outs;
    }

};


//Return the final_map as specified in the description of extended Dijkstra algorithm
ics::HashMap<std::string,Info> extended_dijkstra(const ics::HashGraph<int>& g, std::string start_node) {
}


//Return a string from the start node (implicit in answer_map) to the end node
//  (left to right, separated by a space)
std::string recover_path(ics::HashMap<std::string,Info> answer_map, std::string end_node) {
}


}

#endif /* DIJKSTRA_HPP_ */
