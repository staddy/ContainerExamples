#ifndef STACK_H
#define STACK_H

#include <memory>
#include <istream>
#include <ostream>

template<typename T> class Stack {
public:
    Stack() = default;
    Stack(const Stack& other_);
    Stack(Stack&& other_) = default;
    ~Stack() = default;
    Stack& operator=(const Stack& other_);
    Stack& operator=(Stack&& other_) = default;
    T& top();
    void pop();
    void push(const T& value_);
    void push(T&& value_);
    size_t size() const;
    bool empty() const;
    void clear();
    friend std::ostream& operator<<(std::ostream&, const Stack<T>&);
    friend std::istream& operator>>(std::istream&, Stack<T>&);
private:
    void copy(const Stack& other_);
    class StackElement {
    public:
        StackElement() = delete;
        StackElement(const T& value_, std::unique_ptr<StackElement>&& next_) :
            m_value{value_}, m_next{std::move(next_)} { }
        StackElement(T&& value_, std::unique_ptr<StackElement>&& next_) :
            m_value{std::move(value_)}, m_next{std::move(next_)} { }
        T& value() { return m_value; }
        std::unique_ptr<StackElement>& next() { return m_next; }
    private:
        T m_value;
        std::unique_ptr<StackElement> m_next{nullptr};
    };
    std::unique_ptr<StackElement> m_top{nullptr};
    size_t m_size{0};
};

template<typename T> void Stack<T>::copy(const Stack& other_) {
    if (other_.m_top == nullptr) {
        return;
    }
    Stack<T> temp;
    auto t = other_.m_top.get();
    do {
        temp.push(t->value());
        t = t->next().get();
    } while (t != nullptr);
    t = temp.m_top.get();
    do {
        push(t->value());
        t = t->next().get();
    } while (t != nullptr);
}

template<typename T> Stack<T>::Stack(const Stack& other_) {
    copy(other_);
}

template<typename T> Stack<T>& Stack<T>::operator=(const Stack& other_) {
    if (&other_ != this) {
        clear();
        copy(other_);
    }
    return *this;
}

template<typename T> T& Stack<T>::top() {
    if (m_top == nullptr) {
        throw std::out_of_range("top() of empty stack");
    }
    return m_top->value();
}

template<typename T> void Stack<T>::pop() {
    if (m_top != nullptr) {
        m_top = std::move(m_top->next());
        --m_size;
    }
}

template<typename T> void Stack<T>::push(const T& value_) {
    m_top = std::make_unique<StackElement>(value_, std::move(m_top));
    ++m_size;
}

template<typename T> void Stack<T>::push(T&& value_) {
    m_top = std::make_unique<StackElement>(std::move(value_), std::move(m_top));
    ++m_size;
}

template<typename T> size_t Stack<T>::size() const {
    return m_size;
}

template<typename T> bool Stack<T>::empty() const {
    return m_top == nullptr;
}

template<typename T> void Stack<T>::clear() {
    m_top = nullptr;
    m_size = 0;
}

std::ostream& operator<<(std::ostream& stream_, const Stack<int>& stack_) {
    if (stack_.m_top == nullptr) {
        return stream_;
    }
    auto p = stack_.m_top.get();
    auto s = stack_.size();
    stream_.write(reinterpret_cast<char*>(&s), sizeof(s));
    stream_ << p->value();
    while ((p = p->next().get())) {
        stream_ << p->value();
    }
    return stream_;
}

template<typename T> std::istream& operator>>(std::istream& stream_, Stack<T>& stack_) {
    size_t s;
    stream_.read(reinterpret_cast<char*>(&s), sizeof(s));
    stack_.clear();
    for (size_t i = 0; i < s; ++i) {
        T temp;
        stream_ >> temp;
        stack_.push(temp);
    }
    return stream_;
}

#endif // STACK_H
