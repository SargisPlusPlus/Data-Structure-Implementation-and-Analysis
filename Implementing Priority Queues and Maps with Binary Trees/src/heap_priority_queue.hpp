///*
// SARGIS DUDAKLYAN ID: 38671346, Program 3
//*/

#ifndef HEAP_PRIORITY_QUEUE_HPP_
#define HEAP_PRIORITY_QUEUE_HPP_

#include <string>
#include <iostream>
#include <sstream>
//#include <initializer_list>
#include "ics_exceptions.hpp"
#include "iterator.hpp"
#include "priority_queue.hpp"
#include <utility>              //For std::swap function
#include "array_stack.hpp"
namespace ics {

template<class T> class HeapPriorityQueue: public PriorityQueue<T> {
public:
	HeapPriorityQueue() = delete;
	explicit HeapPriorityQueue(bool (*agt)(const T& a, const T& b));
	HeapPriorityQueue(int initialLength, bool (*agt)(const T& a, const T& b));
	HeapPriorityQueue(const HeapPriorityQueue<T>& to_copy);
	//KLUDGE: not usable yet. HeapPriorityQueue(std::initializer_list<T> il,bool (*agt)(const T& a, const T& b));
	HeapPriorityQueue(ics::Iterator<T>& start, const ics::Iterator<T>& stop,
			bool (*agt)(const T& a, const T& b));
	virtual ~HeapPriorityQueue();

	virtual bool empty() const;
	virtual int size() const;
	virtual T& peek() const;
	virtual std::string str() const;

	virtual int enqueue(const T& element);
	virtual T dequeue();
	virtual void clear();

	virtual int enqueue(ics::Iterator<T>& start, const ics::Iterator<T>& stop);

	virtual HeapPriorityQueue<T>& operator =(const HeapPriorityQueue<T>& rhs);
	virtual bool operator ==(const PriorityQueue<T>& rhs) const;
	virtual bool operator !=(const PriorityQueue<T>& rhs) const;

	template<class T2>
	friend std::ostream& operator <<(std::ostream& outs,
			const HeapPriorityQueue<T2>& p);

	virtual ics::Iterator<T>& abegin() const;
	virtual ics::Iterator<T>& aend() const;

	class Iterator: public ics::Iterator<T> {
	public:
		//KLUDGE should be callable only in begin/end
		Iterator(HeapPriorityQueue<T>* fof, bool begin);
		Iterator(const Iterator& i);
		virtual ~Iterator();
		virtual T erase();
		virtual std::string str() const;
		virtual const ics::Iterator<T>& operator ++();
		virtual const ics::Iterator<T>& operator ++(int);
		virtual bool operator ==(const ics::Iterator<T>& rhs) const;
		virtual bool operator !=(const ics::Iterator<T>& rhs) const;
		virtual T& operator *() const;
		virtual T* operator ->() const;
	private:
		HeapPriorityQueue<T> it;     //Copy of pq to use as iterator via dequeue
		HeapPriorityQueue<T>* ref_pq;
		int expected_mod_count;
		bool can_erase = true;
	};

	virtual Iterator begin() const;
	virtual Iterator end() const;

private:
	T* pq;
	bool (*gt)(const T& a, const T& b); // gt(a,b) = true iff a has higher priority than b
	int length = 0;                //Physical length of array (must be > .size()
	int used = 0;                  //Amount of array used
	int mod_count = 0;                  //For sensing concurrent modification
	void ensure_length(int new_length);
	int left_child(int i);         //Useful abstractions for heaps as arrays
	int right_child(int i);
	int parent(int i);
	bool is_root(int i);
	bool in_heap(int i);
	void percolate_up(int i);
	void percolate_down(int i);
};

//Insert constructor/methods here: see array_priority_queue.hpp
template<class T>
HeapPriorityQueue<T>::HeapPriorityQueue(bool (*agt)(const T& a, const T& b)) :
		gt(agt) {
	pq = new T[length];
}

template<class T>
HeapPriorityQueue<T>::HeapPriorityQueue(int initialLength,
		bool (*agt)(const T& a, const T& b)) :
		gt(agt), length(initialLength) {
	if (length < 0)
		length = 0;
	pq = new T[length];
}

template<class T>
HeapPriorityQueue<T>::HeapPriorityQueue(const HeapPriorityQueue<T>& to_copy) :
		gt(to_copy.gt), length(to_copy.length), used(to_copy.used) {
	pq = new T[length];
	for (int i = 0; i < to_copy.used; ++i)
		pq[i] = to_copy.pq[i];
}

template<class T>
HeapPriorityQueue<T>::HeapPriorityQueue(ics::Iterator<T>& start,
		const ics::Iterator<T>& stop, bool (*agt)(const T& a, const T& b)) :
		gt(agt) {
	pq = new T[length];
	enqueue(start, stop);
}

template<class T>
HeapPriorityQueue<T>::~HeapPriorityQueue() {
	delete[] pq;
}

template<class T>
int HeapPriorityQueue<T>::enqueue(const T& element) {
	if (used == 0) {
		this->ensure_length(used + 1);
		pq[used] = element;
		used++;
	} else {
		this->ensure_length(used + 1);
		pq[used++] = element;
		percolate_up(used - 1);
	}

	++mod_count;
	return 1;

}

template<class T>
int HeapPriorityQueue<T>::enqueue(ics::Iterator<T>& start,
		const ics::Iterator<T>& stop) {
	int count = 0;
	for (; start != stop; ++start)
		count += enqueue(*start);

	return count;
}

template<class T>
T HeapPriorityQueue<T>::dequeue() {

	T to_return;
	if (this->empty())
		throw EmptyError("ArrayPriorityQueue::dequeue");

	else {
		to_return = pq[0];
		pq[0] = pq[--used];

		percolate_down(0);
	}

	++mod_count;
	return to_return;
}

template<class T>
void HeapPriorityQueue<T>::clear() {

	used = 0;
	++mod_count;
}

template<class T>
inline bool HeapPriorityQueue<T>::empty() const {
	return (used == 0);
}

template<class T>
int HeapPriorityQueue<T>::size() const {
	return used;
}

template<class T>
T& HeapPriorityQueue<T>::peek() const {
	if (empty())
		throw EmptyError("ArrayPriorityQueue::peek");

	return pq[0];
}

template<class T>
std::string HeapPriorityQueue<T>::str() const {

	std::ostringstream answer;

	if (empty())
		answer << "priority_queue[]";
	else {
		HeapPriorityQueue<T> copy(*this);
		T* temp_array = new T[used];

		for (int i = 0; i < used; i++)
			temp_array[i] = copy.dequeue();

		answer << "priority_queue[";
		for (int i = used - 1; i >= 0; --i) {
			answer << temp_array[i];
			if (!i)
				answer << "]:highest";
			else
				answer << ",";
		}

		delete[] temp_array;
	}
	return answer.str();

}

template<class T>
HeapPriorityQueue<T>& HeapPriorityQueue<T>::operator =(
		const HeapPriorityQueue<T>& rhs) {
	if (this == &rhs)
		return *this;
	this->ensure_length(rhs.used);

	gt = rhs.gt;
	used = rhs.used;

	for (int i = 0; i < used; ++i)
		pq[i] = rhs.pq[i];
	++mod_count;
	return *this;

}

template<class T>
bool HeapPriorityQueue<T>::operator ==(const PriorityQueue<T>& rhs) const {
	if (this == &rhs)
		return true;
	if (used != rhs.size())
		return false;

	HeapPriorityQueue temp = *this;
	for (auto& i = rhs.abegin(); i != rhs.aend(); ++i) {
		if (temp.dequeue() != *i)
			return false;
	}
	return true;
}

template<class T>
bool HeapPriorityQueue<T>::operator !=(const PriorityQueue<T>& rhs) const {
	return !(*this == rhs);
}

//Insert heap helper methods here.
template<class T>
void HeapPriorityQueue<T>::ensure_length(int new_length) {
	if (length >= new_length)
		return;
	T* old_pq = pq;
	length = std::max(new_length + 1, 2 * length);
	pq = new T[length];
	for (int i = 0; i < used; ++i)
		pq[i] = old_pq[i];

	delete[] old_pq;
}

template<class T>
int HeapPriorityQueue<T>::left_child(int i) //Useful abstractions for heaps as arrays
		{
	return (i * 2) + 1;
}

template<class T>
int HeapPriorityQueue<T>::right_child(int i) {
	return (i * 2) + 2;
}

template<class T>
int HeapPriorityQueue<T>::parent(int i) {
	if (is_root(i))
		return -1;
	else
		return (i - 1) / 2;
}

template<class T>
bool HeapPriorityQueue<T>::is_root(int i) {
	if (i == 0)
		return true;
	else
		return false;
}

template<class T>
bool HeapPriorityQueue<T>::in_heap(int i) {
	if (i < used && i >= 0)
		return true;
	else
		return false;
}

template<class T>
void HeapPriorityQueue<T>::percolate_up(int i) {
	if (!is_root(i)) {
		if (gt(pq[i], pq[parent(i)])) {

			std::swap(pq[parent(i)], pq[i]);
			percolate_up(parent(i));
		}
	}
}

template<class T>
void HeapPriorityQueue<T>::percolate_down(int i) {

	if (used == 0 || used == 1) {
		return;
	} else if (in_heap(right_child(i))) {
		if (gt(pq[left_child(i)], pq[right_child(i)])) {
			std::swap(pq[left_child(i)], pq[i]);
			percolate_down(left_child(i));
		} else {
			if (gt(pq[right_child(i)], pq[i])) {
				std::swap(pq[right_child(i)], pq[i]);
				percolate_down(right_child(i));
			}
		}
	} else if (in_heap(left_child(i))) {
		if (gt(pq[left_child(i)], pq[i])) {
			std::swap(pq[left_child(i)], pq[i]);
			percolate_down(left_child(i));
		}
	}
}

//KLUDGE: memory-leak
template<class T>
auto HeapPriorityQueue<T>::abegin() const -> ics::Iterator<T>& {
	return *(new Iterator(const_cast<HeapPriorityQueue<T>*>(this), true));
}

//KLUDGE: memory-leak
template<class T>
auto HeapPriorityQueue<T>::aend() const -> ics::Iterator<T>& {
	return *(new Iterator(const_cast<HeapPriorityQueue<T>*>(this), false));
}

template<class T>
auto HeapPriorityQueue<T>::begin() const -> HeapPriorityQueue<T>::Iterator {
	return Iterator(const_cast<HeapPriorityQueue<T>*>(this), true);
}

template<class T>
auto HeapPriorityQueue<T>::end() const -> HeapPriorityQueue<T>::Iterator {
	return Iterator(const_cast<HeapPriorityQueue<T>*>(this), false);
}

//Insert Iterator constructor/methods here: see array_priority_queue.hpp
template<class T>
HeapPriorityQueue<T>::Iterator::Iterator(HeapPriorityQueue<T>* fof, bool begin) :
		it(nullptr), ref_pq(fof) {
	if (begin)
		it = *fof;
	expected_mod_count = ref_pq->mod_count;
}

template<class T>
HeapPriorityQueue<T>::Iterator::Iterator(const Iterator& i) :
		it(i.it), ref_pq(i.ref_pq), expected_mod_count(i.expected_mod_count), can_erase(
				i.can_erase) {
}

template<class T>
HeapPriorityQueue<T>::Iterator::~Iterator() {
}

template<class T>
T HeapPriorityQueue<T>::Iterator::erase() {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError("HeapPriorityQueue::Iterator::erase");
	if (!can_erase)
		throw CannotEraseError(
				"HeapPriorityQueue::Iterator::erase Iterator cursor already erased");
	if (it.size() == 0)
		throw CannotEraseError(
				"ArrayPriorityQueue::Iterator::erase Iterator cursor beyond data structure");

	can_erase = false;
	T to_return = it.dequeue();
	for (int j = 0; j < ref_pq->used; j++) {
		if (ref_pq->pq[j] == to_return) {
			to_return = ref_pq->pq[j];
			ref_pq->pq[j] = ref_pq->pq[ref_pq->used - 1];
			ref_pq->percolate_down(j);
			ref_pq->percolate_up(j);
			break;
		}
	}
	--ref_pq->used;
	expected_mod_count = ref_pq->mod_count;
	return to_return;

}

template<class T>
std::string HeapPriorityQueue<T>::Iterator::str() const {
	std::ostringstream answer;
	answer << ref_pq->str() << "/peek()=" << it.peek() << "/expected_mod_count="
			<< expected_mod_count << "/can_erase=" << can_erase;
	return answer.str();

}

template<class T>
std::ostream& operator <<(std::ostream& outs, const HeapPriorityQueue<T>& p) {
	outs << p.str();
	return outs;
}

template<class T>
const ics::Iterator<T>& HeapPriorityQueue<T>::Iterator::operator ++() {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError(
				"ArrayPriorityQueue::Iterator::operator ++(int)");

	if (can_erase == true && it.size() != 0)
		it.dequeue();

	can_erase = true;
	return *this;
}

template<class T>
const ics::Iterator<T>& HeapPriorityQueue<T>::Iterator::operator ++(int) {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError(
				"ArrayPriorityQueue::Iterator::operator ++(int)");

	Iterator* to_return = new Iterator(&(this->it), can_erase);

	if (can_erase == true && it.size() != 0) {
		it.dequeue();
	}
	can_erase = true;
	return *to_return;
}

template<class T>
bool HeapPriorityQueue<T>::Iterator::operator ==(
		const ics::Iterator<T>& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("HeapPriorityQueue::Iterator::operator ==");
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError(
				"HeapPriorityQueue::Iterator::operator ==");
	if (ref_pq != rhsASI->ref_pq)
		throw ComparingDifferentIteratorsError(
				"HeapPriorityQueue::Iterator::operator ==");

	return it.size() == rhsASI->it.size();
}

template<class T>
bool HeapPriorityQueue<T>::Iterator::operator !=(
		const ics::Iterator<T>& rhs) const {
	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	if (rhsASI == 0)
		throw IteratorTypeError("HeapPriorityQueue::Iterator::operator ==");
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError(
				"HeapPriorityQueue::Iterator::operator ==");
	if (ref_pq != rhsASI->ref_pq)
		throw ComparingDifferentIteratorsError(
				"HeapPriorityQueue::Iterator::operator ==");

	return this->it.size() != rhsASI->it.size();
}

template<class T>
T& HeapPriorityQueue<T>::Iterator::operator *() const {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError(
				"HeapPriorityQueue::Iterator::operator *");
	if (!can_erase || it.empty()) {
		std::ostringstream where;
		where << " when size = " << ref_pq->size();
		throw IteratorPositionIllegal(
				"HeapPriorityQueue::Iterator::operator * Iterator illegal: "
						+ where.str());
	}
	return it.peek();
}

template<class T>
T* HeapPriorityQueue<T>::Iterator::operator ->() const {
	if (expected_mod_count != ref_pq->mod_count)
		throw ConcurrentModificationError(
				"HeapPriorityQueue::Iterator::operator *");
	if (!can_erase || it.empty()) {
		std::ostringstream where;
		where << " when size = " << ref_pq->size();
		throw IteratorPositionIllegal(
				"HeapPriorityQueue::Iterator::operator * Iterator illegal: "
						+ where.str());
	}

	return &it.peek();
}

}

#endif /* HEAP_PRIORITY_QUEUE_HPP_ */
