//Sargis Dudaklyan, program1
//38671346

/*
#include <string>
#include <iostream>
#include <fstream>
#include <vector>                    //For use with split function in ics46goody
#include "ics46goody.hpp"
#include "iterator.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"

typedef ics::ArraySet<std::string>           NodeSet;
typedef ics::pair<std::string,NodeSet>       GraphEntry;
typedef ics::ArrayMap<std::string,NodeSet>   Graph;
typedef ics::ArrayPriorityQueue<std::string> NodePQ;



Graph read_graph(std::ifstream &file) {
 Graph graph;
 std::string line;

 while(getline(file, line))
 {
  std::vector<std::string> words = ics::split(line, ";");

  if (!graph[words[0]].contains(words[1]))
  {
   graph[words[0]].insert(words[1]);
  }
 }

 file.close();
 return graph;
}


void print_graph(const Graph& graph)
{

 NodePQ p( [](const std::string& a,const std::string& b) {return a < b;});
 for(GraphEntry kv : graph)
 {
  p.enqueue(kv.first);
 }
 for(std::string key : p)
 {
  if(!graph[key].empty())
   std::cout << " " << key << " --> " << graph[key] << std::endl;
 }

}



ics::ArraySet<std::string> reachable(const Graph& graph, std::string start) {

 NodeSet AS;
 ics::ArrayQueue<std::string> q;
 q.enqueue(start);

 while (q.size() != 0) {
  std::string temp = q.peek();
  AS.insert(q.dequeue());
  if(graph.has_key(temp))
   for (ics::Iterator<std::string>& i = graph[temp].abegin(); i != graph[temp].aend(); ++i)
    if (!AS.contains(*i))
     q.enqueue(*i);
 }

 return AS;

}


int main() {


 try {
    std::ifstream text_file;
    ics::safe_open(text_file,"Enter file name to analyze","graph1.txt, graph2.txt, graph3.txt");
    Graph graph = read_graph(text_file);
    print_graph(graph);


    std::string startingNode;
    while (startingNode != "quit")
        {
         std::cout << std::endl << "Enter starting node: ";
         std::cin >> startingNode;
         if (graph.has_key(startingNode))
         {
           NodeSet AS = reachable(graph, startingNode);
           std::cout << "Reachable from " << startingNode << " = " << AS; // << " = " << "set[";

         }
         else if (!graph.has_key(startingNode) && startingNode != "quit")
          std::cout <<"   " << startingNode <<" is not a key in the graph"<< std::endl;
        }


  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }


  return 0;
}

*/
