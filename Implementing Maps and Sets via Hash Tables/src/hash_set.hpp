/*
SARGIS DUDAKLYAN ID: 38671346, Program 4
*/

#ifndef HASH_SET_HPP_
#define HASH_SET_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "pair.hpp"
#include "iterator.hpp"
#include "set.hpp"


namespace ics {

template<class T> class HashSet : public Set<T>	{
 public:
   HashSet() = delete;
   HashSet(int (*ahash)(const T& element), double the_load_factor = 1.0);
   HashSet(int initial_bins, int (*ahash)(const T& element), double the_load_factor = 1.0);
   HashSet(const HashSet<T>& to_copy);
   //HashSet(std::initializer_list<Entry> il, int (*ahash)(const T& element), double the_load_factor = 1.0);
   HashSet(ics::Iterator<T>& start, const ics::Iterator<T>& stop, int (*ahash)(const T& element), double the_load_factor = 1.0);
   virtual ~HashSet();

   virtual bool empty      () const;
   virtual int  size       () const;
   virtual bool contains   (const T& element) const;
   virtual std::string str () const;

   virtual bool contains (ics::Iterator<T>& start, const ics::Iterator<T>& stop) const;

   virtual int  insert (const T& element);
   virtual int  erase  (const T& element);
   virtual void clear  ();

   virtual int insert (ics::Iterator<T>& start, const ics::Iterator<T>& stop);
   virtual int erase  (ics::Iterator<T>& start, const ics::Iterator<T>& stop);
   virtual int retain (ics::Iterator<T>& start, const ics::Iterator<T>& stop);

   virtual HashSet<T>& operator = (const HashSet<T>& rhs);
   virtual bool operator == (const Set<T>& rhs) const;
   virtual bool operator != (const Set<T>& rhs) const;
   virtual bool operator <= (const Set<T>& rhs) const;
   virtual bool operator <  (const Set<T>& rhs) const;
   virtual bool operator >= (const Set<T>& rhs) const;
   virtual bool operator >  (const Set<T>& rhs) const;

   template<class T2>
   friend std::ostream& operator << (std::ostream& outs, const HashSet<T2>& s);

   virtual ics::Iterator<T>& abegin () const;
   virtual ics::Iterator<T>& aend   () const;

 private:
   class LN;

 public:
   class Iterator : public ics::Iterator<T> {
     public:
       //KLUDGE should be callable only in begin/end
       Iterator(HashSet<T>* fof, bool begin);
       Iterator(const Iterator& i);
       virtual ~Iterator();
       virtual T           erase();
       virtual std::string str  () const;
       virtual const ics::Iterator<T>& operator ++ ();
       virtual const ics::Iterator<T>& operator ++ (int);
       virtual bool operator == (const ics::Iterator<T>& rhs) const;
       virtual bool operator != (const ics::Iterator<T>& rhs) const;
       virtual T& operator *  () const;
       virtual T* operator -> () const;
     private:
       ics::pair<int,LN*> current; //Bin Index and Cursor; stop: LN* == nullptr
       HashSet<T>*        ref_set;
       int                expected_mod_count;
       bool               can_erase = true;
       void advance_cursors();
   };

   virtual Iterator begin () const;
   virtual Iterator end   () const;

 private:
   class LN {
     public:
       LN ()                      : next(nullptr){}
       LN (const LN& ln)          : value(ln.value), next(ln.next){}
       LN (T v,  LN* n = nullptr) : value(v), next(n){}

       T   value;
       LN* next;
   };

   LN** set      = nullptr;
   int (*hash)(const T& element);
   double load_factor;//used/bins <= load_factor
   int bins      = 1; //# bins in array
   int used      = 0; //# of key->value pairs in the hash table
   int mod_count = 0; //For sensing concurrent modification
   int   hash_compress (const T& element) const;
   void  ensure_load_factor(int new_used);
   LN*   find_element (int bin, const T& element) const;
   LN*   copy_list(LN*   l) const;
   LN**  copy_hash_table(LN** ht, int bins) const;
   void  delete_hash_table(LN**& ht, int bins);
 };





template<class T>
HashSet<T>::HashSet(int (*ahash)(const T& element), double the_load_factor) : hash(ahash), load_factor(the_load_factor) {
	 set = new LN*[bins];

	 	LN* newNode = new LN();
	 	newNode->value= T();
	 	newNode->next = nullptr;

	 	for (int i = 0; i < bins; i++) {
	 		set[i] = newNode;
	 	}
}

template<class T>
HashSet<T>::HashSet(int initial_bins, int (*ahash)(const T& element), double the_load_factor) : bins(initial_bins), hash(ahash), load_factor(the_load_factor) {
 //write code here
	 set = new LN*[bins];
	 for(int i = 0; i < bins; i++)
	 set[i] = new LN();
}

template<class T>
HashSet<T>::HashSet(const HashSet<T>& to_copy) : hash(to_copy.hash), load_factor(to_copy.load_factor), bins(to_copy.bins), used(to_copy.used) {
 //write code here
	set = copy_hash_table(to_copy.set, bins);

}

template<class T>
HashSet<T>::HashSet(ics::Iterator<T>& start, const ics::Iterator<T>& stop, int (*ahash)(const T& element), double the_load_factor) : hash(ahash), load_factor(the_load_factor) {
	//insert(start,stop);
	set = new LN*[bins];
	for(int i = 0; i < bins; i++)
	set[i] = new LN();
	insert(start,stop);
}

//KLUDGE: Not usable yet
//template<class KEY,class T>
//HashSet<T>::HashSet(std::initializer_list<Entry> i,int (*ahash)(const KEY& k), double the_load_factor = 1.0) {
//  set = new LN*[bins];
//  put(il.abegin(),il.aend());
//}

template<class T>
HashSet<T>::~HashSet() {
 delete_hash_table(set,bins);
}

template<class T>
inline bool HashSet<T>::empty() const {

	return(used==0);
}

template<class T>
int HashSet<T>::size() const {
 return used;
}

template<class T>
bool HashSet<T>::contains (const T& element) const {
 //write code here
	int index=hash_compress(element);
	if (find_element(index,element)!=nullptr)
		return true;
	else
		return false;
}

template<class T>
std::string HashSet<T>::str() const {
		std::ostringstream outputStr;
		if (this->size() == 0)
		{
			outputStr << "set[]";
			return outputStr.str();
		}

		for (int i = 0; i < bins; i++) {
			outputStr << "\nbin[" << i << "]: ";
			for (LN* l = set[i]; l != nullptr; l = l->next)
			{
				outputStr << "set[" << l->value << "]" << (l->next != nullptr ? " -> " : " -> #");

			}
		}
		return outputStr.str();
}

template<class T>
bool HashSet<T>::contains(ics::Iterator<T>& start, const ics::Iterator<T>& stop) const {
 //write code here
	for (; start != stop; start++)
	 {
	  if (!contains(*start))
	  {
	   return false;
	  }
	 }
	return true;
}

template<class T>
int HashSet<T>::insert(const T& element) {
 //write code here
	   if (contains(element))
		   return 0;

		LN* newNode = new LN;
		newNode->value = element;
		newNode->next = nullptr;
		this->ensure_load_factor(used + 1);
		int index = hash_compress(element);

		LN*& n = set[index];
		newNode->next = n;
		n = newNode;
		used++;
		return 1;
}

template<class T>
int HashSet<T>::erase(const T& element) {
 //write code here
		if (contains(element)) {
			used--;
			int index = hash_compress(element);

			LN* head = set[index];

			if (set[index]->value == element)
			{
				set[index]=head->next;

			    head->next=nullptr;
				delete head;
				mod_count++;
				return 1;
			}
			else
			{
			    LN* l;
				for(LN* p = head; p->next!= nullptr;)
				{
					l = p;
					p = p->next;
					if (p->value == element)
					{
						l->next = p->next;
						p->next = nullptr;
						delete p;
						break;
					}
				}
				mod_count++;
			return 1;
			}
		}
		else
			return 0;
		//mod_count++;
}

template<class T>
void HashSet<T>::clear() {
		for (int i=0; i<bins; i++)
			{
			 while(set[i]->next != nullptr)
			 {		LN* to_del = set[i];

			  set[i] = set[i]->next;
			  to_del->next = nullptr;
			  delete to_del;
			  to_del = set[i];
			 }
			}
	used=0;
 mod_count++;
}

template<class T>
int HashSet<T>::insert(ics::Iterator<T>& start, const ics::Iterator<T>& stop) {
 //write code here
	int i = 0;
	 for(; start != stop; start++)
	 {
	  insert(*start);
	  i++;
	 }
	 return i;
}

template<class T>
int HashSet<T>::erase(ics::Iterator<T>& start, const ics::Iterator<T>& stop) {
 //write code here
	 int i = 0;
	 for(; start != stop; start++)
	 {
	  i += erase(*start);
	 }
	 return i;
}

template<class T>
int HashSet<T>::retain(ics::Iterator<T>& start, const ics::Iterator<T>& stop) {
  int count = 0;
	HashSet* hs = new HashSet(bins,hash,load_factor);
	 for (; start != stop; start++)
	 {
	  if(contains(*start))
	  hs->insert(*start);
	 }
	  this->clear();
	 for (int j=0; j<hs->bins; j++)
	 for(LN* p = hs->set[j]; p->next != nullptr;  p= p->next)
	 {
	  this->insert(p->value);
	  count++;
	 }
	  hs->clear();
	  delete hs;
	  return count;
}

template<class T>
HashSet<T>& HashSet<T>::operator = (const HashSet<T>& rhs) {
 //write code here
	if(this == &rhs)
		   return *this;
		this->ensure_load_factor(used+1);
		used = rhs.used;

		for (int i = 0; i < bins; ++i)
		{
		  for (LN* l=set[i]; l->next!=nullptr; l=l->next)
			set[i]->value=rhs.set[i]->value;
		}

		++mod_count;
		return *this;
}

template<class T>
bool HashSet<T>::operator == (const Set<T>& rhs) const {
 //write code here

			if (this == &rhs)
				return true;
			else if (used != rhs.size())
				return false;
			else
			{
	         for (int i=0; i<bins; i++)
	        	 for (LN* l=set[i]; l->next!=nullptr; l=l->next)
	        	 {
	        		 if (!rhs.contains(l->value))
	        		 {
	        			 return false;
	        		 }
	        	 }
			}
		return true;
}

template<class T>
bool HashSet<T>::operator != (const Set<T>& rhs) const {
 return !(*this == rhs);
}

template<class T>
bool HashSet<T>::operator <= (const Set<T>& rhs) const {
 //write code here
	 if(*this == rhs)
	  return true;

	 else
	 {
	 for (int i=0; i<bins; i++)
	  {
		 LN* p = set[i];

	  while(p->next!=nullptr)
	  {
	    if (!(rhs.contains(p->value)))
	    {
		 return false;
	    }
	     p=p->next;
	   }
	  }
	  }
	  return true;
}

template<class T>
bool HashSet<T>::operator < (const Set<T>& rhs) const {
 //write code here
	 return !(*this >= rhs);

}

template<class T>
bool HashSet<T>::operator >= (const Set<T>& rhs) const {
 //write code here
	if(*this == rhs)
	  return true;
	 else
	 {
	  //for (int i=0; i<rhs.bins; i++)
		 //for()
	  for (ics::Iterator<T>& p = rhs.abegin(); p != rhs.aend(); p++)
	  {
	   if (!(this->contains(*p)))
	   {
		return false;
	   }
	  }
	 }
	 return true;
}

template<class T>
bool HashSet<T>::operator > (const Set<T>& rhs) const {
 //write code here
	 return !(*this <= rhs);

}

template<class T>
std::ostream& operator << (std::ostream& outs, const HashSet<T>& s) {
 //write code here
		 if (s.empty())
		 {
		    outs << "set[]";
		 }
		 else
		 {
		   outs << "set[";
		   for (int i = 0; i < s.bins; i++)
		   {
			 for (auto l = s.set[i]; l != nullptr; l = l->next)
			  {
				outs << l->value;
				if (l->next->next!= nullptr)
					{
						outs << ",";
					}
					else
						break;
			}
		   }
		outs << "]";
		   }
		return outs;
}

//KLUDGE: memory-leak
template<class T>
auto HashSet<T>::abegin () const -> ics::Iterator<T>& {
 return *(new Iterator(const_cast<HashSet<T>*>(this),true));
}

//KLUDGE: memory-leak
template<class T>
auto HashSet<T>::aend () const -> ics::Iterator<T>& {
 return *(new Iterator(const_cast<HashSet<T>*>(this),false));
}

template<class T>
auto HashSet<T>::begin () const -> HashSet<T>::Iterator {
 return Iterator(const_cast<HashSet<T>*>(this),true);
}

template<class T>
auto HashSet<T>::end () const -> HashSet<T>::Iterator {
 return Iterator(const_cast<HashSet<T>*>(this),false);
}

template<class T>
int HashSet<T>::hash_compress (const T& element) const {
	return abs(hash(element) % bins);
}

template<class T>
void HashSet<T>::ensure_load_factor(int new_used) {

	double fac = new_used/bins;

	 if(fac > load_factor)
	 {
	  HashSet* hs = new HashSet(bins * 2, hash, fac);
	  for(int i = 0; i < bins; i++)
	  {
	   while(set[i]->next != nullptr)
	   {
	    LN* temp = set[i];
	    set[i] = temp->next;
	    temp->next = nullptr;
	    int hc = hs->hash_compress(temp->value);
	    temp->next = hs->set[hc];
	    hs->set[hc] = temp;
	    hs->used++;
	   }
	  }
	  auto hs_used=hs->used;
	  auto hs_bins=hs->bins;
	  this->delete_hash_table(set, bins);
	  this->used = hs_used;
     this->bins = hs_bins;
	  this->set = copy_hash_table(hs->set, hs_bins);
	 }
}

template<class T>
typename HashSet<T>::LN* HashSet<T>::find_element (int bin, const T& element) const {
 for (LN* c = set[bin]; c->next!=nullptr; c=c->next)
   if (element == c->value)
     return c;

 return nullptr;
}

template<class T>
typename HashSet<T>::LN* HashSet<T>::copy_list (LN* l) const {
 if (l == nullptr)
   return nullptr;
 else
   return new LN(l->value, copy_list(l->next));
}

template<class T>
typename HashSet<T>::LN** HashSet<T>::copy_hash_table (LN** ht, int bins) const {
	HashSet* temp = new HashSet(bins, this->hash, this->load_factor);

	 for(int i = 0; i < bins; i++)
	 {
	  temp->set[i] = copy_list(ht[i]);
	 }

	 return temp->set;
}

template<class T>
void HashSet<T>::delete_hash_table (LN**& ht, int bins) {
		for (int i=0; i<bins; i++)
			{
			 while(ht[i] != nullptr)
			 {
			  LN* p_el = ht[i];

			  ht[i] = ht[i]->next;
			  p_el->next = nullptr;
			  delete p_el;
			  p_el = ht[i];
			 }
			}

			delete [] ht;
			used=0;
	}


template<class T>
void HashSet<T>::Iterator::advance_cursors(){
 //write code here
			current.second = current.second->next;

			if (current.second->next!=nullptr)
				return;

			while(current.second->next==nullptr)
			{current.first++;
		         if (current.first>=ref_set->bins)
		         {
		        	 current.first=-1;
		        	 current.second=nullptr;
		        	 return;
		         }

		         current.second=ref_set->set[current.first];
			}
}




template<class T>
HashSet<T>::Iterator::Iterator(HashSet<T>* fof, bool begin) : ref_set(fof) {
 //write code here
		if (begin)
		{
			current.second = ref_set->set[0];
			current.first = 0;
			while(current.second->next==nullptr)
			{
				current.first++;
		         if (current.first>=ref_set->bins)
		         {

		        	 current.first=-1;
		        	 current.second=nullptr;
		        	 break;
		         }

	          current.second=ref_set->set[current.first];
			}
		}
		else
		{
			current.first = -1;
			current.second = nullptr;
		}
	  expected_mod_count = ref_set->mod_count;
}


template<class T>
HashSet<T>::Iterator::Iterator(const Iterator& i) :
   current(i.current), ref_set(i.ref_set), expected_mod_count(i.expected_mod_count), can_erase(i.can_erase) {}

template<class T>
HashSet<T>::Iterator::~Iterator() {}

template<class T>
T HashSet<T>::Iterator::erase() {
	//ERRRRRRROOOOOOOOOORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
	//std::cout<<"e m c"<<expected_mod_count<<std::endl;
	//std::cout<<"f s mc "<<ref_set->mod_count<<std::endl;

 if (expected_mod_count != ref_set->mod_count)
   throw ConcurrentModificationError("HashSet::Iterator::erase");
 if (!can_erase)
   throw CannotEraseError("HashSet::Iterator::erase Iterator cursor already erased");
 if (current.second == nullptr)
   throw CannotEraseError("HashSet::Iterator::erase Iterator cursor beyond data structure");

 	can_erase = false;
   LN* del=current.second;
   T toReturn = del->value;
   advance_cursors();
   ref_set->erase(del->value);
   expected_mod_count = ref_set->mod_count;
   return toReturn;
}

template<class T>
std::string HashSet<T>::Iterator::str() const {
	std::ostringstream answer;
		answer << ref_set->str() << "(current=" << current.first << "/"
				<< current.second << ",expected_mod_count=" << expected_mod_count
				<< ",can_erase=" << can_erase << ")";
		return answer.str();
}

template<class T>
const ics::Iterator<T>& HashSet<T>::Iterator::operator ++ () {
 if (expected_mod_count != ref_set->mod_count)
   throw ConcurrentModificationError("HashSet::Iterator::operator ++");
	if (can_erase && current.second!=nullptr && current.first!=-1)
	{
		advance_cursors();
	}
	can_erase = true;
	return *this;
}

//KLUDGE: creates garbage! (can return local value!)
template<class T>
const ics::Iterator<T>& HashSet<T>::Iterator::operator ++ (int) {
 if (expected_mod_count != ref_set->mod_count)
   throw ConcurrentModificationError("HashSet::Iterator::operator ++(int)");

 	  Iterator* toReturn = new Iterator(this->ref_set,can_erase);

 	  if (can_erase && current.second!=nullptr && current.first!=-1)
 	  	advance_cursors();
 	  can_erase = true;
 	  return *toReturn;
 }

template<class T>
bool HashSet<T>::Iterator::operator == (const ics::Iterator<T>& rhs) const {
 const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
 if (rhsASI == 0)
   throw IteratorTypeError("HashSet::Iterator::operator ==");
 if (expected_mod_count != ref_set->mod_count)
   throw ConcurrentModificationError("HashSet::Iterator::operator ==");
 if (ref_set != rhsASI->ref_set)
   throw ComparingDifferentIteratorsError("HashSet::Iterator::operator ==");

 if (current.first==rhsASI->current.first && current.second==rhsASI->current.second)
 		return true;
 	else
 		return false;
 //  //write code here
}


template<class T>
bool HashSet<T>::Iterator::operator != (const ics::Iterator<T>& rhs) const {
 const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
 if (rhsASI == 0)
   throw IteratorTypeError("HashSet::Iterator::operator !=");
 if (expected_mod_count != ref_set->mod_count)
   throw ConcurrentModificationError("HashSet::Iterator::operator !=");
 if (ref_set != rhsASI->ref_set)
   throw ComparingDifferentIteratorsError("HashSet::Iterator::operator !=");
 	return 	(current.first!=rhsASI->current.first);

 //write code here
}

template<class T>
T& HashSet<T>::Iterator::operator *() const {
 if (expected_mod_count !=
     ref_set->mod_count)
   throw ConcurrentModificationError("HashSet::Iterator::operator *");
 if (!can_erase || current.second == nullptr)
   throw IteratorPositionIllegal("HashSet::Iterator::operator * Iterator illegal: exhausted");
 return current.second->value;

 //write code here
}

template<class T>
T* HashSet<T>::Iterator::operator ->() const {
 if (expected_mod_count !=
     ref_set->mod_count)
   throw ConcurrentModificationError("HashSet::Iterator::operator *");
 if (!can_erase || current.second == nullptr)
   throw IteratorPositionIllegal("HashSet::Iterator::operator * Iterator illegal: exhausted");
 return &(current.second->value);

 //write code here
}

}

#endif /* HASH_SET_HPP_ */
