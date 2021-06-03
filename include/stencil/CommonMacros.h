#define SUPPRESS_WARNINGS_START                                                                 \
    _Pragma("warning(push, 3)") _Pragma("clang diagnostic push") _Pragma("GCC diagnostic push") \
        _Pragma("clang diagnostic ignored \"-Weverything\"")

#define SUPPRESS_WARNINGS_END _Pragma("GCC diagnostic pop") _Pragma("clang diagnostic pop") _Pragma("warning(pop)")

#define CLASS_DEFAULT_COPY_AND_MOVE(name)   \
    name(name const&) = default;            \
    name(name&&)      = default;            \
    name& operator=(name const&) = default; \
    name& operator=(name&&) = delete

#define CLASS_DELETE_MOVE_ASSIGNMENT(name)  \
    name(name const&) = default;            \
    name(name&&)      = default;            \
    name& operator=(name const&) = default; \
    name& operator=(name&&) = delete

#define CLASS_DELETE_MOVE_AND_COPY_ASSIGNMENT(name) \
    name(name const&) = default;                    \
    name(name&&)      = default;                    \
    name& operator=(name const&) = delete;          \
    name& operator=(name&&) = delete

#define CLASS_DELETE_COPY_AND_MOVE(name)   \
    name(name const&) = delete;            \
    name(name&&)      = delete;            \
    name& operator=(name const&) = delete; \
    name& operator=(name&&) = delete

#define CLASS_DELETE_COPY_DEFAULT_MOVE(name) \
    name(name const&) = delete;              \
    name(name&&)      = default;             \
    name& operator=(name const&) = delete;   \
    name& operator=(name&&) = default

#define CLASS_ONLY_MOVE_CONSTRUCT(name)    \
    name(name const&) = delete;            \
    name(name&&)      = default;           \
    name& operator=(name const&) = delete; \
    name& operator=(name&&) = delete