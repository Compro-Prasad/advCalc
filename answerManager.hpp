#ifndef answerMANAGER
#define answerMANAGER

#include <limits.h>

#include "calcError.hpp"
#include "calcStack.hpp"

template <typename Type> class answerManager {
  // The Stack Array dynamically allocated at
  // runtime
  calcStack<Type> *answerStack;
  // The number of stacks
  uint numOfStacks;
  // Size of each stack
  uint ansPerStack;
  // Total number of answers currently being
  // available
  ulong numOfAns;
  // Automatically delete older answers
  bool autoDelete;
  // Method for increasing the size of the array
  // answerStack
  bool increaseSize();
  // Shifting the answer list to delete previous
  // answers
  bool shift();

public:
  // Destructor for the answerManager
  ~answerManager();
  // Construct a default object having
  // numOfStacks = 1024,
  // ansPerStack = 8192,
  // numOfAns = 0,
  // autoDelete = false
  answerManager();
  // Construct an object specifying the number of
  // stacks to start with
  explicit answerManager(const uint);
  // Construct an object which is a copy of
  // another one
  answerManager(const answerManager &);
  // Construct an object specifying number of stacks
  // and answers per stack
  answerManager(const uint, const uint);
  // Construct an object specifying to auto delete
  // answers and the max limit of answers to be
  // stored
  answerManager(const bool, const ulong);
  // Construct an object specifying to auto delete,
  // number of stacks and answers per stack
  answerManager(const bool, const uint, const uint);
  // Return the number of answers in the list
  ulong answerCount() const { return this->numOfAns; }
  // Check if there are any answers
  bool isEmpty() const { return !this->numOfAns; }
  void toggleAutoDelete();
  bool parseAns(constStr*, Type &) const;
  bool getAns(Type &, ulong pos = 0) const;
  void display() const;
  bool push(const Type);
};

extern answerManager<long double> ansList;
extern bool store;

template <typename Type> bool answerManager<Type>::increaseSize() {
  if (++numOfStacks == 0)
    return _error(outOfRange, (--numOfStacks, 0));
  try {
    calcStack<Type> *tmp = new calcStack<Type>[numOfStacks];
    uint i;
    for (i = 0; i < numOfStacks - 1; ++i)
      tmp[i] = this->answerStack[i];
    tmp[i].setCapacity(this->ansPerStack);
    delete[] this->answerStack;
    this->answerStack = tmp;
    return 1;
  } catch (const std::bad_alloc &x) {
    return error(memAlloc);
  }
}

template <typename Type> bool answerManager<Type>::shift() {
  try {
    calcStack<Type> *tmp = new calcStack<Type>[numOfStacks];
    for (uint i = 1; i < numOfStacks; ++i)
      tmp[i - 1] = answerStack[i];
    delete[] answerStack;
    answerStack = tmp;
    return numOfAns -= ansPerStack, 1;
  } catch (const std::bad_alloc &x) {
    return error(memAlloc);
  }
}

template <typename Type> answerManager<Type>::~answerManager() {
  delete[] answerStack;
}

template <typename Type> answerManager<Type>::answerManager() {
  numOfStacks = 1024;
  answerStack = new calcStack<Type>[numOfStacks];
  ansPerStack = 8192;
  for (uint i = 0; i < numOfStacks; ++i)
    answerStack[i].setCapacity(ansPerStack);
  numOfAns = 0;
  autoDelete = false;
}

template <typename Type> answerManager<Type>::answerManager(const uint nos) {
  numOfStacks = nos;
  answerStack = new calcStack<Type>[numOfStacks];
  ansPerStack = 10000;
  for (uint i = 0; i < numOfStacks; ++i)
    answerStack[i].setCapacity(ansPerStack);
  numOfAns = 0;
  autoDelete = false;
}

template <typename Type>
answerManager<Type>::answerManager(const uint nos, const uint aps) {
  numOfStacks = nos;
  answerStack = new calcStack<Type>[numOfStacks];
  ansPerStack = aps;
  for (uint i = 0; i < numOfStacks; ++i)
    answerStack[i].setCapacity(ansPerStack);
  numOfAns = 0;
  autoDelete = false;
}

template <typename Type>
answerManager<Type>::answerManager(const bool ad, const ulong maxLt) {
  autoDelete = ad;
  numOfStacks = maxLt / 10000 + (autoDelete ? 2 : 1);
  ansPerStack = 10000;
  numOfAns = 0;
  answerStack = new calcStack<Type>[numOfStacks];
  for (uint i = 0; i < numOfStacks; ++i)
    answerStack[i].setCapacity(ansPerStack);
}

template <typename Type>
answerManager<Type>::answerManager(const bool ad, const uint nos,
                                   const uint aps) {
  this->autoDelete = ad;
  this->numOfStacks = nos + (ad ? 1 : 0);
  this->answerStack = new calcStack<Type>[this->numOfStacks];
  this->ansPerStack = aps;
  for (uint i = 0; i < numOfStacks; ++i)
    this->answerStack[i].setCapacity(aps);
  this->numOfAns = 0;
}

template <typename Type> bool answerManager<Type>::push(const Type x) {
  if (this->answerStack[this->numOfAns++ / this->numOfStacks].push(x)) {
    if (this->numOfAns == this->numOfStacks * this->ansPerStack)
      return autoDelete ? this->shift() : this->increaseSize();
    return 1;
  }
  return 0;
}

template <typename Type> void answerManager<Type>::toggleAutoDelete() {
  autoDelete = autoDelete ? 0 : 1;
}

template <typename Type>
bool answerManager<Type>::parseAns(constStr*s, Type &x) const {
  if (**s == 'A' || **s == 'a') {
    constStr c = *s + 1;
    ulong y = 0;
    while (*c > 47 && *c < 58) {
      if (y > (ULONG_MAX - *c + 48) / 10)
        return error(invalidAns);
      y = y * 10 + *(c++) - 48;
    }
    if (y > numOfAns)
      return error(invalidAns);
    if (*s == c - 1 && y <= numOfAns) {
      *s = c;
      return this->getAns(y, x);
    }
  }
  return error(parseError);
}

template <typename Type>
bool answerManager<Type>::getAns(Type &x, ulong pos) const {
  uint stackNo = pos / this->ansPerStack;
  return this->answerStack[stackNo].find(pos % this->ansPerStack, x)
             ? 1
             : error(invalidAns);
}

template <typename Type> void answerManager<Type>::display() const {
  for (uint i = 0; i < this->numOfAns; i += this->ansPerStack)
    this->answerStack[i].display();
}

#endif

/*
  Local Variables:
  mode: c++
  c-file-offset: 2
  eval: (clang-format-buffer)
  fill-column: 80
  End:
*/
