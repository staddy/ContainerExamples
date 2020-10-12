#include "linkedlist.h"

size_t constexpr MAX_SIZE = 100000;

void printLinkedList(const LinkedList &list_) {
    std::cout << "LinkedList with size " << list_.size() << ":\n";
    bool first = true;
    for (const auto& element : list_) {
        if (!first) {
            std::cout << ", ";
        }
        first = false;
        std::cout << element.n();
    }
    std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& stream_, LinkedList& list_) {
    uint64_t s = list_.size();
    stream_.write(reinterpret_cast<char*>(&s), sizeof(s));
    for (uint64_t i = 0; i < s; ++i) {
        int32_t n = static_cast<int32_t>(list_[i].n());
        stream_.write(reinterpret_cast<char*>(&n), sizeof(n));
    }
    return stream_;
}
std::istream& operator>>(std::istream& stream_, LinkedList& list_) {
    list_.clear();
    uint64_t s{0};
    stream_.read(reinterpret_cast<char*>(&s), sizeof(s));
    if (s > MAX_SIZE) {
        return stream_;
    }
    for (uint64_t i = 0; i < s; ++i) {
        int32_t n{0};
        stream_.read(reinterpret_cast<char*>(&n), sizeof(n));
        list_.pushBack(n);
    }
    return stream_;
}

LinkedList::LinkedList(size_t n_) {
    std::cout << "LinkedList(size_t n_)\n";
    for (size_t i = 0; i < n_; ++i) {
        pushBack(Element());
    }
}

LinkedList::LinkedList(const LinkedList &other_) {
    std::cout << "Copy constructor\n";
    for (size_t i = 0; i < other_.size(); ++i) {
        this->pushBack(other_[i].n());
    }
}

LinkedList::LinkedList(LinkedList &&other_) {
    std::cout << "Move constructor\n";
    this->m_first = other_.m_first;
    this->m_last = other_.m_last;
    this->m_size = other_.m_size;
    other_.m_first = other_.m_last = nullptr;
    other_.m_size = 0;
}

LinkedList &LinkedList::operator=(LinkedList &&other_) {
    if (this == &other_) {
        return *this;
    }
    std::cout << "LinkedList& operator=(LinkedList&& other_)\n";
    this->clear();
    this->m_first = other_.m_first;
    this->m_last = other_.m_last;
    this->m_size = other_.m_size;
    other_.m_first = other_.m_last = nullptr;
    other_.m_size = 0;
    return *this;
}

void LinkedList::pushBack(const Element &element_) {
    if (m_last == nullptr) {
        assert(m_size == 0);
        assert(m_first == nullptr);
        m_first = new ElementWrapper(element_);
        m_last = m_first;
    } else {
        assert(m_last->right == nullptr);
        m_last = (m_last->right = new ElementWrapper(element_, m_last, nullptr));
    }
    ++m_size;
}

void LinkedList::pushFront(const Element &element_) {
    if (m_last == nullptr) {
        assert(m_size == 0);
        assert(m_first == nullptr);
        m_first = new ElementWrapper(element_);
        m_last = m_first;
    } else {
        assert(m_first->left == nullptr);
        m_first = (m_first->left = new ElementWrapper(element_, nullptr, m_first));
    }
    ++m_size;
}

void LinkedList::popBack() {
    if (m_last == nullptr) {
        assert(m_size == 0);
        assert(m_first == nullptr);
        return;
    }
    if (m_first == m_last) {
        assert(m_first->left == nullptr);
        assert(m_first->right == nullptr);
        assert(m_size == 1);
        delete m_first;
        m_first = (m_last = nullptr);
    } else {
        auto tmp = m_last;
        m_last = m_last->left;
        m_last->right = nullptr;
        delete tmp;
    }
    --m_size;
}

void LinkedList::popFront() {
    if (m_last == nullptr) {
        assert(m_size == 0);
        assert(m_first == nullptr);
        return;
    }
    if (m_first == m_last) {
        assert(m_first->left == nullptr);
        assert(m_first->right == nullptr);
        assert(m_size == 1);
        delete m_first;
        m_first = (m_last = nullptr);
    } else {
        auto tmp = m_first;
        m_first = m_first->right;
        m_first->left = nullptr;
        delete tmp;
    }
    --m_size;
}

size_t LinkedList::size() const { return m_size; }

Element &LinkedList::operator[](size_t n_) {
    return const_cast<Element&>(const_cast<const LinkedList&>(*this)[n_]);
}

void LinkedList::clear() {
    if (m_size == 0) {
        assert(m_first == nullptr);
        assert(m_last == nullptr);
        return;
    }
    auto tmp = m_first;
    assert(tmp != nullptr);
    for (size_t i = 0; i < m_size; ++i) {
        assert(tmp != nullptr);
        auto tmp2 = tmp;
        tmp = tmp->right;
        delete tmp2;
    }
    m_first = m_last = nullptr;
    m_size = 0;
}

bool LinkedList::operator==(const LinkedList &other_) {
    if (m_size != other_.m_size) {
        return false;
    }
    for (size_t i = 0; i < m_size; ++i) {
        if ((*this)[i].n() != other_[i].n()) {
            return false;
        }
    }
    return true;
}

bool LinkedList::operator!=(const LinkedList &other_) {
    return !((*this) == other_);
}

LinkedList::Iterator LinkedList::begin() {
    return m_first;
}

LinkedList::Iterator LinkedList::end() {
    return nullptr;
}

LinkedList::Iterator LinkedList::begin() const {
    return m_first;
}

LinkedList::Iterator LinkedList::end() const {
    return nullptr;
}

const Element &LinkedList::operator[](size_t n_) const {
    if (n_ >= m_size) {
        throw std::out_of_range("n_ >= m_size");
    }
    auto tmp = m_first;
    assert(tmp != nullptr);
    for (size_t i = 0; i < n_; ++i) {
        assert(tmp != nullptr);
        tmp = tmp->right;
    }
    return tmp->element;
}

LinkedList &LinkedList::operator=(const LinkedList &other_) {
    if (this == &other_) {
        return *this;
    }
    std::cout << "LinkedList& operator=(const LinkedList& other_)\n";
    this->clear();
    for (size_t i = 0; i < other_.size(); ++i) {
        this->pushBack(other_[i].n());
    }
    return *this;
}

LinkedList::ElementWrapper::ElementWrapper(const Element &element_, LinkedList::ElementWrapper *left_, LinkedList::ElementWrapper *right_) :
    element{element_}, left{left_}, right{right_} { }

LinkedList::Iterator::Iterator(LinkedList::ElementWrapper *element_) : element{element_} { }

bool LinkedList::Iterator::operator!=(const LinkedList::Iterator &other_) {
    return element != other_.element;
}

LinkedList::Iterator LinkedList::Iterator::operator++() {
    (*this)++;
    return *this;
}

Element &LinkedList::Iterator::operator*() {
    return element->element;
}

void LinkedList::Iterator::operator++(int) {
    element = element->right;
}
