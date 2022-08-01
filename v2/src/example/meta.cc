#include <iostream>


template <typename T, typename... Args>
struct TypesCmp
{
    enum {
        value = false
    };
};

template <typename T, typename U, typename... Args>
struct TypesCmp<T, U, Args...>
{
    enum {
        value = false | TypesCmp<T, Args...>::value
    };
};

template <typename T, typename... Args>
struct TypesCmp<T, T, Args...>
{
    enum {
        value = true | TypesCmp<T, Args...>::value
    };
};

int main()
{
    if (TypesCmp<int, double, int&>::value) {
        std::cout << "Issame" << std::endl;
    } else {
        std::cout << "Is not same" << std::endl;
    }
}