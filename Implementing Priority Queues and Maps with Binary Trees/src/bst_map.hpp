///*
// SARGIS DUDAKLYAN ID: 38671346, Program 2
//*/


#ifndef BST_MAP_HPP_
#define BST_MAP_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "iterator.hpp"
#include "pair.hpp"
#include "map.hpp"
#include "array_queue.hpp"   //For traversal
namespace ics {

template<class KEY, class T> class BSTMap: public Map<KEY, T> {
public:
	typedef ics::pair<KEY, T> Entry;
	BSTMap();
	BSTMap(const BSTMap<KEY, T>& to_copy);
	//BSTMap(std::initializer_list<Entry> il);
	BSTMap(ics::Iterator<Entry>& start, const ics::Iterator<Entry>& stop);
	virtual ~BSTMap();

	virtual bool empty() const;
	virtual int size() const;
	virtual bool has_key(const KEY& key) const;
	virtual bool has_value(const T& value) const;
	virtual std::string str() const;

	virtual T put(const KEY& key, const T& value);
	virtual T erase(const KEY& key);
	virtual void clear();

	virtual int put(ics::Iterator<Entry>& start,
			const ics::Iterator<Entry>& stop);

	virtual T& operator [](const KEY&);
	virtual const T& operator [](const KEY&) const;
	virtual BSTMap<KEY, T>& operator =(const BSTMap<KEY, T>& rhs);
	virtual bool operator ==(const Map<KEY, T>& rhs) const;
	virtual bool operator !=(const Map<KEY, T>& rhs) const;

	template<class KEY2, class T2>
	friend std::ostream& operator <<(std::ostream& outs,
			const Map<KEY2, T2>& m);

	virtual ics::Iterator<Entry>& abegin() const;
	virtual ics::Iterator<Entry>& aend() const;

private:
	class TN;

public:
	class Iterator: public ics::Iterator<Entry> {
	public:
		//KLUDGE should be callable only in begin/end
		Iterator(BSTMap<KEY, T>* fof, bool begin);
		Iterator(const Iterator& i);
		virtual ~Iterator() {
		}
		virtual Entry erase();
		virtual std::string str() const;
		virtual const ics::Iterator<Entry>& operator ++();
		virtual const ics::Iterator<Entry>& operator ++(int);
		virtual bool operator ==(const ics::Iterator<Entry>& rhs) const;
		virtual bool operator !=(const ics::Iterator<Entry>& rhs) const;
		virtual Entry& operator *() const;
		virtual Entry* operator ->() const;
	private:
		ics::ArrayQueue<Entry> it;          //Iterator (as Stack) for Map
		BSTMap<KEY, T>* ref_map;
		int expected_mod_count;
		bool can_erase = true;
	};

	virtual Iterator begin() const;
	virtual Iterator end() const;
	//KLUDGE: define
	//virtual ics::Iterator<KEY>&  begin_key   () const;
	//virtual ics::Iterator<KEY>&  end_key     () const;
	//virtual ics::Iterator<T>&    begin_value () const;
	//virtual ics::Iterator<T>&    end_value   () const;

private:
	class TN {
	public:
		TN() :
				left(nullptr), right(nullptr) {
		}
		TN(const TN& tn) :
				value(tn.value), left(tn.left), right(tn.right) {
		}
		TN(Entry v, TN* l = nullptr, TN* r = nullptr) :
				value(v), left(l), right(r) {
		}

		Entry value;
		TN* left;
		TN* right;
	};

	TN* map = nullptr;
	int used = 0; //Amount of array used
	int mod_count = 0; //For sensing concurrent modification
	TN* find_key(TN* root, const KEY& key) const;
	bool find_value(TN* root, const T& value) const;
	T& insert(TN*& root, const KEY& key, const T& value);
	ics::pair<KEY, T> remove_closest(TN*& root);
	T remove(TN*& root, const KEY& key);
	TN* copy(TN* root) const;
	void copy_to_queue(TN* root, ArrayQueue<Entry>& q) const;
	void delete_BST(TN*& root);
	bool equals(TN* root, const Map<KEY, T>& other) const;
	std::string string_rotated(TN* root, std::string indent) const;
};

template<class KEY, class T>
BSTMap<KEY, T>::BSTMap() {
}

template<class KEY, class T>
BSTMap<KEY, T>::BSTMap(const BSTMap<KEY, T>& to_copy) : used(to_copy.used)
{
	map = copy(to_copy.map);
}

template<class KEY, class T>
BSTMap<KEY, T>::BSTMap(ics::Iterator<Entry>& start,
		const ics::Iterator<Entry>& stop) {


	put(start, stop);
}

//KLUDGE: Not usable yet
//template<class KEY,class T>
//BSTMap<KEY,T>::BSTMap(std::initializer_list<Entry> i) {
//  map = new T[length];
//  put(il.abegin(),il.aend());
//}

template<class KEY, class T>
BSTMap<KEY, T>::~BSTMap() {

	delete_BST(map);
}

template<class KEY, class T>
inline bool BSTMap<KEY, T>::empty() const {

	if (size() == 0)
		return true;
	else
		return false;
}

template<class KEY, class T>
int BSTMap<KEY, T>::size() const {

	return used;
}

template<class KEY, class T>
bool BSTMap<KEY, T>::has_key(const KEY& element) const {

	TN* p = map;

	return (find_key(p, element) != nullptr);
}

template<class KEY, class T>
bool BSTMap<KEY, T>::has_value(const T& element) const {

	TN* p = map;

	return find_value(p, element);
}

template<class KEY, class T>
std::string BSTMap<KEY, T>::str() const {


	std::ostringstream outputStr;

	ics::ArrayQueue<Entry> q;
	TN* root = map;
	copy_to_queue(root, q);

	if (q.empty()) {
		outputStr << "map[]";
		return outputStr.str();
	} else {

		outputStr << "map[";

		while (!q.empty()) {
			TN* next = new TN;
			next->value = q.dequeue();
			outputStr << next->value.first;
			outputStr << "->";
			outputStr << next->value.second;
			if (!q.empty())
				outputStr << ",";

		}

		outputStr << "]";

		TN* p = map;
		outputStr << string_rotated(p,"\n");
	}
	return outputStr.str();

}

template<class KEY, class T>
T BSTMap<KEY, T>::put(const KEY& key, const T& value) {
	used++;
	  ++mod_count;

	  return insert(map, key, value);

}

template<class KEY, class T>
T BSTMap<KEY, T>::erase(const KEY& key) {

	if (has_key(key))
		used--;

	++mod_count;
	return remove(map, key);

}

template<class KEY, class T>
void BSTMap<KEY, T>::clear() {
	  ++mod_count;

	delete_BST(map);
}

template<class KEY, class T>
int BSTMap<KEY, T>::put(ics::Iterator<Entry>& start,
		const ics::Iterator<Entry>& stop) {
	int count = 0;
	for (; start != stop; ++start)
	{	++count;
		put(start->first, start->second);
	}
	return count;
}

template<class KEY, class T>
T& BSTMap<KEY, T>::operator [](const KEY& key) {

	for (TN* c = map; c != nullptr;c = (key < c->value.first ? c->left : c->right))
	{
		if (key == c->value.first)
			return c->value.second;
	}

	used++;
	  ++mod_count;

		return insert(map,key,T());

}

template<class KEY, class T>
const T& BSTMap<KEY, T>::operator [](const KEY& key) const {
	TN* temp;
	for (TN* c = map; c != nullptr;c = (key < c->value.first ? c->left : c->right)) {
		temp = c;
		if (key == c->value.first)
			return c->value.second;
	}

	std::ostringstream answer;
	answer << "BSTMap::operator []: key(" << key << ") not in Map";
	throw KeyError(answer.str());
}

template<class KEY, class T>
bool BSTMap<KEY, T>::operator ==(const Map<KEY, T>& rhs) const {

//	if(this == &rhs)
//		return true;
//	if (size() != rhs.size())
//	   return false;
//
//	ArrayQueue<Entry> q;
//	TN* root = map;
//	copy_to_queue(root, q);
//
//	while(!q.empty())
//	{
//		Entry next = q.dequeue();
//	    if (!rhs.has_value(next.second) || !rhs.has_key(next.first))
//	    	return false;
//	}
//
//	return true;
//
	TN* p = map;
	return equals (p,rhs);
}

template<class KEY, class T>
BSTMap<KEY, T>& BSTMap<KEY, T>::operator =(const BSTMap<KEY, T>& rhs) {
	 if (this == &rhs)
		 return *this;
	 else
	 {
		 ics::ArrayQueue<Entry> q;
		 TN* root = rhs.map;
		 copy_to_queue(root, q);
		 clear();
		 while (!q.empty())
		 {
			 	 //std::cout << "q.peeK() = " << q.peek().first << std::endl;
		 	    TN* next = new TN;
		 	    next->value = q.dequeue();
		 	    put(next->value.first, next->value.second);
		 }
	 }


	  ++mod_count;

	 return *this;

}

template<class KEY, class T>
bool BSTMap<KEY, T>::operator !=(const Map<KEY, T>& rhs) const {

	return !(*this == rhs);
}

template<class KEY, class T>
std::ostream& operator <<(std::ostream& outs, const BSTMap<KEY, T>& m)
{
	  if (m.empty())
	  {
	    outs << "map[]";
	    return outs;
	  }else{
	    outs << "map[";

	    for (auto kv : m)
	    {
	    	outs << kv.first << "->" << kv.second;

	    }
	    outs << "]";

	  return outs;

		}
}


//KLUDGE: memory-leak
template<class KEY, class T>
auto BSTMap<KEY, T>::abegin() const -> ics::Iterator<Entry>& {
	return *(new Iterator(const_cast<BSTMap<KEY, T>*>(this), true));
}

//KLUDGE: memory-leak
template<class KEY, class T>
auto BSTMap<KEY, T>::aend() const -> ics::Iterator<Entry>& {
	return *(new Iterator(const_cast<BSTMap<KEY, T>*>(this), false));
}

template<class KEY, class T>
auto BSTMap<KEY, T>::begin() const -> BSTMap<KEY,T>::Iterator {
	return Iterator(const_cast<BSTMap<KEY, T>*>(this), true);
}

template<class KEY, class T>
auto BSTMap<KEY, T>::end() const -> BSTMap<KEY,T>::Iterator {
	return Iterator(const_cast<BSTMap<KEY, T>*>(this), false);
}

template<class KEY, class T>
typename BSTMap<KEY, T>::TN* BSTMap<KEY, T>::find_key(TN* root,
		const KEY& key) const {

	for (TN* c = root; c != nullptr;
			c = (key < c->value.first ? c->left : c->right))
		if (key == c->value.first)
			return root;

	return nullptr;

}

template<class KEY, class T>
bool BSTMap<KEY, T>::find_value(TN* root, const T& value) const {
	if (root == nullptr) {
		return false;
	} else if (root != nullptr) {
		if (root->value.second == value)
			return true;
		else {
			return find_value(root->left, value)
					|| find_value(root->right, value);
		}
	}

	return false;
}

template<class KEY, class T>
T& BSTMap<KEY, T>::insert(TN*& root, const KEY& key, const T& value)
{
	if (root == nullptr) {
		Entry entry(key, value);
		root = new TN(entry);
		return root->value.second;

	} else {
		if (root->value.first == key) {
			T temp = root->value.second;
			root->value.second = value;
			used--;
			return temp;

		} else if (key > root->value.first) {
			insert(root->right, key, value);
		} else {
			insert(root->left, key, value);
		}
	}
}

template<class KEY, class T>
ics::pair<KEY, T> BSTMap<KEY, T>::remove_closest(TN*& root) {
	if (root->right == nullptr) {
		ics::pair<KEY, T> to_return = root->value;
		TN* to_delete = root;
		root = root->left;
		delete to_delete;
		return to_return;
	} else
		return remove_closest(root->right);
}

template<class KEY, class T>
T BSTMap<KEY, T>::remove(TN*& root, const KEY& key) {
	if (root == nullptr) {
		std::ostringstream answer;
		answer << "BSTMap::erase: key(" << key << ") not in Map";
		throw KeyError(answer.str());
	} else if (key == root->value.first) {
		T to_return = root->value.second;
		if (root->left == nullptr) {
			TN* to_delete = root;
			root = root->right;
			delete to_delete;
		} else if (root->right == nullptr) {
			TN* to_delete = root;
			root = root->left;
			delete to_delete;
		} else
			root->value = remove_closest(root->left);
		return to_return;
	} else
		return remove((key < root->value.first ? root->left : root->right), key);
}

template<class KEY, class T>
typename BSTMap<KEY, T>::TN* BSTMap<KEY, T>::copy(TN* root) const {

	TN* r;

	if (root == nullptr)
		return nullptr;
	else {
		r = new TN;
		r->value = root->value;
		r->left = copy(root->left);
		r->right = copy(root->right);
		return r;
	}

}

template<class KEY, class T>
void BSTMap<KEY, T>::copy_to_queue(TN* root, ArrayQueue<Entry>& q) const {

	if (root == nullptr)
		return;

	q.enqueue(root->value);

	if (root->left != nullptr)
		copy_to_queue(root->left,q);
	if (root->right != nullptr)
		copy_to_queue(root->right,q);
}

template<class KEY, class T>
void BSTMap<KEY, T>::delete_BST(TN*& root) {

	used = 0;
	while (root != nullptr)
		remove(root, root->value.first);
	mod_count++;
}

template<class KEY, class T>
bool BSTMap<KEY, T>::equals(TN* root, const Map<KEY, T>& other) const {

	if(this == &other)
		return true;
	if (size() != other.size())
	   return false;

	ArrayQueue<Entry> q;
	TN* r = map;
	copy_to_queue(r, q);

	while(!q.empty())
	{
		Entry next = q.dequeue();
	    if (!other.has_value(next.second) || !other.has_key(next.first))
	    	return false;
	}

	return true;
}

template<class KEY, class T>
std::string BSTMap<KEY, T>::string_rotated(TN* root, std::string indent) const {

//	std::stringstream a,b;
//	std::string a1,b1;
//	a<<root->value.first;
//	b<<root->value.second;
//	a1=a.str();
//	b1=b.str();

	std::ostringstream answer;

	if (root==nullptr)
	{
		return "";
	}
	else
	{
		answer << string_rotated(root->right, indent+"..")  << indent << root->value.first + "->" << root->value.second <<  string_rotated(root->left, indent+"..");
		return answer.str();
	}
}




template<class KEY, class T>
BSTMap<KEY, T>::Iterator::Iterator(BSTMap<KEY, T>* fof, bool begin) :
		it(), ref_map(fof) {
	if(begin)
	{
		ArrayQueue<Entry> q;
		ref_map->copy_to_queue(ref_map->map, it);
	}

	expected_mod_count = ref_map->mod_count;
}

template<class KEY, class T>
BSTMap<KEY, T>::Iterator::Iterator(const Iterator& i) :
		it(i.it), ref_map(i.ref_map), expected_mod_count(i.expected_mod_count), can_erase(
				i.can_erase) {
}

//KLUDGE: must define in .hpp
//template<class KEY,class T>
//BSTMap<KEY,T>::Iterator::~Iterator() {}

template<class KEY, class T>
auto BSTMap<KEY, T>::Iterator::erase() -> Entry {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::erase");
	if (!can_erase)
		throw CannotEraseError(
				"BSTMap::Iterator::erase Iterator cursor already erased");
	if (it.empty())
		throw CannotEraseError(
				"BSTMap::Iterator::erase Iterator cursor beyond data structure");

	can_erase = false;

 Entry to_return = it.dequeue();
 ref_map->remove(ref_map->map,to_return.first);
 ref_map->used--;

	expected_mod_count = ref_map->mod_count;
	return to_return;
}

template<class KEY, class T>
std::string BSTMap<KEY, T>::Iterator::str() const {
	std::ostringstream answer;
	  answer << ref_map->str() <<"(expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";

	  return answer.str();
}

//KLUDGE: cannot use Entry
template<class KEY, class T>
auto BSTMap<KEY, T>::Iterator::operator ++() -> const ics::Iterator<ics::pair<KEY,T>>& {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator ++");

	{
		it.dequeue();
	}


	can_erase = true;
	return *this;


}

//KLUDGE: creates garbage! (can return local value!)
template<class KEY, class T>
auto BSTMap<KEY, T>::Iterator::operator ++(int) -> const ics::Iterator<ics::pair<KEY,T>>& {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator ++(int)");

	Iterator* to_return = new Iterator(*this);
	if (can_erase && !it.empty())
		it.dequeue();
	can_erase = true;
	return *to_return;
}

template<class KEY, class T>
bool BSTMap<KEY, T>::Iterator::operator ==(
		const ics::Iterator<Entry>& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("BSTMap::Iterator::operator ==");
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator ==");
	if (ref_map != rhsASI->ref_map)
		throw ComparingDifferentIteratorsError("BSTMap::Iterator::operator ==");

	return it.size() == rhsASI->it.size();
}

template<class KEY, class T>
bool BSTMap<KEY, T>::Iterator::operator !=(
		const ics::Iterator<Entry>& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("BSTMap::Iterator::operator !=");
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator !=");
	if (ref_map != rhsASI->ref_map)
		throw ComparingDifferentIteratorsError("BSTMap::Iterator::operator !=");

	return it.size() != rhsASI->it.size();
}

template<class KEY, class T>
ics::pair<KEY, T>& BSTMap<KEY, T>::Iterator::operator *() const {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator *");
	if (!can_erase || it.empty())
		throw IteratorPositionIllegal(
				"BSTMap::Iterator::operator * Iterator illegal: exhausted");

	return it.peek();
}

template<class KEY, class T>
ics::pair<KEY, T>* BSTMap<KEY, T>::Iterator::operator ->() const {
	if (expected_mod_count != ref_map->mod_count)
		throw ConcurrentModificationError("BSTMap::Iterator::operator *");
	if (!can_erase || it.empty())
		throw IteratorPositionIllegal(
				"BSTMap::Iterator::operator -> Iterator illegal: exhausted");

	return &it.peek();
}

}

#endif /* BST_MAP_HPP_ */
