#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <functional>
#include <vector>
#include <limits>
#include <memory>


class lookup_error: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Nie znaleziono danego klucza";
    }
};

namespace {
    template<class K, class V, class Hash = std::hash<K>>
    struct iom_container;

    template<class K, class V, class Hash = std::hash<K>>
    class iom_node;
}

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {
public:
    /// Iterator
    class iterator {
    public:
        using it = iterator;
        using node = iom_node<K, V, Hash>;

        explicit iterator(node *n) : pos(n) {}
        iterator(const it &other) : pos(other.pos) {}
        it operator++() {
            pos = pos->_nextInOrder;
            return *this;
        }
        bool operator==(const it &other) {
            return (!other.pos && !this->pos) || (other.pos && this->pos && *other.pos == *this->pos);
        }
        bool operator==(const it &other) const {
            return (!other.pos && !this->pos) || (other.pos && this->pos && *other.pos == *this->pos);
        }
        bool operator!=(const it &other) {
            return !(*this == other);
        }
        bool operator!=(const it &other) const {
            return !(*this == other.pos);
        }
        const node* operator->() const {
            return this->pos;
        }
        const node& operator*() const {
            return *(this->pos);
        }
    private:
        node *pos;
    };

    using it = iterator;
    using node = iom_node<K, V, Hash>;

    insertion_ordered_map();
    ~insertion_ordered_map() noexcept;
    insertion_ordered_map(const insertion_ordered_map &other);
    insertion_ordered_map(insertion_ordered_map &&other) noexcept;

    bool insert(K const &k, V const &v);
    void erase(K const &k);

    void merge(insertion_ordered_map &other);
    insertion_ordered_map<K, V, Hash> &operator=(insertion_ordered_map<K, V, Hash> other);

    V &at(K const &k);
    V const &at(K const &k) const;

    V &operator[](K const &k);
    size_t size() const;
    bool empty() const;

    void clear();
    bool contains(K const &k) const;

    it begin();
    it begin() const;
    it end();
    it end() const;

private:
    iom_container<K, V, Hash> *_data;
    const static size_t Unshareable = std::numeric_limits<size_t>::max();

    void AboutToModify(bool bMarkUnshareable = false);
    node* getNode(K k) const;
    void moveNodeToEnd(K k);
    bool insert(K const &k, V const &v, bool bMarkUnshareable);
};

namespace {
    template<class K, class V, class Hash>
    class iom_node {
    public:
        K first;
        V second;

        iom_node(const K &k, const V &v) :
        first(k),
        second(v),
        _nextOnList(nullptr),
        _nextInOrder(nullptr),
        _prevInOrder(nullptr) {};

        iom_node(const K &k, const V &v, iom_node<K, V, Hash>* next) :
        first(k),
        second(v),
        _nextOnList(next),
        _nextInOrder(nullptr),
        _prevInOrder(nullptr) {};

        ~iom_node() = default;
        bool operator==(iom_node<K, V, Hash> n) {
            return this->first == n.first && this->second == n.second;
        }

    private:
        iom_node *_nextOnList;
        iom_node *_nextInOrder;
        iom_node *_prevInOrder;

        friend class insertion_ordered_map<K, V, Hash>;
        friend class iom_container<K, V, Hash>;
    };

    /// Iom_container
    template<class K, class V, class Hash>
    struct iom_container {
        using node = iom_node<K, V, Hash>;
        using it = typename insertion_ordered_map<K, V, Hash>::iterator;

        explicit iom_container(size_t container_capacity = DEFAULT_ARRAY_SIZE);
        iom_container(const iom_container &other, size_t diffCapacity = 0);
        ~iom_container() noexcept;

        const static size_t DEFAULT_ARRAY_SIZE = 16;
        constexpr const static float DEFAULT_LOAD_FACTOR = 0.75f;

        size_t capacity;
        inline size_t threshold();
        void clear() noexcept;
        bool insert(K const &k, V const &v);
        void erase(K const &k);

        std::vector<node*> arr;
        size_t current_size;

        size_t refs;
        node* firstNode;
        node* lastNode;

        it begin();
        it begin() const;
        it end();
        it end() const;

