#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <assert.h>
#include <stdexcept>
#include <iostream>
#include <fstream>

class Element {
public:
    Element() = default;
    Element(int n_) : m_n{n_} {}
    int n() const { return m_n; }
    ~Element() {
        std::cout << "~Element()\n";
    }
private:
    int m_n{0};
};

class LinkedList {
public:
    LinkedList(size_t n_ = 0);
    LinkedList(const LinkedList& other_);
    LinkedList(LinkedList&& other_);
    ~LinkedList() { clear(); };
    LinkedList& operator=(const LinkedList& other_);
    LinkedList& operator=(LinkedList&& other_);
    void pushBack(const Element& element_);
    void pushFront(const Element& element_);
    void popBack();
    void popFront();
    size_t size() const;
    const Element& operator[](size_t n_) const;
    Element& operator[](size_t n_);
    void clear();
    bool operator==(const LinkedList& other_) const;
    bool operator!=(const LinkedList& other_) const;
    friend std::ostream& operator<<(std::ostream& stream_, LinkedList& list_);
    friend std::istream& operator>>(std::istream& stream_, LinkedList& list_);
private:
    class ElementWrapper {
    public:
        ElementWrapper(const Element& element_,
                       ElementWrapper* left_ = nullptr,
                       ElementWrapper* right_ = nullptr);
        Element element;
        ElementWrapper* left{nullptr};
        ElementWrapper* right{nullptr};
    };
public:
    class Iterator {
    public:
        Iterator(ElementWrapper* element_);
        ElementWrapper* element{nullptr};
        bool operator!=(const Iterator& other_);
        Iterator operator++(int);
        Iterator& operator++();
        Element& operator*();
    };
    Iterator begin();
    Iterator end();
    Iterator begin() const;
    Iterator end() const;
private:
    ElementWrapper* m_first{nullptr};
    ElementWrapper* m_last{nullptr};
    size_t m_size{0};
};

void printLinkedList(const LinkedList& list_);
std::ostream& operator<<(std::ostream& stream_, LinkedList& list_);
std::istream& operator>>(std::istream& stream_, LinkedList& list_);

#endif // LINKEDLIST_H
