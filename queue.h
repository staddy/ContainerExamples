#ifndef QUEUE_H
#define QUEUE_H

#include <memory>
#include <stdexcept>

template<typename T> class Queue {
public:
    Queue() = default;
    Queue(const Queue& other_) = delete;
    Queue(Queue&& other_) = default;
    ~Queue() = default;
    Queue& operator=(const Queue& other_) = delete;
    Queue& operator=(Queue&& other_) = default;
    T& front();
    T& back();
    void pop();
    void push(const T& value_);
    void push(T&& value_);
    size_t size() const;
    bool empty() const;
    void clear();
private:
    class QueueElement {
    public:
        QueueElement() = delete;
        QueueElement(const T& value_, std::shared_ptr<QueueElement>&& next_) :
            m_value{value_}, m_next{std::move(next_)} { }
        QueueElement(T&& value_, std::shared_ptr<QueueElement>&& next_) :
            m_value{std::move(value_)}, m_next{std::move(next_)} { }
        T& value() { return m_value; }
        std::shared_ptr<QueueElement>& next() { return m_next; }
    private:
        T m_value;
        std::shared_ptr<QueueElement> m_next{nullptr};
    };
    std::shared_ptr<QueueElement> m_front{nullptr};
    std::shared_ptr<QueueElement> m_back{nullptr};
    size_t m_size{0};
};

template<typename T> T& Queue<T>::front() {
    if (m_front == nullptr) {
        throw std::out_of_range("front() of empty queue");
    }
    return m_front->value();
}

template<typename T> T& Queue<T>::back() {
    if (m_back == nullptr) {
        throw std::out_of_range("back() of empty queue");
    }
    return m_back->value();
}

template<typename T> void Queue<T>::pop() {
    if (m_front != nullptr) {
        m_front = std::move(m_front->next());
        --m_size;
    }
}

template<typename T> void Queue<T>::push(const T& value_) {
    if (m_back == nullptr) {
        m_back = m_front = std::make_shared<QueueElement>(value_, std::move(m_front));
    } else {
        m_back;
    }
    ++m_size;
}

template<typename T> void Queue<T>::push(T&& value_) {
    m_front = std::make_shared<QueueElement>(std::move(value_), std::move(m_front));
    ++m_size;
}

template<typename T> size_t Queue<T>::size() const {
    return m_size;
}

template<typename T> bool Queue<T>::empty() const {
    return m_front == nullptr;
}

template<typename T> void Queue<T>::clear() {
    m_front = nullptr;
    m_back = nullptr;
    m_size = 0;
}

#endif // QUEUE_H