        node* getNode(K k) const;
        void moveNodeToEnd(node *t);
        void rehashUp();
        void rehashDown();
        void rehash();
    };
}

/// IOM_CONTAINER
template <class K, class V, class Hash>
iom_container<K, V, Hash>::iom_container(size_t container_capacity) :
        capacity(container_capacity),
        current_size(0),
        refs(1),
        firstNode(nullptr),
        lastNode(nullptr) {
    arr = std::vector<iom_node<K, V, Hash>*>(capacity);
}

template <class K, class V, class Hash>
iom_container<K, V, Hash>::~iom_container() noexcept {
    clear();
}

template<class K, class V, class Hash>
size_t iom_container<K, V, Hash>::threshold() {
    return (capacity * DEFAULT_LOAD_FACTOR);
}

template <class K, class V, class Hash>
void iom_container<K, V, Hash>::clear() noexcept {
    iom_node<K, V, Hash> *node = firstNode;
    while (node != nullptr) {
        auto *next = node->_nextInOrder;
        delete node;
        node = next;
    }

    current_size = 0;
    arr = std::vector<iom_node<K, V, Hash>*>(DEFAULT_ARRAY_SIZE);
    capacity = DEFAULT_ARRAY_SIZE;

    firstNode = nullptr;
    lastNode = nullptr;
}

template <class K, class V, class Hash>
bool iom_container<K, V, Hash>::insert(K const &key, V const &value) {
    K k(key);
    V v(value);

    size_t hash_code = Hash{}(k) % capacity;

    if (arr[hash_code] != nullptr) {
        auto **node = &arr[hash_code];
        iom_node<K, V, Hash> *prev_on_list = nullptr;

        // Znajdź pozycję szukanego węzła
        while (*node != nullptr && (*node)->first != k)
            prev_on_list = *node, *node = (*node)->_nextOnList;

        // Przestaw istniejący element
        if (*node != nullptr) {
            // Przełącz elementy na liście iteratorów
            moveNodeToEnd(*node);

            return false;
        }
        // Dodaj nowy element
        else {
            auto new_node = std::make_unique<iom_node<K, V, Hash>>(k, v);
            if (prev_on_list)
                prev_on_list->_nextOnList = new_node.get();

            if (++current_size == 1)
                firstNode = new_node.get(), lastNode = new_node.get();
            else
                moveNodeToEnd(new_node.get());

            new_node.release();
        }
    }
    else {
        auto new_node = std::make_unique<iom_node<K, V, Hash>>(k, v);
        arr[hash_code] = new_node.get();

        if (++current_size == 1)
            firstNode = new_node.get(), lastNode = new_node.get();
        else
            moveNodeToEnd(new_node.get());

        new_node.release();
    }

    if (current_size >= threshold()) {
        this->rehashUp();
    }

    return true;
}

template<class K, class V, class Hash>
iom_container<K, V, Hash>::iom_container(const iom_container &other, size_t diffCapacity) :
capacity(diffCapacity == 0 ? other.capacity : diffCapacity),
current_size(0),
refs(1) {
    try {
        arr = std::vector<iom_node<K, V, Hash> *>(capacity);

        for (auto it = other.begin(), end = other.end(); it != end; ++it)
            insert(it->first, it->second);
    }
    // Przechwyć wyjątek
    catch (std::exception &e) {
        // Usuń wszystkie dotychczas dodane wierzchołki
        clear();

        // Przekaż informację, że konstruktor zakończył się niepowodzeniem
        throw e;
    }
}

