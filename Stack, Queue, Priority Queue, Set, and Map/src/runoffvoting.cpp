//Sargis Dudaklyan, program1
//38671346

/*
#include <string>
#include <iostream>
#include <fstream>
#include <vector>                    //For use with split function in ics46goody
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "ics46goody.hpp"
#include "iterator.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"

#include <sstream>

typedef ics::ArrayQueue<std::string>              CandidateQueue;
typedef ics::ArraySet<std::string>                CandidateSet;
typedef ics::ArrayMap<std::string,int>            CandidateTally;

typedef ics::ArrayMap<std::string,CandidateQueue> Preferences;
typedef ics::pair<std::string,CandidateQueue>     PreferencesEntry;

typedef ics::pair<std::string,int>                TallyEntry;
typedef ics::ArrayPriorityQueue<TallyEntry>       TallyEntryPQ;


Preferences read_voter_preferences(std::ifstream &file) {
 Preferences pref;
 std::string line;
 while(getline(file,line))
 {
  std::vector<std::string> words = ics::split(line, ";");
  for (int i = 1; i < words.size(); ++i)
  {
   pref[words[0]].enqueue(words[i]);
  }
 }
 file.close();
 return pref;
}


void print_voter_preferences(const Preferences& preferences) {

 ics::ArrayPriorityQueue<std::string> p([](const std::string& a, const std::string& b){return a < b;});

 for(PreferencesEntry kv : preferences)
 {
  p.enqueue(kv.first);
 }
 for(std::string title : p)
 {
  std::cout << " " << title << " --> " << preferences[title] << std::endl;
 }

}

void print_tally(std::string message, const CandidateTally& tally, bool (*sort_by)(const TallyEntry& i,const TallyEntry& j)) {


 TallyEntryPQ talPQ(sort_by);
 ics::ArraySet<std::string> p;

 for(TallyEntry kv : tally)
 {
  talPQ.enqueue(kv);
  p.insert(kv.first);
 }


 for(TallyEntry m : talPQ)
 {
  p.insert(m.first);
 }



 std::cout << message << p << std::endl;
 for(TallyEntry m : talPQ)
 {
  std::cout << m.first << " -> " << m.second << std::endl;
 }
}



CandidateTally evaluate_ballot(const Preferences& preferences, const CandidateSet& candidates) {
 CandidateTally CanTa;
 for(PreferencesEntry PE : preferences)
 {
  for(ics::Iterator<std::string>& i = preferences[PE.first].abegin(); i !=preferences[PE.first].aend(); i++)
  {
   if(candidates.contains(*i))
   {
    CanTa[*i] += 1;
    break;
   }
  }
 }


 return CanTa;
}

CandidateSet remaining_candidates(const CandidateTally& tally) {
 CandidateSet CanSet;
 int smallest = 0;
 for(TallyEntry te1 : tally)
 {
  if(smallest == 0)
  {
   smallest = te1.second;
  }

  if(smallest > te1.second)
  {
   smallest = te1.second;
  }
 }
 for(TallyEntry te2 : tally)
 {
  if(te2.second != smallest)
  {
   CanSet.insert(te2.first);
  }
 }
 return CanSet;
}


int main() {
  try {
   std::ifstream text_file;
   ics::safe_open(text_file,"Enter file name to analyze","votepref3.txt");
   Preferences pref = read_voter_preferences(text_file);
   print_voter_preferences(pref);
   CandidateSet temp;
   for(PreferencesEntry kv : pref)
   {
    for (ics::Iterator<std::string>& i = kv.second.abegin(); i != kv.second.aend(); i++)
       {
         temp.insert(*i);
       }
   }

   int count = 1;

   CandidateTally CanTa;
   for (ics::Iterator<std::string>& i = temp.abegin(); i != temp.aend(); i++)
   {
    CanTa[*i] = 0;
   }

   std::cout << std::endl;

   while(temp.size() > 1)
   {
    std::ostringstream convert;
    std::string s;
    convert << count;
    s = convert.str();

    std::string message = "Vote count on ballot # ";
    message += s;
    message += " with candidates alphabetically: still in election = ";


    std::string message2 = "Vote count on ballot # ";
    message2 += s;
    message2 += " with candidates numerically: still in election = ";


    CanTa = evaluate_ballot(pref, temp);
    print_tally(message , CanTa,
      [](const TallyEntry& i,const TallyEntry& j){return (i.first < j.first);});

    std::cout << std::endl;

    print_tally(message2, CanTa,
      [](const TallyEntry& i,const TallyEntry& j){return (i.second > j.second);});


    temp = remaining_candidates(CanTa);

    std::cout << std::endl;
    count++;
   }


   if( temp.size() == 1)
   {


    std::cout << "The winner is : " ;
    for(ics::Iterator<std::string>&i = temp.abegin(); i != temp.aend(); i++)
    {
     std::cout << *i;
    }

   }
   else
   {
    std::cout << "No winner: election is a tie among candidates remaining on the last ballot";
   }


  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;

}

*/
