///*
// SARGIS DUDAKLYAN ID: 38671346, Program 2
//*/
//
//#ifndef LINKED_PRIORITY_QUEUE_HPP_
//#define LINKED_PRIORITY_QUEUE_HPP_
//
//#include <string>
//#include <iostream>
//#include <sstream>
////#include <initializer_list>
//#include "ics_exceptions.hpp"
//#include "iterator.hpp"
//#include "priority_queue.hpp"
//#include "array_stack.hpp"
//
//namespace ics {
//template<class T> class LinkedPriorityQueue : public PriorityQueue<T>  {
//  public:
//    LinkedPriorityQueue() = delete;
//    explicit LinkedPriorityQueue(bool (*agt)(const T& a, const T& b));
//    LinkedPriorityQueue(const LinkedPriorityQueue<T>& to_copy);
//    //KLUDGE: not usable yet. LinkedPriorityQueue(std::initializer_list<T> il,bool (*agt)(const T& a, const T& b));
//    LinkedPriorityQueue(ics::Iterator<T>& start, const ics::Iterator<T>& stop,bool (*agt)(const T& a, const T& b));
//    virtual ~LinkedPriorityQueue();
//
//    virtual bool empty      () const;
//    virtual int  size       () const;
//    virtual T&   peek       () const;
//    virtual std::string str () const;
//
//    virtual int  enqueue (const T& element);
//    virtual T    dequeue ();
//    virtual void clear   ();
//
//    virtual int enqueue (ics::Iterator<T>& start, const ics::Iterator<T>& stop);
//
//    virtual LinkedPriorityQueue<T>& operator = (const LinkedPriorityQueue<T>& rhs);
//    virtual bool operator == (const PriorityQueue<T>& rhs) const;
//    virtual bool operator != (const PriorityQueue<T>& rhs) const;
//
//    template<class T2>
//    friend std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T2>& s);
//
//    virtual ics::Iterator<T>& abegin () const;
//    virtual ics::Iterator<T>& aend   () const;
//
//  private:
//    class LN;
//
//  public:
//    class Iterator : public ics::Iterator<T> {
//      public:
//        //KLUDGE should be callable only in begin/end
//        Iterator(LinkedPriorityQueue<T>* fof, LN* initial);
//        virtual ~Iterator();
//        virtual T           erase();
//        virtual std::string str  () const;
//        virtual const ics::Iterator<T>& operator ++ ();
//        virtual const ics::Iterator<T>& operator ++ (int);
//        virtual bool operator == (const ics::Iterator<T>& rhs) const;
//        virtual bool operator != (const ics::Iterator<T>& rhs) const;
//        virtual T& operator *  () const;
//        virtual T* operator -> () const;
//      private:
//        LN*                     prev;     //if header, current at front of list
//        LN*                     current;  //if can_erase is false, this value is unusable
//        LinkedPriorityQueue<T>* ref_pq;
//        int                     expected_mod_count;
//        bool                    can_erase = true;
//    };
//
//    virtual Iterator begin () const;
//    virtual Iterator end   () const;
//
//  private:
//    class LN {
//      public:
//        LN ()                      : next(nullptr){}
//        LN (const LN& ln)          : value(ln->value), next(ln->next){}
//        LN (T v,  LN* n = nullptr) : value(v), next(n){}
//        T   value;
//        LN* next;
//    };
//
//    bool (*gt)(const T& a, const T& b);  // gt(a,b) = true iff a has higher priority than b
//    int used      =  0;
//    LN* front     =  new LN();
//    int mod_count =  0;                  //For sensing concurrent modification
//    void delete_list(LN*& front);        //Recycle storage, set front's argument to nullptr;
//  };
//
//template<class T>
//LinkedPriorityQueue<T>::LinkedPriorityQueue(bool (*agt)(const T& a, const T& b)) : gt(agt)
//{
//}
//
//template<class T>
//LinkedPriorityQueue<T>::LinkedPriorityQueue(const LinkedPriorityQueue<T>& to_copy) : gt(to_copy.gt)
//{
// for(auto element : to_copy)
// enqueue(element);
//}
//
//template<class T>
//LinkedPriorityQueue<T>::LinkedPriorityQueue(ics::Iterator<T>& start, const ics::Iterator<T>& stop, bool (*agt)(const T& a, const T& b)): gt(agt)
//{
// enqueue(start,stop);
//}
//
//template<class T>
//LinkedPriorityQueue<T>::~LinkedPriorityQueue()
//{
// LN* p=this->front;
// while(p != nullptr)
// {
//  LN *del = p;
//  p = p->next;
//  delete del;
// }
//}
//
//template<class T>
//void LinkedPriorityQueue<T>::clear()
//{
// LN* p=this->front->next;
// while(p != nullptr)
// {
//  LN *del = p;
//  p = p->next;
//  front->next = p;
//  delete del;
// }
//  used = 0;
//  mod_count++;
//}
//
//template<class T>
//int LinkedPriorityQueue<T>::enqueue (const T& element)
//{
//  if(front->next == nullptr)
//  {
//	front->next = new LN(element);
//  }
//  else if (gt(element, front->next->value))
//  {
//   LN* el = front->next;
//   front->next = new LN(element, el);
//  }
//  else
//  {
//   LN* p1 = front->next;
//   LN* p2 = front->next;
//
//   while (p2 != nullptr && (!gt(element, p2->value)))
//   {
//	p1 = p2;
//	p2 = p2->next;
//   }
//	p1->next = new LN(element, p2);
//  }
//  used++; mod_count++;
// return 1;
//}
//
//
//template <class T>
//int LinkedPriorityQueue<T>::enqueue (ics::Iterator<T>& start, const ics::Iterator<T>& stop)
//{
//    int i=0;
//	for (; start != stop; start++)
//	{
//	 enqueue(*start);i++;
//	}
//	return i;
//}
//
//
//template <class T>
//bool LinkedPriorityQueue<T>::empty() const
//{
//	if(used == 0)
//	return true;
//
//	else
//	return false;
//}
//
//template<class T>
//int  LinkedPriorityQueue<T>::size() const
//{
// return used;
//}
//
//template<class T>
//T& LinkedPriorityQueue<T> :: peek() const
//{
// {
//  if(this->empty())
//	throw EmptyError("LinkedPriorityQueue::peek");
//  else
//   return front->next->value;
// }
//}
//
//template<class T>
//std::string LinkedPriorityQueue<T>::str () const
//{
// LinkedPriorityQueue* lpq = new LinkedPriorityQueue([](const T& a, const T& b){return a > b;});
// lpq->enqueue(this->abegin(), this->aend());
//
// std::ostringstream pq;
// if (this->empty())
// {
//  pq << "priority_queue[]";
//  return pq.str();
// }
// else
// {
//  pq << "priority_queue[";
//  LN* p = lpq->front->next;
//  while(p!=nullptr)
//  {
//    pq << p->value;
//    if(p->next != nullptr)
//    pq << ",";
//    p = p->next;
//  }
//    pq << "]:highest";
//  }
//   return pq.str();
//}
//
//template<class T>
//T LinkedPriorityQueue<T>::dequeue ()
//{
// if(this->empty())
//  throw EmptyError("LinkedPriorityQueue::dequeue");
//
// LN* del = front->next;
// this->front->next = del->next;
// T element = del->value;
// del->next = nullptr;
// delete del;
// used--;mod_count++;
// return element;
//}
//
//template<class T>
//LinkedPriorityQueue<T>& LinkedPriorityQueue<T> :: operator = (const LinkedPriorityQueue<T>& rhs)
//{
// if(this != &rhs)
// {
//  this->clear();
//  gt = rhs.gt;
//  LN* p = rhs.front->next;
//  while(p!=nullptr)
//  {
//   enqueue(p->value);
//   p=p->next;
//  }
// }
//  return *this;
//}
//
//template <class T>
//bool LinkedPriorityQueue<T>::operator == (const PriorityQueue<T>& rhs) const
//{
// if(this == &rhs)
//  return true;
// else if(used != rhs.size())
// {
//  return false;
// }
// else
// {
//  ics::Iterator<T>& p = this->abegin();
//  ics::Iterator<T>& p1 = rhs.abegin();
//  for(; p1 != rhs.aend(); p1++, p++)
//  {
//   if(*p1 != *p)
//   {
//	return false;
//   }
//  }
//  return true;
// }
//}
//
//template <class T>
//bool LinkedPriorityQueue<T>::operator != (const PriorityQueue<T>& rhs) const
//{
// return !(*this == rhs);
//}
//
//template <class T>
//std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T>& s)
//{
//  outs << s.str();
//  return outs;
//}
//
////KLUDGE: memory-leak
//template<class T>
//auto LinkedPriorityQueue<T>::abegin () const -> ics::Iterator<T>& {
//  return *(new Iterator(const_cast<LinkedPriorityQueue<T>*>(this),front->next));
//}
//
////KLUDGE: memory-leak
//template<class T>
//auto LinkedPriorityQueue<T>::aend () const -> ics::Iterator<T>& {
//  return *(new Iterator(const_cast<LinkedPriorityQueue<T>*>(this),nullptr));
//}
//
//template<class T>
//auto LinkedPriorityQueue<T>::begin () const -> LinkedPriorityQueue<T>::Iterator {
//  return Iterator(const_cast<LinkedPriorityQueue<T>*>(this),front->next);
//}
//
//template<class T>
//auto LinkedPriorityQueue<T>::end () const -> LinkedPriorityQueue<T>::Iterator {
//  return Iterator(const_cast<LinkedPriorityQueue<T>*>(this),nullptr);
//}
//
//
//template<class T>
//LinkedPriorityQueue<T>::Iterator::Iterator(LinkedPriorityQueue<T>* fof, LN* initial) : current(initial), ref_pq(fof) {
//  prev = ref_pq->front;
//  expected_mod_count = ref_pq->mod_count;
//}
//
//template<class T>
//LinkedPriorityQueue<T>::Iterator::~Iterator() {}
//
//template<class T>
//T LinkedPriorityQueue<T>::Iterator::erase() {
//  if (expected_mod_count != ref_pq->mod_count)
//    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::erase");
//  if (!can_erase)
//    throw CannotEraseError("LinkedPriorityQueue::Iterator::erase Iterator cursor already erased");
//
//  if (current == nullptr)
//    throw CannotEraseError("LinkedPriorityQueue::Iterator::erase Iterator cursor beyond data structure");
//
//  can_erase = false;
//  T to_return = current->value;
//
//  if (prev == nullptr)
//  {
//   ref_pq->front = current;
//  }
//  else
//  {
//   prev->next = current;
//  }
//  --ref_pq->used;
//  expected_mod_count = ref_pq->mod_count;
//  return to_return;
//}
//
//template<class T>
//std::string LinkedPriorityQueue<T>::Iterator::str() const {
//  std::ostringstream answer;
//  answer << ref_pq->str() << "(current=" << current << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
//  return answer.str();
//}
//
//template<class T>
//const ics::Iterator<T>& LinkedPriorityQueue<T>::Iterator::operator ++ () {
//  if (expected_mod_count != ref_pq->mod_count)
//    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ++");
//
//  if (can_erase && current != nullptr) {
//    prev = current;
//    current = current->next;
//  }
//  can_erase = true;
//  return *this;
//}
//
////KLUDGE: creates garbage! (can return local value!)
//template<class T>
//const ics::Iterator<T>& LinkedPriorityQueue<T>::Iterator::operator ++ (int) {
//  if (expected_mod_count != ref_pq->mod_count)
//    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ++(int)");
//
//  Iterator* to_return = new Iterator(this->ref_pq,prev);
//  if (can_erase && current != nullptr) {
//    to_return->current = prev = current;
//    current = current->next;
//  }
//  can_erase = true;
//  return *to_return;
//}
//
//template<class T>
//bool LinkedPriorityQueue<T>::Iterator::operator == (const ics::Iterator<T>& rhs) const {
//  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//  if (rhsASI == 0)
//    throw IteratorTypeError("LinkedPriorityQueue::Iterator::operator ==");
//  if (expected_mod_count != ref_pq->mod_count)
//    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ==");
//  if (ref_pq != rhsASI->ref_pq)
//    throw ComparingDifferentIteratorsError("LinkedPriorityQueue::Iterator::operator ==");
//  return current == rhsASI->current;
//}
//
//template<class T>
//bool LinkedPriorityQueue<T>::Iterator::operator != (const ics::Iterator<T>& rhs) const {
//  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//  if (rhsASI == 0)
//    throw IteratorTypeError("LinkedPriorityQueue::Iterator::operator !=");
//  if (expected_mod_count != ref_pq->mod_count)
//    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator !=");
//  if (ref_pq != rhsASI->ref_pq)
//    throw ComparingDifferentIteratorsError("LinkedPriorityQueue::Iterator::operator !=");
//  return current != rhsASI->current;
//}
//
//template<class T>
//T& LinkedPriorityQueue<T>::Iterator::operator *() const {
//  if (expected_mod_count !=ref_pq->mod_count)
//    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator *");
//  if (!can_erase || current == nullptr) {
//    std::ostringstream where;
//    where << current
//          << " when front = " << ref_pq->front;
//    throw IteratorPositionIllegal("LinkedPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
//  }
//  return current->value;
//}
//
//template<class T>
//T* LinkedPriorityQueue<T>::Iterator::operator ->() const {
//  if (expected_mod_count !=ref_pq->mod_count)
//    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator *");
//  if (!can_erase || current == nullptr) {
//    std::ostringstream where;
//    where << current
//          << " when front = " << ref_pq->front;
//    throw IteratorPositionIllegal("LinkedPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
//  }
//    return &(current->value);
//}
//}

#endif /* LINKED_PRIORITY_QUEUE_HPP_ */