template<class K, class V, class Hash>
void iom_container<K, V, Hash>::erase(K const &k) {
    size_t hash_code = Hash{}(k) % capacity;

    if (arr[hash_code] != nullptr) {
        auto *node = arr[hash_code];
        iom_node<K, V, Hash> *prev_on_list = nullptr;

        while (node != nullptr && node->first != k)
            prev_on_list = node, node = node->_nextInOrder;

        if (node != nullptr) {
            // Przepnij głowę listy kubełka
            if (arr[hash_code] == node)
                arr[hash_code] = nullptr;

            // Przepnij it
            if (node->_prevInOrder)
                node->_prevInOrder->_nextInOrder = node->_nextInOrder;
            if (node->_nextInOrder)
                node->_nextInOrder->_prevInOrder = node->_prevInOrder;

            // Zmień węzły zaczynające i kończące it
            if (node == firstNode)
                firstNode = node->_nextInOrder;
            if (node == lastNode)
                lastNode = node->_prevInOrder;

            // Usuń element z listy
            auto next_on_list = node->_nextOnList;
            delete node;

            if (prev_on_list != nullptr)
                prev_on_list->_nextOnList = next_on_list;

        }
        else
            throw lookup_error();

        current_size--;
    }
    else
        throw lookup_error();

    if (capacity > DEFAULT_ARRAY_SIZE && 3 * current_size < threshold())
        rehashDown();
}

template<class K, class V, class Hash>
typename iom_container<K, V, Hash>::it iom_container<K, V, Hash>::end() {
    return it(nullptr);
}

template<class K, class V, class Hash>
typename iom_container<K, V, Hash>::it iom_container<K, V, Hash>::begin() {
    return iom_container<K, V, Hash>::it(firstNode);
}

template<class K, class V, class Hash>
typename iom_container<K, V, Hash>::it iom_container<K, V, Hash>::begin() const {
    return it(firstNode);
}

template<class K, class V, class Hash>
typename iom_container<K, V, Hash>::it iom_container<K, V, Hash>::end() const {
    return it(nullptr);
}

template<class K, class V, class Hash>
typename iom_container<K, V, Hash>::node *iom_container<K, V, Hash>::getNode(K k) const {
    node *res = nullptr;
    auto hash_code = Hash{}(k);

    if (arr[hash_code] != nullptr) {
        res = arr[hash_code];

        while (res != nullptr && res->first != k)
            res = res->_nextOnList;
    }

    return res;
}

template<class K, class V, class Hash>
void iom_container<K, V, Hash>::moveNodeToEnd(iom_container::node *t) {
    // Musimy przepiąć
    if (t != nullptr && (t != firstNode || t != lastNode)) {
        // Zmień firtsNode i lastNode
        if (t == firstNode)
            firstNode = t->_nextInOrder;

        if (t == lastNode)
            lastNode = t->_prevInOrder;

        // "Omiń" węzeł t w iteracji
        if (t->_prevInOrder)
            t->_prevInOrder->_nextInOrder = t->_nextInOrder;
        if (t->_nextInOrder)
            t->_nextInOrder->_prevInOrder = t->_prevInOrder;

        // Dołącz t na koniec iteracji
        if (lastNode)
            lastNode->_nextInOrder = t;
        t->_prevInOrder = lastNode;

        lastNode = t;
        t->_nextInOrder = nullptr;
    }
}

template<class K, class V, class Hash>
void iom_container<K, V, Hash>::rehashUp() {
    this->capacity *= 2;
    this->arr = std::vector<iom_node<K, V, Hash>*>(this->capacity);

    rehash();
}

template<class K, class V, class Hash>
void iom_container<K, V, Hash>::rehash() {
    auto node = firstNode;
    size_t hash_code;

    while (node != nullptr) {
        node->_nextOnList = nullptr;

        hash_code = Hash{}(node->first) % capacity;

        if (arr[hash_code] != nullptr) {
            iom_node<K, V, Hash> *head = arr[hash_code];
            iom_node<K, V, Hash> *prev = nullptr;

            while (head != nullptr)
                prev = head, head = head->_nextOnList;

            if (prev)
                prev->_nextOnList = node;
        }
        else
            arr[hash_code] = node;

        node = node->_nextInOrder;
    }
}

template<class K, class V, class Hash>
void iom_container<K, V, Hash>::rehashDown() {
    this->capacity /= 2;
    this->arr = std::vector<iom_node<K, V, Hash>*>(this->capacity);

    rehash();
}

/// INSERTION_ORDERED_MAP
template<class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map() :
        _data(new iom_container<K, V, Hash>()) {}

template<class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::~insertion_ordered_map() noexcept {
    if (_data != nullptr && (_data->refs == Unshareable || --_data->refs < 1)) {
        delete _data;
    }
}

