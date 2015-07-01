//
//#ifndef HASH_MAP_HPP_
//#define HASH_MAP_HPP_
//
//#include <string>
//#include <iostream>
//#include <sstream>
//#include <initializer_list>
//#include "ics_exceptions.hpp"
//#include "iterator.hpp"
//#include "pair.hpp"
//#include "map.hpp"
//#include "array_queue.hpp"   //For traversal
//namespace ics {
//
//template<class KEY, class T> class HashMap: public Map<KEY, T> {
//public:
//	typedef ics::pair<KEY, T> Entry;
//	HashMap() = delete;
//	HashMap(int (*ahash)(const KEY& k), double the_load_factor = 1.0);
//	HashMap(int initial_bins, int (*ahash)(const KEY& k),
//			double the_load_factor = 1.0);
//	HashMap(const HashMap<KEY, T>& to_copy);
//	//HashMap(std::initializer_list<Entry> il, int (*ahash)(const KEY& k), double the_load_factor = 1.0);
//	HashMap(ics::Iterator<Entry>& start, const ics::Iterator<Entry>& stop,
//			int (*ahash)(const KEY& k), double the_load_factor = 1.0);
//	virtual ~HashMap();
//
//	virtual bool empty() const;
//	virtual int size() const;
//	virtual bool has_key(const KEY& key) const;
//	virtual bool has_value(const T& value) const;
//	virtual std::string str() const;
//
//	virtual T put(const KEY& key, const T& value);
//	virtual T erase(const KEY& key);
//	virtual void clear();
//
//	virtual int put(ics::Iterator<Entry>& start,
//			const ics::Iterator<Entry>& stop);
//
//	virtual T& operator [](const KEY&);
//	virtual const T& operator [](const KEY&) const;
//	virtual HashMap<KEY, T>& operator =(const HashMap<KEY, T>& rhs);
//	virtual bool operator ==(const Map<KEY, T>& rhs) const;
//	virtual bool operator !=(const Map<KEY, T>& rhs) const;
//
//	template<class KEY2, class T2>
//	friend std::ostream& operator <<(std::ostream& outs,
//			const HashMap<KEY2, T2>& m);
//
//	virtual ics::Iterator<Entry>& abegin() const;
//	virtual ics::Iterator<Entry>& aend() const;
//
//private:
//	class LN;
//
//public:
//	class Iterator: public ics::Iterator<Entry> {
//	public:
//		//KLUDGE should be callable only in begin/end
//		Iterator(HashMap<KEY, T>* fof, bool begin);
//		Iterator(const Iterator& i);
//		virtual ~Iterator() {
//		}
//		virtual Entry erase();
//		virtual std::string str() const;
//		virtual const ics::Iterator<Entry>& operator ++();
//		virtual const ics::Iterator<Entry>& operator ++(int);
//		virtual bool operator ==(const ics::Iterator<Entry>& rhs) const;
//		virtual bool operator !=(const ics::Iterator<Entry>& rhs) const;
//		virtual Entry& operator *() const;
//		virtual Entry* operator ->() const;
//	private:
//		ics::pair<int, LN*> current; //Bin Index and Cursor; stop: LN* == nullptr
//		HashMap<KEY, T>* ref_map;
//		int expected_mod_count;
//		bool can_erase = true;
//		void advance_cursors();
//	};
//
//	virtual Iterator begin() const;
//	virtual Iterator end() const;
//	//KLUDGE: define
//	//virtual ics::Iterator<KEY>&  begin_key   () const;
//	//virtual ics::Iterator<KEY>&  end_key     () const;
//	//virtual ics::Iterator<T>&    begin_value () const;
//	//virtual ics::Iterator<T>&    end_value   () const;
//
//private:
//	class LN {
//	public:
//		LN() :
//				next(nullptr) {
//		}
//		LN(const LN& ln) :
//				value(ln.value), next(ln.next) {
//		}
//		LN(Entry v, LN* n = nullptr) :
//				value(v), next(n) {
//		}
//
//		Entry value;
//		LN* next;
//	};
//
//	LN** map = nullptr;
//	int (*hash)(const KEY& k);
//	double load_factor;    //used/bins <= load_factor
//	int bins = 1; //# bins in array
//	int used = 0; //# of key->value pairs in the hash table
//	int mod_count = 0; //For sensing concurrent modification
//	int hash_compress(const KEY& key) const;
//	void ensure_load_factor(int new_used);
//	LN* find_key(int bin, const KEY& key) const;
//	bool find_value(const T& value) const;
//	LN* copy_list(LN* l) const;
//	LN** copy_hash_table(LN** ht, int bins) const;
//	void delete_hash_table(LN**& ht, int bins);
//};
////
//template<class KEY, class T>
//HashMap<KEY, T>::HashMap(int (*ahash)(const KEY& k), double the_load_factor) :
//		hash(ahash), load_factor(the_load_factor) {
//
//	map = new LN*[bins];
//
//	LN* newNode = new LN();
//	newNode->value.second = T();
//	newNode->next = nullptr;
//
//	for (int i = 0; i < bins; i++) {
//		map[i] = newNode;
//	}
//
//}
//
//template<class KEY,class T>
//HashMap<KEY,T>::HashMap(int initial_bins, int (*ahash)(const KEY& k), double the_load_factor) : bins(initial_bins), hash(ahash), load_factor(the_load_factor) {
//
// map = new LN*[bins];
// for(int i = 0; i < bins; i++)
// {
//  map[i] = new LN();
// }
//}
//
//template<class KEY,class T>
//HashMap<KEY,T>::HashMap(const HashMap<KEY,T>& to_copy) : hash(to_copy.hash), load_factor(to_copy.load_factor), bins(to_copy.bins), used(to_copy.used) {
// this->map = copy_hash_table(to_copy.map, bins);
//}
//
//template<class KEY, class T>
//HashMap<KEY, T>::HashMap(ics::Iterator<Entry>& start,
//		const ics::Iterator<Entry>& stop, int (*ahash)(const KEY& k),
//		double the_load_factor) :
//		hash(ahash), load_factor(the_load_factor) {
//
//	map = new LN*[bins];
//	 for(int i = 0; i < bins; i++)
//	 {
//	  map[i] = new LN();
//	 }
//
//	this->put(start,stop);
//}
//
////KLUDGE: Not usable yet
////template<class KEY,class T>
////HashMap<KEY,T>::HashMap(std::initializer_list<Entry> i,int (*ahash)(const KEY& k), double the_load_factor = 1.0) {
////  map = new LN*[bins];
////  put(il.abegin(),il.aend());
////}
//
//template<class KEY, class T>
//HashMap<KEY, T>::~HashMap() {
//	delete_hash_table(map,bins);
//	  //clear();
//	//delete[] map;
//}
//
//template<class KEY, class T>
//inline bool HashMap<KEY, T>::empty() const {
//	return (used == 0);
//}
//
//template<class KEY, class T>
//int HashMap<KEY, T>::size() const {
//	return used;
//}
//
//template<class KEY, class T>
//bool HashMap<KEY, T>::has_key(const KEY& key) const {
//	if (used ==0)
//		return false;
//	for (int i=0; i<bins; i++)
//	{
//		if (find_key(i, key) != nullptr)
//			return true;
//	}
//
//	return false;
//}
//
//template<class KEY, class T>
//bool HashMap<KEY, T>::has_value(const T& value) const {
//	return find_value(value);
//}
//
//template<class KEY, class T>
//std::string HashMap<KEY, T>::str() const {
//
//	std::ostringstream outputStr;
//	if (this->size() == 0)
//	{
//		outputStr << "map[]";
//		return outputStr.str();
//	}
//
//	for (int i = 0; i < bins; i++) {
//		outputStr << "\nbin[" << i << "]: ";
//		for (LN* l = map[i]; l != nullptr; l = l->next)
//		{
//			outputStr << "pair[" << l->value.first << "," << l->value.second
//					<< "]" << (l->next != nullptr ? " -> " : " -> #");
//
//		}
//	}
//	return outputStr.str();
//}
//
//template<class KEY, class T>
//T HashMap<KEY, T>::put(const KEY& key, const T& value) {
//
//   if (has_key(key)) {
//	   T temp;
//	   for (int i = 0; i < bins; i++)
//	   		for (LN* l = map[i]; l->next != nullptr; l = l->next) {
//	   			if (l->value.first == key)
//	   				{
//	   				 temp=l->value.second;
//	   			     l->value.second=value;
//	   				 return temp;
//	   				}
//	   			}
//	}
//
//
//	LN* newNode = new LN;
//	Entry kv(key, value);
//	newNode->value = kv;
//	newNode->next = nullptr;
//
//    this->ensure_load_factor(used + 1);
//	int index = hash_compress(key);
//
//	LN*& n = map[index];
//	newNode->next = n;
//	n = newNode;
//
//	used++;
//	mod_count++;
//	return newNode->value.second;
//}
//
//template<class KEY, class T>
//T HashMap<KEY, T>::erase(const KEY& key) {
//
//	T toReturn;
//	if (has_key(key)) {
//		used--;
//		int index = hash_compress(key);
//
//		LN* head = map[index];
//
//		if (map[index]->value.first == key)
//		{
//
//		   toReturn = head->value.second;
//
//			map[index]=head->next;
//
//		    head->next=nullptr;
//			delete head;
//		}
//		else
//		{
//		    LN* l;
//			for(LN* p = head; p->next!= nullptr;)
//			{
//				l = p;
//				p = p->next;
//				if (p->value.first == key)
//				{
//					toReturn = p->value.second;
//					l->next = p->next;
//					p->next = nullptr;
//					delete p;
//					break;
//				}
//			}
//		}
//	}
//	else
//		{
//		 std::ostringstream answer;
//		  answer << "HashMap::erase: key(" << key << ") not in Map";
//		  throw KeyError(answer.str());
//		}
//	mod_count++;
//	return toReturn;
//
//}
//
//template<class KEY, class T>
//void HashMap<KEY, T>::clear() {
//	for (int i=0; i<bins; i++)
//		{
//
//		while(map[i]->next != nullptr)
//		 {
//			LN* p_el = map[i];
//		    map[i] = map[i]->next;
//		    p_el->next = nullptr;
//		    delete p_el;
//		 }
//		}
//	 used = 0;
//	 mod_count++;
//}
//
//template<class KEY, class T>
//int HashMap<KEY, T>::put(ics::Iterator<Entry>& start,
//		const ics::Iterator<Entry>& stop) {
//	    int count = 0;
//
//		for (; start != stop; ++start)
//		{
//			++count;
//		//	std::cout << start->first << " " << start->second << std::endl;
//			//break;
//			this->put(start->first, start->second);
//		}
//		return count;
//}
//
//template<class KEY, class T>
//T& HashMap<KEY, T>::operator [](const KEY& key) {
//	for (int i = 0; i < bins; i++)
//		for (LN* l = map[i]; l ->next!= nullptr; l = l->next)
//		  {
//		   if (l->value.first == key)
//			   return l->value.second;
//		  }
//
//	this->ensure_load_factor(used+1);
//	int index = hash_compress(key);
//    Entry entry(key, T());
//	map[index] = new LN(entry, map[index]);
//    used++;
//	mod_count++;
//    return map[index]->value.second;
//}
//
//template<class KEY, class T>
//const T& HashMap<KEY, T>::operator [](const KEY& key) const {
//	for (int i = 0; i < bins; i++)
//			for (LN* l = map[i]; l != nullptr; l = l->next) {
//				if (l->value.first == key)
//					return l->value.second;
//				}
//
//		std::ostringstream answer;
//		answer << "HashMap::operator []: key(" << key << ") not in Map";
//		throw KeyError(answer.str());
//}
//
//template<class KEY, class T>
//bool HashMap<KEY, T>::operator ==(const Map<KEY, T>& rhs) const {
//
//		if (this == &rhs)
//			return true;
//		else if (used != rhs.size())
//			return false;
//		else
//		{
//         for (int i=0; i<bins; i++)
//        	 for (LN* l=map[i]; l->next!=nullptr; l=l->next)
//        	 {
//        		 if (!rhs.has_key(l->value.first)||l->value.second != rhs[l->value.first])
//        		 {
//        			 return false;
//        		 }
//        	 }
//		}
//	return true;
//}
//
//template<class KEY, class T>
//HashMap<KEY, T>& HashMap<KEY, T>::operator = (const HashMap<KEY, T>& rhs) {
//	if(this == &rhs)
//	   return *this;
//	this->ensure_load_factor(used+1);
//	used = rhs.used;
//
//	for (int i = 0; i < bins; ++i)
//	{
//		for (LN* l=map[i]; l->next!=nullptr; l=l->next)
//		map[i]->value=rhs.map[i]->value;
//	}
//
//	++mod_count;
//	return *this;
//}
//
//template<class KEY, class T>
//bool HashMap<KEY, T>::operator !=(const Map<KEY, T>& rhs) const {
//	return !(*this == rhs);
//}
//
//template<class KEY, class T>
//std::ostream& operator <<(std::ostream& outs, const HashMap<KEY, T>& m) {
//	 if (m.empty())
//	 {
//	    outs << "map[]";
//	 }
//	 else
//	 {
//	   outs << "map[";
//	   for (int i = 0; i < m.bins; i++)
//	   {
//		 for (auto l = m.map[i]; l != nullptr; l = l->next)
//		  {
//			outs << l->value.first << "->" << l->value.second;
//			if (l->next->value.second != T())
//				{
//					outs << ",";
//				}
//				else
//					break;
//		}
//	   }
//	outs << "]";
//	   }
//	return outs;
//}
//
////KLUDGE: memory-leak
//template<class KEY, class T>
//auto HashMap<KEY, T>::abegin() const -> ics::Iterator<Entry>& {
//	return *(new Iterator(const_cast<HashMap<KEY, T>*>(this), true));
//}
//
////KLUDGE: memory-leak
//template<class KEY, class T>
//auto HashMap<KEY, T>::aend() const -> ics::Iterator<Entry>& {
//	return *(new Iterator(const_cast<HashMap<KEY, T>*>(this), false));
//}
//
//template<class KEY, class T>
//auto HashMap<KEY, T>::begin() const -> HashMap<KEY,T>::Iterator {
//	return Iterator(const_cast<HashMap<KEY, T>*>(this), true);
//}
//
//template<class KEY, class T>
//auto HashMap<KEY, T>::end() const -> HashMap<KEY,T>::Iterator {
//	return Iterator(const_cast<HashMap<KEY, T>*>(this), false);
//}
//
//template<class KEY, class T>
//int HashMap<KEY, T>::hash_compress(const KEY& key) const {
//	return abs(hash(key)) % bins;
//}
//
//template<class KEY,class T>
//void HashMap<KEY,T>::ensure_load_factor(int new_used) {
//
// double fac = new_used/bins;
// if(fac > load_factor)
// {
//  //  bins=bins*2;
//  HashMap* hm = new HashMap(bins*2, hash, fac);
//  for(int i = 0; i < bins; i++)
//  {
//   while(map[i]->next != nullptr)
//   {
//    LN* temp = map[i];
//    map[i] = temp->next;
//    temp->next = nullptr;
//    int hc = hm->hash_compress(temp->value.first);
//    temp->next = hm->map[hc];
//    hm->map[hc] = temp;
//    hm->used++;
//   }
//  }
//  auto hm_used=hm->used;
//	  auto hm_bins=hm->bins;
//	  this->delete_hash_table(map, bins);
//	  this->used = hm_used;
//      this->bins = hm_bins;
//	  this->map = copy_hash_table(hm->map, hm_bins);
// }
//}
//
//template<class KEY, class T>
//typename HashMap<KEY, T>::LN* HashMap<KEY, T>::find_key(int bin,
//		const KEY& key) const {
//
//	LN* p = map[bin];
//	if (p == nullptr) {
//		return nullptr;
//	}
//	for (; p->next != nullptr; p = p->next) {
//		if (p->value.first == key)
//		{
//			return p;
//		}
//	}
//	return nullptr;
//}
//template<class KEY, class T>
//bool HashMap<KEY, T>::find_value(const T& value) const {
//
//	for (int i = 0; i < bins; i++)
//		for (LN* l = map[i]; l->next != nullptr; l = l->next) {
//			if (l->value.second == value) {
//				return true;
//			}
//		}
//
//	return false;
//}
//
//template<class KEY, class T>
//typename HashMap<KEY, T>::LN* HashMap<KEY, T>::copy_list(LN* l) const {
//
//	if (l == nullptr)
//		return nullptr;
//	LN* answer = new LN(l->value);
//	LN* rear = answer;
//	for (LN* c = l->next; c != nullptr; c = c->next)
//		rear = rear->next = new LN(c->value);
//	return answer;
//
//}
//
//template<class KEY, class T>
//typename HashMap<KEY, T>::LN** HashMap<KEY, T>::copy_hash_table(LN** ht,
//		int bins) const {
//
//	LN** newMap = new LN*[bins]();
//
//	for (int i = 0; i < bins; i++) {
//		newMap[i] = copy_list(ht[i]);
//	}
//
//	return newMap;
//}
//
//template<class KEY, class T>
//void HashMap<KEY, T>::delete_hash_table(LN**& ht, int bins) {
//
//	for (int i=0; i<bins; i++)
//		{
//		 while(ht[i] != nullptr)
//		 {
//		  LN* p_el = ht[i];
//		  ht[i] = ht[i]->next;
//		  p_el->next = nullptr;
//		  delete p_el;
//		  p_el = ht[i];
//		 }
//		}
//		used=0;
//
//		delete [] ht;
//}
//
//template<class KEY, class T>
//void HashMap<KEY, T>::Iterator::advance_cursors() {
//
//		current.second = current.second->next;
//
//		if (current.second->next!=nullptr)
//			return;
//
//		while(current.second->next==nullptr)
//		{current.first++;
//	         if (current.first>=ref_map->bins)
//	         {
//	        	 current.first=-1;
//	        	 current.second=nullptr;
//	        	 return;
//	         }
//
//	         current.second=ref_map->map[current.first];
//		}
//	//write code here
//}
//
//template<class KEY, class T>
//HashMap<KEY, T>::Iterator::Iterator(HashMap<KEY, T>* fof, bool begin) :
//		ref_map(fof) {
//	//write code here
//	if (begin)
//	{
//		current.second = ref_map->map[0];
//		current.first = 0;
//		while(current.second->next==nullptr)
//		{
//			current.first++;
//	         if (current.first>=ref_map->bins)
//	         {
//
//	        	 current.first=-1;
//	        	 current.second=nullptr;
//	        	 break;
//	         }
//
//          current.second=ref_map->map[current.first];
//		}
//	}
//	else
//	{
//		current.first = -1;
//		current.second = nullptr;
//	}
//  expected_mod_count = ref_map->mod_count;
//
//}
//
//template<class KEY, class T>
//HashMap<KEY, T>::Iterator::Iterator(const Iterator& i) :
//		current(i.current), ref_map(i.ref_map), expected_mod_count(
//				i.expected_mod_count), can_erase(i.can_erase) {
//}
//
////KLUDGE: must define in .hpp
////template<class KEY,class T>
////HashMap<KEY,T>::Iterator::~Iterator() {}
//
//template<class KEY, class T>
//auto HashMap<KEY, T>::Iterator::erase() -> Entry {
//	if (expected_mod_count != ref_map->mod_count)
//		throw ConcurrentModificationError("HashMap::Iterator::erase");
//	if (!can_erase)
//		throw CannotEraseError(
//				"HashMap::Iterator::erase Iterator cursor already erased");
//	if (current.second == nullptr)
//		throw CannotEraseError(
//				"HashMap::Iterator::erase Iterator cursor beyond data structure");
//	//write code here
//	can_erase = false;
//  LN* del=current.second;
//  Entry toReturn = del->value;
//	advance_cursors();
//  ref_map->erase(del->value.first);
//
//   expected_mod_count = ref_map->mod_count;
//   return toReturn;
//
//
//}
//
//template<class KEY, class T>
//std::string HashMap<KEY, T>::Iterator::str() const {
//	std::ostringstream answer;
//	answer << ref_map->str() << "(current=" << current.first << "/"
//			<< current.second << ",expected_mod_count=" << expected_mod_count
//			<< ",can_erase=" << can_erase << ")";
//	return answer.str();
//}
//
////KLUDGE: cannot use Entry
//template<class KEY, class T>
//auto HashMap<KEY, T>::Iterator::operator ++() -> const ics::Iterator<ics::pair<KEY,T>>& {
//	if (expected_mod_count != ref_map->mod_count)
//		throw ConcurrentModificationError("HashMap::Iterator::operator ++");
//
//	if (can_erase && current.second!=nullptr && current.first!=-1)
//	{
//		advance_cursors();
//	}
//	can_erase = true;
//	return *this;
//
//}
//
////KLUDGE: creates garbage! (can return local value!)
//template<class KEY, class T>
//auto HashMap<KEY, T>::Iterator::operator ++(
//		int) -> const ics::Iterator<ics::pair<KEY,T>>& {
//	if (expected_mod_count != ref_map->mod_count)
//		throw ConcurrentModificationError(
//				"HashMap::Iterator::operator ++(int)");
//
//	  Iterator* toReturn = new Iterator(this->ref_map,can_erase);
//
//	  if (can_erase && current.second!=nullptr && current.first!=-1)
//	  {
//		  advance_cursors();
//	  }
//
//	  can_erase = true;
//	  return *toReturn;
//}
//
//template<class KEY, class T>
//bool HashMap<KEY, T>::Iterator::operator ==(
//		const ics::Iterator<Entry>& rhs) const {
//	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//	if (rhsASI == 0)
//		throw IteratorTypeError("HashMap::Iterator::operator ==");
//	if (expected_mod_count != ref_map->mod_count)
//		throw ConcurrentModificationError("HashMap::Iterator::operator ==");
//	if (ref_map != rhsASI->ref_map)
//		throw ComparingDifferentIteratorsError(
//				"HashMap::Iterator::operator ==");
//
//	if (current.first==rhsASI->current.first && current.second==rhsASI->current.second)
//		return true;
//	else
//		return false;
//}
//
//template<class KEY, class T>
//bool HashMap<KEY, T>::Iterator::operator !=(
//		const ics::Iterator<Entry>& rhs) const {
//	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//	if (rhsASI == 0)
//		throw IteratorTypeError("HashMap::Iterator::operator !=");
//	if (expected_mod_count != ref_map->mod_count)
//		throw ConcurrentModificationError("HashMap::Iterator::operator !=");
//	if (ref_map != rhsASI->ref_map)
//		throw ComparingDifferentIteratorsError(
//				"HashMap::Iterator::operator !=");
//
//	//write code hereeck
//	//std::cout<<"check"<<std::endl;
//	return 	(current.first!=rhsASI->current.first);
//
//}
//
//template<class KEY, class T>
//ics::pair<KEY, T>& HashMap<KEY, T>::Iterator::operator *() const {
//	if (expected_mod_count != ref_map->mod_count)
//		throw ConcurrentModificationError("HashMap::Iterator::operator *");
//	if (!can_erase || current.second == nullptr)
//		throw IteratorPositionIllegal(
//				"HashMap::Iterator::operator * Iterator illegal: exhausted");
//	//std::cout<<"check****"<<std::endl;
//
//	return current.second->value;
////	return ref_map->map[current.first]->value;
//}
//
//template<class KEY, class T>
//ics::pair<KEY, T>* HashMap<KEY, T>::Iterator::operator ->() const {
//	if (expected_mod_count != ref_map->mod_count)
//		throw ConcurrentModificationError("HashMap::Iterator::operator *");
//	if (!can_erase || current.second == nullptr)
//		throw IteratorPositionIllegal(
//				"HashMap::Iterator::operator -> Iterator illegal: exhausted");
//	//std::cout<<"check->->"<<std::endl;
//
//	//write code here
//
//	return &(current.second->value);
////	  return &(ref_map->map[current.first]->value);
// }
//}

#endif /* HASH_MAP_HPP_ */
