Should we have

Approach : 1

```cpp
template <typename T> struct Visitor<Transaction<T>>
{
    void Visit();
}
```

OR

Approach : 2

```cpp
template <typename T>
struct Transaction<T> {
...
void VisitChanges();

}

```

# Approach : 1

Easier to manage.
    Private variables are much easier accessible. Approach 2 requires Visitor access to private members

# Approach : 2

Cleaner
    The transaction class becomes more of a marker

Easier to have a unified transaction stream
