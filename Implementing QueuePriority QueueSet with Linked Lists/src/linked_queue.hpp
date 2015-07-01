///*
//SARGIS DUDAKLYAN ID: 38671346, Program 2
//*/
//
//#ifndef LINKED_QUEUE_HPP_
//#define LINKED_QUEUE_HPP_
//
//#include <string>
//#include <iostream>
//#include <sstream>
////#include <initializer_list>
//#include "ics_exceptions.hpp"
//#include "iterator.hpp"
//#include "queue.hpp"
//
//namespace ics {
//
//template<class T> class LinkedQueue : public Queue<T>  {
// public:
//   LinkedQueue();
//   LinkedQueue(const LinkedQueue<T>& to_copy);
//   //KLUDGE: not usable yet. LinkedQueue(std::initializer_list<T>il);
//   LinkedQueue(ics::Iterator<T>& start, const ics::Iterator<T>&stop);
//   virtual ~LinkedQueue();
//
//   virtual bool empty      () const;
//   virtual int  size       () const;
//   virtual T&   peek       () const;
//   virtual std::string str () const;
//
//   virtual int  enqueue (const T& element);
//   virtual T    dequeue ();
//   virtual void clear   ();
//
//   virtual int enqueue (ics::Iterator<T>& start, const ics::Iterator<T>& stop);
//
//   virtual LinkedQueue<T>& operator = (const LinkedQueue<T>& rhs);
//   virtual bool operator == (const Queue<T>& rhs) const;
//   virtual bool operator != (const Queue<T>& rhs) const;
//
//   template<class T2>
//   friend std::ostream& operator << (std::ostream& outs, const LinkedQueue<T2>& s);
//
//   virtual ics::Iterator<T>& abegin () const;
//   virtual ics::Iterator<T>& aend   () const;
//
// private:
//   class LN;
//
// public:
//   class Iterator : public ics::Iterator<T> {
//     public:
//       //KLUDGE should be callable only in begin/end
//       Iterator(LinkedQueue<T>* fof, LN* initial);
//       virtual ~Iterator();
//       virtual T           erase();
//       virtual std::string str  () const;
//       virtual const ics::Iterator<T>& operator ++ ();
//       virtual const ics::Iterator<T>& operator ++ (int);
//       virtual bool operator == (const ics::Iterator<T>& rhs) const;
//       virtual bool operator != (const ics::Iterator<T>& rhs) const;
//       virtual T& operator *  () const;
//       virtual T* operator -> () const;
//     private:
//       LN*             prev = nullptr;  //if nullptr, current at front of list
//       LN*             current;         //if can_erase is false, this value is unusable
//       LinkedQueue<T>* ref_queue;
//       int             expected_mod_count;
//       bool            can_erase = true;
//   };
//
//   virtual Iterator begin () const;
//   virtual Iterator end   () const;
//
// private:
//   class LN {
//     public:
//       LN ()                      : next(nullptr){}
//       LN (const LN& ln)          : value(ln->value), next(ln->next){}
//       LN (T v,  LN* n = nullptr) : value(v), next(n){}
//       T   value;
//       LN* next;
//   };
//
//   int used      =  0;
//   LN* front     =  nullptr;
//   LN* rear      =  nullptr;
//   int mod_count =  0;            //For sensing concurrent modification
//   void delete_list(LN*& front);  //Recycle storage, set front'sargument to nullptr;
// };
//
//
////Insert constructor/methods here: see array_queue.hpp
//template<class T>
//LinkedQueue<T>::LinkedQueue()
//{
//}
//
//template<class T>
//LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& to_copy)
//{
// clear();
// for(auto element : to_copy)
// {
//  this->enqueue(element);
// }
//}
//
//template<class T>
//LinkedQueue<T>::LinkedQueue(ics::Iterator<T>& start, const ics::Iterator<T>& stop)
//{
//enqueue(start,stop);
//}
//
//template<class T>
//LinkedQueue<T>::~LinkedQueue()
//{
//LN* p = front;
//while(p!=nullptr)
//{
// LN *to_delete = p;
// p = p->next;
// delete to_delete;
//}
//}
//
//template<class T>
//void LinkedQueue<T>::clear()
//{
//LN* p = front;
//while(p!=nullptr)
//{
// LN *del = p;
// p = p->next;
// delete del;
//}
//used = 0;
//mod_count++;
//}
//
//template<class T>
//bool LinkedQueue<T>::empty() const
//{
//if (used==0)
//{
// return true;
//}
//else
//{
// return false;
//}
//}
//
//template<class T>
//int  LinkedQueue<T>::size() const
//{
//return used;
//}
//
//template<class T>
//T& LinkedQueue<T> :: peek() const
//{
//if(empty())
//{
// throw EmptyError("LinkedQueue::peek");
//}
// return front->value;
//}
//
//template<class T>
//std::string LinkedQueue<T>::str () const
//{
//std::ostringstream outputStr;
//if (empty())
//{
// outputStr << "queue[]";
// return outputStr.str();
//}
//else
//{
// outputStr << "queue[";
// outputStr << front->value;
//
// LN* p = front->next;
// while(p!=nullptr)
// {
//  outputStr << ",";
//  outputStr << p->value;
//  p=p->next;
// }
//  outputStr << "]:rear";
// }
// return outputStr.str();
//}
//
//template<class T>
//int  LinkedQueue<T>::enqueue (const T& element)
//{
//if (empty())
//{
// front = new LN(element);
// rear = front;
//}
//else
//{
// rear->next = new LN(element,rear->next);
// rear=rear->next;
//}
// used++;
// mod_count++;
//return 1;
//}
//
//template<class T>
//T LinkedQueue<T>::dequeue ()
//{
// if(empty())
//	throw EmptyError("LinkedQueue::dequeue");
// else
// {
//	LN *del_el = front;
//
//   front = front->next;
//	T element = del_el->value;
//	delete del_el;
//	used--;
//	mod_count++;
//	return element;
// }
//}
//
//template<class T>
//int LinkedQueue<T>::enqueue (ics::Iterator<T>& start, const ics::Iterator<T>& stop)
//{
//int counter = 0;
//for (; start != stop; start++)
//{
// enqueue(*start);
// counter++;
//}
// return counter;
//}
//
//template<class T>
//LinkedQueue<T>& LinkedQueue<T> :: operator = (const LinkedQueue<T>&rhs)
//{
//if(this != &rhs)
//{
// clear();
// enqueue(rhs.abegin(), rhs.aend());
//}
// return *this;
//}
//
//template <class T>
//bool LinkedQueue<T>::operator == (const Queue<T>& rhs) const
//{
//if (this == &rhs)
//{
// return true;
//}
//else if(used != rhs.size())
//{
// return false;
//}
//else
//{
// ics::Iterator<T>& p = abegin();
// ics::Iterator<T>& p1 = rhs.abegin();
// for(; p1 != rhs.aend(); p1++, p++)
// {
//  if(*p1 != *p)
//  {
//	return false;
//  }
// }
// return true;
//}
//}
//
//template <class T>
//bool LinkedQueue<T>::operator != (const Queue<T>& rhs) const
//{
//return !(*this == rhs);
//}
//
//template <class T>
//std::ostream& operator << (std::ostream& outs, const LinkedQueue<T>&s)
//{
//outs << s.str();
//return outs;
//}
//
////KLUDGE: memory-leak
//template<class T>
//auto LinkedQueue<T>::abegin () const -> ics::Iterator<T>& {
// return *(new Iterator(const_cast<LinkedQueue<T>*>(this),front));
//}
//
////KLUDGE: memory-leak
//template<class T>
//auto LinkedQueue<T>::aend () const -> ics::Iterator<T>& {
// return *(new Iterator(const_cast<LinkedQueue<T>*>(this),nullptr));
//}
//
//template<class T>
//auto LinkedQueue<T>::begin () const -> LinkedQueue<T>::Iterator {
// return Iterator(const_cast<LinkedQueue<T>*>(this),front);
//}
//
//template<class T>
//auto LinkedQueue<T>::end () const -> LinkedQueue<T>::Iterator {
// return Iterator(const_cast<LinkedQueue<T>*>(this),nullptr);
//}
//
//
//template<class T>
//LinkedQueue<T>::Iterator::Iterator(LinkedQueue<T>* fof, LN* initial) : current(initial), ref_queue(fof) {
// expected_mod_count = ref_queue->mod_count;
//}
//
//template<class T>
//LinkedQueue<T>::Iterator::~Iterator() {}
//
//template<class T>
//T LinkedQueue<T>::Iterator::erase() {
// if (expected_mod_count != ref_queue->mod_count)
//   throw ConcurrentModificationError ("LinkedQueue::Iterator::erase");
// if (!can_erase)
//   throw CannotEraseError("LinkedQueue::Iterator::erase Iterator cursor already erased");
// if (current == nullptr)
//   throw CannotEraseError("LinkedQueue::Iterator::erase Iterator cursor beyond data structure");
//
// can_erase = false;
// T to_return = current->value;
//
// LN* temp = current;
// current = current->next;
//
// if (prev == nullptr)
// {
//  ref_queue->front = current;
// }
// else
// {
//  prev->next = current;
// }
// delete temp;
// --ref_queue->used;
// expected_mod_count = ref_queue->mod_count;
// return to_return;
//}
//
//template<class T>
//std::string LinkedQueue<T>::Iterator::str() const {
// std::ostringstream answer;
// answer << ref_queue->str() << "(current=" << current <<",expected_mod_count=" << expected_mod_count << ",can_erase=" <<can_erase << ")";
// return answer.str();
//}
//
//template<class T>
//const ics::Iterator<T>& LinkedQueue<T>::Iterator::operator ++ () {
// if (expected_mod_count != ref_queue->mod_count)
//   throw ConcurrentModificationError("LinkedQueue::Iterator::operator ++");
// if (can_erase && current != nullptr)
// {
//   prev = current;
//   current = current->next;
// }
// can_erase = true;
// return *this;
//}
//
//template<class T>
//const ics::Iterator<T>& LinkedQueue<T>::Iterator::operator ++ (int) {
// if (expected_mod_count != ref_queue->mod_count)
//   throw ConcurrentModificationError("LinkedQueue::Iterator::operator ++(int)");
//
// Iterator* to_return = new Iterator(this->ref_queue,prev);
// if (can_erase && current != nullptr) {
//   to_return->current = prev = current;
//   current = current->next;
// }
// can_erase = true;
// return *to_return;
//}
//
//template<class T>
//bool LinkedQueue<T>::Iterator::operator == (const ics::Iterator<T>&rhs) const
//{
// const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
// if (rhsASI == 0)
//   throw IteratorTypeError("LinkedQueue::Iterator::operator ==");
// if (expected_mod_count != ref_queue->mod_count)
//   throw ConcurrentModificationError("LinkedQueue::Iterator::operator ==");
// if (ref_queue != rhsASI->ref_queue)
//   throw ComparingDifferentIteratorsError("LinkedQueue::Iterator::operator ==");
// return current == rhsASI->current;
//}
//
//
//template<class T>
//bool LinkedQueue<T>::Iterator::operator != (const ics::Iterator<T>&rhs) const
//{
// const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
// if (rhsASI == 0)
//   throw IteratorTypeError("LinkedQueue::Iterator::operator !=");
// if (expected_mod_count != ref_queue->mod_count)
//   throw ConcurrentModificationError ("LinkedQueue::Iterator::operator !=");
// if (ref_queue != rhsASI->ref_queue)
//   throw ComparingDifferentIteratorsError ("LinkedQueue::Iterator::operator !=");
// return current != rhsASI->current;
//}
//
//template<class T>
//T& LinkedQueue<T>::Iterator::operator *() const
//{
// if (expected_mod_count != ref_queue->mod_count)
//   throw ConcurrentModificationError ("LinkedQueue::Iterator::operator *");
// if (!can_erase || current == nullptr)
// {
//   std::ostringstream where;
//   where << current
//         << " when front = " << ref_queue->front << " and "
//         << " and rear = " << ref_queue->rear;
//   throw IteratorPositionIllegal("LinkedQueue::Iterator::operator * Iterator illegal: "+where.str());
// }
// return current->value;
//}
//
//template<class T>
//T* LinkedQueue<T>::Iterator::operator ->() const
//{
// if (expected_mod_count !=ref_queue->mod_count)
//   throw ConcurrentModificationError("LinkedQueue::Iterator::operator *");
// if (!can_erase || current == nullptr) {
//   std::ostringstream where;
//   where << current
//         << " when front = " << ref_queue->front << " and "
//         << " and rear = " << ref_queue->rear;
//   throw IteratorPositionIllegal("LinkedQueue::Iterator::operator * Iterator illegal: "+where.str());
// }
// return &(current->value);
//}
//
//}

#endif /* LINKED_QUEUE_HPP_ */
