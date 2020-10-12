#include "vector.h"

#include <stdexcept>

Vector::Vector(size_t n) {
    m_size = n;
    m_data.reset(new VectorElement[m_size]);
    m_allocated = m_size;
}

Vector::Vector(const Vector& other) {
    copyFromOther(other);
}

Vector& Vector::operator=(const Vector& other) {
    copyFromOther(other);
    return *this;
}

size_t Vector::size() const {
    return m_size;
}

VectorElement& Vector::operator[](size_t n) {
    if (n >= m_size) {
        throw std::out_of_range("");
    }
    return m_data[n];
}

void Vector::pushBack(const VectorElement &element) {
    if (m_size >= m_allocated) {
        m_allocated *= 2;
        auto newMemory = new VectorElement[m_allocated];
        for (size_t i = 0; i < m_size; ++i) {
            newMemory[i] = m_data[i];
        }
        m_data.reset(newMemory);
    }
    m_data[m_size] = element;
    ++m_size;
}

void Vector::erase(size_t n) {
    for (size_t i = n; i < m_size; ++i) {
        m_data[i] = m_data[i + 1];
    }
    --m_size;
}

void Vector::copyFromOther(const Vector& other) {
    m_size = other.m_size;
    m_data.reset(new VectorElement[m_size]);
    m_allocated = m_size;
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] = other.m_data[i];
    }
}