template<class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(const insertion_ordered_map &other) {
    if (other._data->refs != Unshareable) {
        _data = other._data;
        _data->refs++;
    }
    else {
        _data = new iom_container<K, V, Hash>(*other._data);
    }
}

template<class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map &&other) noexcept :
_data(std::move(other._data)) {
    _data->current_size = other._data->current_size;
    _data->capacity = other._data->capacity;
    _data->lastNode = other._data->lastNode;
    _data->firstNode = other._data->firstNode;
    _data->refs = other._data->refs;

    other._data = nullptr;
}

template<class K, class V, class Hash>
size_t insertion_ordered_map<K, V, Hash>::size() const {
    return _data ? _data->current_size : 0;
}

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::empty() const {
    return size() == 0;
}

template<class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::clear() {
    if (!_data)
        _data = new iom_container<K, V, Hash>();

    AboutToModify();
    if (_data)
        _data->clear();
}

template<class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::AboutToModify(bool bMarkUnshareable) {
    if (_data->refs > 1 && _data->refs != Unshareable) {
        auto new_container = std::make_unique<iom_container<K, V, Hash>>(*_data);
        --_data->refs;
        _data = new_container.get();
        
        new_container.release();
    }

    _data->refs = bMarkUnshareable ? Unshareable : 1;
}

template<class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::erase(K const &k) {
    AboutToModify();
    if (_data)
        _data->erase(k);
}

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::insert(K const &k, V const &v, bool bMarkUnshareable) {
    AboutToModify(bMarkUnshareable);
    return _data ? _data->insert(k, v) : false;
}

template<class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::at(const K &k) {
    AboutToModify(true);
    iom_node<K, V, Hash>* node = getNode(k);

    if (!node)
        throw lookup_error();

    return node->second;
}

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::contains(const K &k) const {
    iom_node<K, V, Hash> *node = getNode(k);

    return node != nullptr;
}

template<class K, class V, class Hash>
V const &insertion_ordered_map<K, V, Hash>::at(const K &k) const {
    AboutToModify(true);
    iom_node<K, V, Hash> *node = getNode(k);

    if (!node)
        throw lookup_error();

    return node->second;
}

template<class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::operator[](const K &k) {
    AboutToModify(true);

    if (!contains(k))
        insert(k, V(), true);

    return at(k);
}

template<class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::iterator insertion_ordered_map<K, V, Hash>::begin() {
    return _data ? _data->begin() : iterator(nullptr);
}

template<class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::iterator insertion_ordered_map<K, V, Hash>::end() {
    return _data ? _data->end() : iterator(nullptr);
}

template<class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::iterator insertion_ordered_map<K, V, Hash>::begin() const {
    return _data ? _data->begin() : iterator(nullptr);
}

template<class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::iterator insertion_ordered_map<K, V, Hash>::end() const {
    return _data ? _data->end() : iterator(nullptr);
}

template<class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::merge(insertion_ordered_map<K, V, Hash> &other) {
    // Stwórz kopię obecnej haszmapy
    insertion_ordered_map<K, V, Hash> tmp(*this);

    // Połącz obie haszmapy
    for (auto it = other.begin(); it != other.end(); ++it)
        tmp.insert(it->first, it->second);

    // Zamień tymczasową haszmapę z naszą haszmapą
    std::swap(tmp._data, this->_data);
}

template<class K, class V, class Hash>
insertion_ordered_map<K, V, Hash> &insertion_ordered_map<K, V, Hash>::operator=(insertion_ordered_map<K, V, Hash> other) {
    if (other._data != this->_data) {
        insertion_ordered_map<K, V, Hash> tmp(other);
        std::swap(tmp._data, this->_data);
    }

    return *this;
}

template<class K, class V, class Hash>
typename insertion_ordered_map<K, V, Hash>::node *insertion_ordered_map<K, V, Hash>::getNode(K k) const {
    return _data ? _data->getNode(k) : nullptr;
}

template<class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::moveNodeToEnd(K k) {
    AboutToModify();
    if (_data)
        _data->moveNodeToEnd(getNode(k));
}

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::insert(const K &k, const V &v) {
    return insert(k, v, false);
}

#endif //INSERTION_ORDERED_MAP_H
