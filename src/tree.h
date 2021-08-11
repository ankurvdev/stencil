#pragma once
#include <list>
#include <memory>

template <typename T> struct tree
{
    struct _Node
    {
        _Node(T&& val) : data(std::move(val)) {}
        _Node(_Node const&) = delete;
        _Node()             = delete;
        _Node& operator=(_Node const&) = delete;

        _Node(_Node&& r) = default;
        _Node& operator=(_Node&& r) = default;

        T      data;
        _Node* firstchild{};
        _Node* lastchild{};
        _Node* nextsibling{};
        _Node* prevsibling{};
        _Node* parent{};
    };

    std::list<std::unique_ptr<_Node>> _nodes;    // TODO : avoid wastage

    public:
    tree()              = default;
    tree(tree const& r) = delete;
    tree& operator=(tree const& r) = delete;
    tree(tree&&)                   = default;
    tree& operator=(tree&&) = default;

    tree<T> clone() const { throw std::logic_error("Not implemented"); }
    // tree(tree&& obj) { std::swap(_nodes, obj._nodes); }

    struct iterator
    {
        T const* operator->() const { return &_current->data; }
        T const& operator*() const { return _current->data; }

        bool operator!=(iterator const& r) const { return !((*this) == r); }
        bool operator==(iterator const& r) const { return _current == r._current && _parent == r._parent; }

        iterator& operator++()
        {
            _current = _current->nextsibling;
            return *this;
        }

        private:
        static iterator create(_Node* node) { return iterator(node); }
        static iterator end() { return iterator{}; }

        iterator() = default;
        iterator(_Node* node) : _current(node), _parent(node == nullptr ? nullptr : node->parent) {}
        iterator(_Node* cur, _Node* par) : _current(cur), _parent(par) {}

        _Node* _current{nullptr};
        _Node* _parent{nullptr};
        friend struct tree;
    };

    template <typename TIt> struct range
    {
        TIt _begin;
        TIt _end;

        auto begin() const { return _begin; }
        auto end() const { return _end; }
    };

    iterator rootbegin() const { return _nodes.size() == 0 ? iterator::end() : iterator::create(_nodes.front().get()); }
    iterator rootend() const { return iterator::end(); }

    range<iterator> children(iterator it) const
    {
        return range<iterator>{iterator{it._current->firstchild, it._current}, iterator{nullptr, it._current}};
    }

    // Insert after last child
    iterator addchild(iterator it, T&& data)
    {
        auto item = std::make_unique<_Node>(std::forward<T>(data));
        if (it != iterator::end())
        {
            auto parent       = it._current;
            auto prevsib      = parent->lastchild;
            item->parent      = parent;
            item->prevsibling = prevsib;

            parent->lastchild  = item.get();
            parent->firstchild = parent->firstchild == nullptr ? item.get() : parent->firstchild;
            if (prevsib)
            {
                if (prevsib->nextsibling != nullptr) throw std::logic_error("Next sibling should be empty");
                prevsib->nextsibling = item.get();
            }
        }
        else
        {
            if (_nodes.size() != 0) { throw std::logic_error("Use add_sibling to add multiple roots"); }
        }

        auto retit = iterator::create(item.get());
        _nodes.push_back(std::move(item));
        return retit;
    }

    iterator addsibling(iterator it, T&& data)
    {
        if (it == iterator::end()) { throw std::logic_error("Add Sibling on last not allowed"); }

        auto item = std::make_unique<_Node>(std::forward<T>(data));

        auto crnt = it._current;
        auto prnt = crnt->parent;
        auto next = crnt->nextsibling;

        item->parent      = prnt;
        item->nextsibling = crnt->nextsibling;
        item->prevsibling = crnt;

        crnt->nextsibling = item.get();

        if (prnt != nullptr && prnt->lastchild == crnt) prnt->lastchild = item.get();
        if (next != nullptr) next->prevsibling = item.get();

        auto retit = iterator::create(item.get());

        _nodes.push_back(std::move(item));
        return retit;
    }
};
