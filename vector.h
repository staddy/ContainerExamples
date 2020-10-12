#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <string>

class VectorElement {
public:
    VectorElement() = default;
    VectorElement(const VectorElement&) = default;
    VectorElement(VectorElement&&) = default;
    VectorElement(std::string t) : m_test(std::move(t)) { }
    VectorElement& operator=(const VectorElement&) = default;
    VectorElement& operator=(VectorElement&&) = default;
    std::string m_test{"test"};
};

class Vector {
public:
    Vector() = default;
    Vector(size_t n);
    Vector(const Vector& other);
    Vector(Vector&& other) = default;
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) = default;
    size_t size() const;
    VectorElement& operator[](size_t n);
    void pushBack(const VectorElement& element);
    void erase(size_t n);

private:
    std::unique_ptr<VectorElement[]> m_data{nullptr};
    size_t m_size{0};
    size_t m_allocated{0};
    void copyFromOther(const Vector& other);
};

#endif // VECTOR_H
