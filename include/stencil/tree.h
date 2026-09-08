#pragma once
#include <list>
#include <memory>

template <typename T> struct Tree
{
    struct _Node
    {
        explicit _Node(T&& val) : data(std::move(val)) {}
        _Node(_Node const&)            = delete;
        _Node()                        = delete;
        _Node& operator=(_Node const&) = delete;

        _Node(_Node&& r)            = default;
        _Node& operator=(_Node&& r) = default;

        T      data;
        _Node* firstchild{};
        _Node* lastchild{};
        _Node* nextsibling{};
        _Node* prevsibling{};
        _Node* parent{};
    };

    std::list<std::unique_ptr<_Node>> nodes;    // TODO : avoid wastage

    public:
    Tree()                         = default;
    Tree(Tree const& r)            = delete;
    Tree& operator=(Tree const& r) = delete;
    Tree(Tree&&)                   = default;
    Tree& operator=(Tree&&)        = default;

    [[noreturn]] Tree<T> Clone() const { throw std::logic_error("Not implemented"); }
    // tree(tree&& obj) { std::swap(_nodes, obj._nodes); }

    struct Iterator
    {
        T const* operator->() const { return &_current->data; }
        T const& operator*() const { return _current->data; }

        bool operator!=(Iterator const& r) const { return !((*this) == r); }
        bool operator==(Iterator const& r) const { return _current == r._current && _parent == r._parent; }

        Iterator& operator++()
        {
            _current = _current->nextsibling;
            return *this;
        }

        private:
        static Iterator create_(_Node* node) { return Iterator(node); }
        static Iterator end_() { return Iterator{}; }

        Iterator() = default;
        explicit Iterator(_Node* node) : _current(node), _parent(node == nullptr ? nullptr : node->parent) {}
        Iterator(_Node* cur, _Node* par) : _current(cur), _parent(par) {}

        _Node* _current{nullptr};
        _Node* _parent{nullptr};
        friend struct Tree;
    };

    template <typename TIt> struct Range
    {
        TIt begin;
        TIt end;

        auto Begin() const { return begin; }
        auto End() const { return end; }
    };

    Iterator Rootbegin() const { return nodes.size() == 0 ? Iterator::end() : Iterator::create(nodes.front().get()); }
    Iterator Rootend() const { return Iterator::end(); }

    Range<Iterator> Children(Iterator it) const
    {
        return Range<Iterator>{Iterator{it._current->firstchild, it._current}, Iterator{nullptr, it._current}};
    }

    // Insert after last child
    Iterator Addchild(Iterator it, T&& data)
    {
        auto item = std::make_unique<_Node>(std::forward<T>(data));
        if (it != Iterator::end())
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
            if (nodes.size() != 0) { throw std::logic_error("Use add_sibling to add multiple roots"); }
        }

        auto retit = Iterator::create(item.get());
        nodes.push_back(std::move(item));
        return retit;
    }

    Iterator Addsibling(Iterator it, T&& data)
    {
        if (it == Iterator::end()) { throw std::logic_error("Add Sibling on last not allowed"); }

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

        auto retit = Iterator::create(item.get());

        nodes.push_back(std::move(item));
        return retit;
    }
};
