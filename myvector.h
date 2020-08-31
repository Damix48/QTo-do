#ifndef MYVECTOR_H
#define MYVECTOR_H

template <class T>
class MyVector {
 private:
  T* vect;
  unsigned int Size;
  unsigned int Capacity;
  void resize();

 public:
  MyVector();
  ~MyVector();
  T& operator[](unsigned int) const;
  void Push(T);
  void Pop(unsigned int = 0);
  unsigned int GetSize() const;
  unsigned int GetCapacity() const;

  class Iterator {
   private:
    T* p;

   public:
    Iterator(T*);
    T& operator*() const;
    T* operator->() const;
    Iterator& operator++();
    Iterator& operator--();
    bool operator==(const Iterator&) const;
    bool operator!=(const Iterator&) const;
  };

  Iterator begin() const;
  Iterator end() const;
};

template <class T>
MyVector<T>::MyVector() : vect(nullptr), Size(0), Capacity(0) {}

template <class T>
MyVector<T>::~MyVector() {
  delete[] vect;
}

template <class T>
T& MyVector<T>::operator[](unsigned int i) const {
  return vect[i];
}

template <class T>
unsigned int MyVector<T>::GetCapacity() const {
  return Capacity;
}

template <class T>
unsigned int MyVector<T>::GetSize() const {
  return Size;
}

template <class T>
void MyVector<T>::Pop(unsigned int i) {
  if (Size != 0 && i < Size) {
    Size--;
    T aux = vect[i];
    for (unsigned int j = i; j < Size; j++) {
      vect[j] = vect[j + 1];
    }
    vect[Size] = nullptr;
    delete aux;
  }
}

template <class T>
void MyVector<T>::resize() {
  if (Capacity == 0) {
    Capacity = 1;
  } else {
    Capacity *= 2;
  }

  T* aux = vect;
  vect = new T[Capacity];

  for (unsigned int i = 0; i < Size; i++) {
    vect[i] = aux[i];
  }

  delete[] aux;
}

template <class T>
void MyVector<T>::Push(T t) {
  if (Capacity == Size) {
    resize();
  }

  vect[Size] = t;
  Size++;
}

template <class T>
MyVector<T>::Iterator::Iterator(T* ptr) : p(ptr) {}

template <class T>
T& MyVector<T>::Iterator::operator*() const {
  return *p;
}

template <class T>
T* MyVector<T>::Iterator::operator->() const {
  return p;
}

template <class T>
typename MyVector<T>::Iterator& MyVector<T>::Iterator::operator++() {
  p++;
  return *this;
}

template <class T>
typename MyVector<T>::Iterator& MyVector<T>::Iterator::operator--() {
  p--;
  return *this;
}

template <class T>
bool MyVector<T>::Iterator::operator==(const Iterator& it) const {
  return p == it.p;
}

template <class T>
bool MyVector<T>::Iterator::operator!=(const Iterator& it) const {
  return p != it.p;
}

template <class T>
typename MyVector<T>::Iterator MyVector<T>::begin() const {
  return Iterator(vect);
}

template <class T>
typename MyVector<T>::Iterator MyVector<T>::end() const {
  return Iterator(vect + Size);
}

#endif
