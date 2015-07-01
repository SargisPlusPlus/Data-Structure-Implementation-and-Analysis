 /*
 SARGIS DUDAKLYAN ID: 38671346, Program 2
*/
#ifndef LINKED_SET_HPP_
#define LINKED_SET_HPP_

#include <string>
#include <iostream>
#include <sstream>
//#include <initializer_list>
#include "ics_exceptions.hpp"
#include "iterator.hpp"
#include "set.hpp"

namespace ics {
template<class T> class LinkedSet : public Set<T>	{
  public:
	LinkedSet();
	LinkedSet(const LinkedSet<T>& to_copy);
	//KLUDGE: not usable yet. LinkedSet(std::initializer_list<T> il);
    LinkedSet(ics::Iterator<T>& start, const ics::Iterator<T>& stop);

    virtual ~LinkedSet();

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

    virtual LinkedSet<T>& operator = (const LinkedSet<T>& rhs);
    virtual bool operator == (const Set<T>& rhs) const;
    virtual bool operator != (const Set<T>& rhs) const;
    virtual bool operator <= (const Set<T>& rhs) const;
    virtual bool operator <  (const Set<T>& rhs) const;
    virtual bool operator >= (const Set<T>& rhs) const;
    virtual bool operator >  (const Set<T>& rhs) const;

    template<class T2>
    friend std::ostream& operator << (std::ostream& outs, const LinkedSet<T2>& s);

    virtual ics::Iterator<T>& abegin () const;
    virtual ics::Iterator<T>& aend   () const;

  private:
    class LN;

  public:
    class Iterator : public ics::Iterator<T> {
      public:
        //KLUDGE should be callable only in begin/end
        Iterator(LinkedSet<T>* fof, LN* initial);
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
        LN*           current;  //if can_erase is false, this value is unusable
        LinkedSet<T>* ref_set;
        int           expected_mod_count;
        bool          can_erase = true;
    };

    virtual Iterator begin () const;
    virtual Iterator end   () const;

  private:
    class LN {
      public:
        LN ()                      : next(nullptr){}
        LN (const LN& ln)          : value(ln->value), next(ln->next){}
        LN (T v,  LN* n = nullptr) : value(v), next(n){}
        T   value;
        LN* next;
    };

    LN* front     = new LN();
    LN* trailer   = front;
    int used      = 0;             //Amount of array used
    int mod_count = 0;             //For sensing concurrent modification
    int erase_at(LN* p);
    void delete_list(LN*& front);  //Recycle storage, set front's argument to nullptr;
  };

template<class T>
LinkedSet<T>::LinkedSet()
{
}

template<class T>
LinkedSet<T> :: LinkedSet(const LinkedSet<T>& to_copy)
{
  clear();
  for(T element : to_copy)
  {
   insert(element);
  }
}

template<class T>
LinkedSet<T> :: LinkedSet(ics::Iterator<T>& start, const ics::Iterator<T>& stop)
{
 insert(start,stop);
}

template<class T>
LinkedSet<T>::~LinkedSet()
{
 clear();
 delete front;
}

template<class T>
bool LinkedSet<T>::empty() const
{
 if(used == 0)
 return true;
 else
 return false;
}

template<class T>
int LinkedSet<T>::size() const
{
 return used;
}

template<class T>
bool LinkedSet<T>::contains(const T& element) const
{
 LN* p=front;
 while(p != trailer)
 {
  if (p->value == element)
  {
   return true;
  }
   p = p->next;
 }
 return false;
}

template<class T>
std::string LinkedSet<T>::str () const
{
 std::ostringstream outputStr;
 if (this->empty())
 {
  outputStr << "set[]";
  return outputStr.str();
 }
 else
 {
  outputStr << "set[";
  LN* p = front;
  while(p != trailer)
  {
   outputStr << p->value;
   if(p->next != trailer)
   { outputStr << ",";}
   p=p->next;
  }
   outputStr << "]";
 }
  return outputStr.str();
}

template<class T>
bool LinkedSet<T>::contains (ics::Iterator<T>& start, const ics::Iterator<T>& stop) const
{
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
int LinkedSet<T>:: insert (const T& element)
{
 if(contains(element))
  return 0;
 else
 {
  front = new LN(element, front);
  used++;
  mod_count++;
  return 1;
 }
}

template<class T>
int LinkedSet<T>:: erase (const T& element)
{
 if(!contains(element))
 return 0;

 LN* p_el;
 if(front->value == element)
 {
  p_el = front;
  front = front->next;
  p_el->next = nullptr;
  delete p_el;
 }
 else
 {
  for(LN* p = front; p != trailer;)
  {
   p_el = p;
   p = p->next;
   if(p->value == element)
   {
	p_el->next = p->next;
	p->next = nullptr;
	delete p;
	break;
   }
  }
 }
 used--; mod_count++;
 return 1;
}

template<class T>
int LinkedSet<T>::erase_at(LN* p) {
 LN* p_el;
 if (p==front)
 {
  p_el = front;
  front = front->next;
  p_el->next = nullptr;
  delete p_el;
 }
  return 1;
}


template<class T>
void LinkedSet<T>:: clear ()
{
 LN* p_el = front;
 while(front != trailer)
 {
  front = front->next;
  p_el->next = nullptr;
  delete p_el;
  p_el = front;
 }
 used = 0;mod_count++;
}

template<class T>
int LinkedSet<T>::insert (ics::Iterator<T>& start, const ics::Iterator<T>& stop)
{
 int i = 0;
 for(; start != stop; start++)
 {
  insert(*start);
  i++;
 }
 return i;
}

template<class T>
int LinkedSet<T>:: erase (ics::Iterator<T>& start, const ics::Iterator<T>& stop)
{
 int i = 0;
 for(; start != stop; start++)
 {
  i += erase(*start);
 }
 return i;
}

template<class T>
int LinkedSet<T>::retain (ics::Iterator<T>& start, const ics::Iterator<T>& stop)
{
 int i = 0;
 LinkedSet* p_el = new LinkedSet();
 for (; start != stop; start++)
 {
  if(contains(*start))
  p_el->insert(*start);
 }
  this->clear();
 for(LN* p = p_el->front; p != p_el->trailer;  p= p->next)
 {
  this->insert(p->value);
  i++;
 }
  p_el->clear();
  delete p_el;
  return i;
}

template<class T>
LinkedSet<T>& LinkedSet<T>::operator = (const LinkedSet<T>& rhs)
{
 if(this != &rhs)
 {
  this->clear();

  for (LN* p = rhs.front; p != rhs.trailer; p = p->next)
  {
   insert(p->value);
  }
 }
 return *this;
}

template<class T>
bool LinkedSet<T>:: operator == (const Set<T>& rhs) const
{
 if (this == &rhs)
  return true;
 else if(used != rhs.size())
 {
  return false;
 }
 else
 {
  LinkedSet* p_el = new LinkedSet(rhs.abegin(), rhs.aend());

  for(LN* p = p_el->front; p != p_el->trailer; p = p->next)
  {
   if(!contains(p->value))
   {
	return false;
   }
  }
   p_el->clear();
   delete p_el;
   return true;
 }
}

template<class T>
bool LinkedSet<T>:: operator != (const Set<T>& rhs) const
{
 return !(*this == rhs);
}

template<class T>
bool LinkedSet<T>:: operator <= (const Set<T>& rhs) const
{
 if(*this == rhs)
  return true;

 else
 {
  LN* p = front;
  while(p!=trailer)
  {
    if (!(rhs.contains(p->value)))
    {
	 return false;
    }
     p=p->next;
   }
  }
  return true;
}

template<class T>
bool LinkedSet<T>:: operator < (const Set<T>& rhs) const
{
 return !(*this >= rhs);
}

template<class T>
bool LinkedSet<T>:: operator >= (const Set<T>& rhs) const
{
 if(*this == rhs)
  return true;
 else
 {
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
bool LinkedSet<T>:: operator > (const Set<T>& rhs) const
{
 return !(*this <= rhs);
}

template<class T>
std::ostream& operator << (std::ostream& outs, const LinkedSet<T>& s)
{
 outs << s.str();
 return outs;
}
//KLUDGE: memory-leak
template<class T>
auto LinkedSet<T>::abegin () const -> ics::Iterator<T>& {
  return *(new Iterator(const_cast<LinkedSet<T>*>(this),front));
}

//KLUDGE: memory-leak
template<class T>
auto LinkedSet<T>::aend () const -> ics::Iterator<T>& {
  return *(new Iterator(const_cast<LinkedSet<T>*>(this),trailer));
}

template<class T>
auto LinkedSet<T>::begin () const -> LinkedSet<T>::Iterator {
  return Iterator(const_cast<LinkedSet<T>*>(this),front);
}

template<class T>
auto LinkedSet<T>::end () const -> LinkedSet<T>::Iterator {
  return Iterator(const_cast<LinkedSet<T>*>(this),trailer);
}

template<class T>
LinkedSet<T>::Iterator::Iterator(LinkedSet<T>* fof, LN* initial) : current(initial), ref_set(fof) {
 expected_mod_count = ref_set->mod_count;
}

template<class T>
LinkedSet<T>::Iterator::~Iterator()
{
}

template<class T>
T LinkedSet<T>::Iterator::erase() {
 if (expected_mod_count != ref_set->mod_count)
  throw ConcurrentModificationError("LinkedSet::Iterator::erase");
 if (!can_erase)
  throw CannotEraseError("LinkedSet::Iterator::erase Iterator cursor already erased");
 if (current == nullptr)
  throw CannotEraseError("LinkedSet::Iterator::erase Iterator cursor beyond data structure");

  can_erase = false;
  T to_Return = current->value;
   --ref_set->used;
   expected_mod_count = ref_set->mod_count;
   return to_Return;
}

template<class T>
std::string LinkedSet<T>::Iterator::str() const
{
 std::ostringstream answer;
 answer << ref_set->str() << "(current=" << current << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
 return answer.str();
}

template<class T>
const ics::Iterator<T>& LinkedSet<T>::Iterator::operator ++ () {
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("LinkedSet::Iterator::operator ++");
  if (can_erase && current != nullptr)
  {
   current = current->next;
  }
  can_erase = true;
  return *this;
}

//KLUDGE: creates garbage! (can return local value!)
template<class T>
const ics::Iterator<T>& ics::LinkedSet<T>::Iterator::operator ++ (int) {
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("LinkedSet::Iterator::operator ++(int)");

  Iterator* toReturn = new Iterator(this->ref_set,current->next);
  if (can_erase && current != nullptr)
  {
    current = current->next;
  }
  can_erase = true;
  return *toReturn;
}

template<class T>
bool ics::LinkedSet<T>::Iterator::operator == (const ics::Iterator<T>& rhs) const {
  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("LinkedSet::Iterator::operator ==");
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("LinkedSet::Iterator::operator ==");
  if (ref_set != rhsASI->ref_set)
    throw ComparingDifferentIteratorsError("LinkedSet::Iterator::operator ==");
  return current == rhsASI->current;
}


template<class T>
bool ics::LinkedSet<T>::Iterator::operator != (const ics::Iterator<T>& rhs) const {
 const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
  if (rhsASI == 0)
    throw IteratorTypeError("LinkedSet::Iterator::operator !=");
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("LinkedSet::Iterator::operator !=");
  if (ref_set != rhsASI->ref_set)
    throw ComparingDifferentIteratorsError("LinkedSet::Iterator::operator !=");
  return current != rhsASI->current;
 return true;
}

template<class T>
T& ics::LinkedSet<T>::Iterator::operator *() const {
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("LinkedSet::Iterator::operator *");
  if (!can_erase || current == nullptr)
  {
    std::ostringstream where;
    where 	<< current
    		<< " when front = " << ref_set->front << " and "
    		<< " and trailer = " << ref_set->trailer;
    throw IteratorPositionIllegal("LinkedSet::Iterator::operator * Iterator illegal: "+where.str());
  }
  return current->value;
}

template<class T>
T* ics::LinkedSet<T>::Iterator::operator ->() const {
  if (expected_mod_count != ref_set->mod_count)
    throw ConcurrentModificationError("LinkedSet::Iterator::operator *");
  if (!can_erase || current == nullptr)
  {
   std::ostringstream where;
   where << current << " when size = " << ref_set->size();
   throw IteratorPositionIllegal("LinkedSet::Iterator::operator * Iterator illegal: "+where.str());
  }
  return &(current->value);
 }
}

#endif /* LINKED_SET_HPP_ */
