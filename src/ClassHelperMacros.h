
#define DELETE_MOVE_ASSIGNMENT(name)        \
    name(name const&) = default;            \
    name(name&&)      = default;            \
    name& operator=(name const&) = default; \
    name& operator=(name&&) = delete

#define DELETE_MOVE_AND_COPY_ASSIGNMENT(name) \
    name(name const&) = default;              \
    name(name&&)      = default;              \
    name& operator=(name const&) = delete;    \
    name& operator=(name&&) = delete

#define DELETE_COPY_AND_MOVE(name)         \
    name(name const&) = delete;            \
    name(name&&)      = delete;            \
    name& operator=(name const&) = delete; \
    name& operator=(name&&) = delete

#define DELETE_COPY_DEFAULT_MOVE(name)     \
    name(name const&) = delete;            \
    name(name&&)      = default;           \
    name& operator=(name const&) = delete; \
    name& operator=(name&&) = default

#define ONLY_MOVE_CONSTRUCT(name)          \
    name(name const&) = delete;            \
    name(name&&)      = default;           \
    name& operator=(name const&) = delete; \
    name& operator=(name&&) = delete

#if !defined TODO
#define TODO(...) throw std::logic_error("Not Implemented")
#endif
