namespace Stencil
{
template <typename T> struct Mutators;

template <typename T> struct Accessors;

template <typename T> struct Mutators<std::vector<T>>
{
    static void add(std::vector<T>& arr, T&& obj) { arr.push_back(std::move(obj)); }
    static void remove(std::vector<T>& arr, size_t index)
    {
        arr.erase(arr.begin() + static_cast<typename std::vector<T>::difference_type>(index));
    }

    template <typename TLambda> static void remove_matching(std::vector<T>& arr, TLambda&& lambda)
    {
        for (auto it = arr.begin(); it != arr.end(); ++it)
        {
            if (lambda(*it))
            {
                arr.erase(it);
                return;
            }
        }
    }
};

template <typename T> struct Accessors<std::vector<T>>
{
    static T const& at(std::vector<T> const& arr, size_t index) { return arr.at(index); }
};

template <typename T, size_t N> struct Accessors<std::array<T, N>>
{
    static T const& at(std::array<T, N> const& arr, size_t index) { return arr.at(index); }
};

}    // namespace Stencil