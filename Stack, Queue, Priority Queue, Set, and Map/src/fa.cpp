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

typedef ics::ArrayQueue<std::string>             InputsQueue;
typedef ics::ArrayMap<std::string,std::string>   InputStateMap;

typedef ics::ArrayMap<std::string,InputStateMap> FA;
typedef ics::pair<std::string,InputStateMap>     FAEntry;
typedef ics::ArrayPriorityQueue<FAEntry>         FAPQ;

typedef ics::pair<std::string,std::string>       Transition;
typedef ics::ArrayQueue<Transition>              TransitionQueue;

const FA read_fa(std::ifstream &file) {

	std::string line;
	FA fa;

	while (getline(file, line))
	{
		std::vector<std::string> words = ics::split(line, ";");
		for (unsigned int i = 1; i < words.size(); i += 2)
		{
			fa[words[0]].put(words[i], words[i+1]);
		}
	}

	return fa;
}


void print_fa(const FA& fa)
{
	std::cout << std::endl << "Finite Automation" << std::endl;

	FAPQ alphabetically([](const FAEntry& i, const FAEntry& j) {return i.first < j.first;});

	for ( auto i : fa )
		alphabetically.enqueue(i);

	for (auto k : alphabetically)
		std::cout << "  " << k.first<< " transitions: "<< k.second<<std::endl;
}


TransitionQueue process(const FA& fa, std::string state, const InputsQueue& inputs)
{


	TransitionQueue transitionQueue;
	Transition transition("", state);

	transitionQueue.enqueue(transition);
	InputStateMap stateMap;

	for (ics::Iterator<std::string>& i = inputs.abegin(); i != inputs.aend(); ++i) //101101
	{
		if (!fa[state].has_key(*i))
		{
		   Transition temp2(*i, "error");
		   transitionQueue.enqueue(temp2);
		   break;
		}
		  std::string new_state = fa[state][*i];
		  Transition temp2(*i, new_state);
		  transitionQueue.enqueue(temp2);
		  state = new_state;
	}

	return transitionQueue;

}


void interpret(const TransitionQueue& tq)
{
	int counter = 0;
	std::cout << "Starting new simulation" << std::endl << "Start state = " << tq.peek().second << std::endl;
	Transition t;
	for (ics::Iterator<Transition>& i = tq.abegin(); i != tq.aend(); ++i)
	{
		counter++;
		if(i == tq.abegin())
			continue;

		if(i->second == "error")
		{
				std::cout << "   Input = " << i->first << " illegal input: terminated" << std::endl << "Stop State = None" << std::endl;
				break;
		}

		std::cout << "   Input = " << i->first <<"; new state = " << i->second << std::endl;

		if (counter == tq.size())
			std::cout << "Stop state = " << i->second << std::endl;
	}
	std::cout << std::endl;
}


int main() {
  try {
	  std::ifstream text_file, text_file2;
	  ics::safe_open(text_file, "Enter file with finite automation: ", "faparity.txt");
	  FA fa = read_fa(text_file);

	  print_fa(fa);
	  std::cout << std::endl;

	  ics::safe_open(text_file2, "Enter file with start-state and input: ", "fainputparity.txt");
	  std::cout << std::endl;

      std::string line, state;

      while (getline(text_file2, line))
      {
    	  TransitionQueue transitionQueue;
    	  InputsQueue inputsQueue;
    	  std::vector<std::string> words = ics::split(line, ";");
    	  state = words[0];

  		for (unsigned int i = 1; i < words.size(); i++)
  			inputsQueue.enqueue(words[i]);

  		interpret(process(fa, state, inputsQueue));
      }

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}

*/
