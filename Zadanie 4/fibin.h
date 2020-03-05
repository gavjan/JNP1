#ifndef ZADANIE4_FIBIN_H
#define ZADANIE4_FIBIN_H

#include <type_traits>
#include <iostream>


using ull = unsigned long long;

// Wygeneruj identyfikator zmiennej
template<int i>
constexpr ull VarName(const char *s) {
    char c = s[0];

    if (c == '\0')
        return 0;

    if ('a' <= c && c <= 'z')
        c -= 32;

    return c + 33*VarName<i + 1>(s + 1);
}

template<>
constexpr ull VarName<6>(const char *s) {
    return 0;
}

// Obliczanie nazw zmiennych
constexpr ull Var(const char *s) {
    return VarName<0>(s);
}

// Wartość n-tej liczby Fibonacciego w danym typie liczbowym
template<typename T, unsigned n>
struct FibVal {
    static constexpr T val = FibVal<T, n-1>::val + FibVal<T, n-2>::val;
};

template<typename T>
struct FibVal<T, 0> {
    static constexpr int val = 0;
};

template<typename T>
struct FibVal<T, 1> {
    static constexpr int val = 1;
};

// --------
// LITERAŁY
// --------
// Struktura do Przechwytywania zapytań o liczbę Fibonacciego
template<unsigned n>
struct Fib{};
struct True{
};
struct False{
};

// ---------
// WYRAŻENIA
// ---------
struct Expr {};

template<typename T>
struct Lit : Expr{};

template <typename... Expressions>
struct Sum : Expr{};

template<typename L, typename R>
struct Eq : Expr{};
template<ull Name>
struct Ref : Expr{};
template<ull Name, typename Value, typename Expression>
struct Let : Expr{};
template <typename Cond, typename Then, typename Else>
struct If : Expr{};
template<ull Var, typename Body>
struct Lambda : Expr{};
template <typename Fun, typename Param>
struct Invoke : Expr{};

struct NullEnv;
template<ull Name, typename Value, typename RestEnv>
struct Binding {};
template<typename ValueType, typename Lambda, typename Env>
struct Closure{};
template<ull Name, typename Env>
struct EnvDict {};

// Obliczanie wyrażeń
template<
        typename ValueType,
        typename T,
        typename Env
>
struct Eval {};

// "Słownik" ze zmiennymi
template<ull Name, typename Value, typename Env>
struct EnvDict<Name, Binding<Name, Value, Env>> {
    Value typedef res;
};

template<ull Name, ull Name2, typename Value2, typename Env>
struct EnvDict<Name, Binding<Name2, Value2, Env>> {
    typename EnvDict<Name, Env>::res typedef res;
};

template<ull Name>
struct EnvDict<Name, NullEnv> {};

// ----------
// OBLICZENIA
// ----------

// Wynikiem literałów są one same
template<typename ValueType, typename T, typename Env>
struct Eval<ValueType, Lit<T>, Env> {
    T typedef res;
};

template<typename ValueType, unsigned n, typename Env>
struct Eval<ValueType, Lit<Fib<n>>, Env> {
    FibVal<ValueType, n> typedef res;
};

// if (true) = wykonujemy Then
template<typename ValueType, typename Then, typename Else, typename Env>
struct Eval<ValueType, If<True, Then, Else>, Env> {
    typename Eval<ValueType, Then, Env>::res typedef res;
};

// if (false) = wykonujemy Else
template<typename ValueType, typename Then, typename Else, typename Env>
struct Eval<ValueType, If<False, Then, Else>, Env> {
    typename Eval<ValueType, Else, Env>::res typedef res;
};

// Musimy obliczyć wartość warunku
template<typename ValueType, typename Cond, typename Then, typename Else, typename Env>
struct Eval<ValueType, If<Cond, Then, Else>, Env> {
    typename Eval<ValueType, If<typename Eval<ValueType, Cond, Env>::res, Then, Else>, Env>::res typedef res;
};

// "Zamknij" Lambdę
template<typename ValueType, ull Var, typename Body, typename Env>
struct Eval<ValueType, Lambda<Var, Body>, Env> {
    Closure<ValueType, Lambda<Var, Body>, Env> typedef res;
};

// Jeśli typ jest Lambdą, to opakowuje go, wpp. zwraca niezmieniony typ Value
template<typename ValueType, typename Value, typename Env>
struct Closurise {
    Value typedef res;
};

template<typename ValueType, ull Var, typename Body, typename Env>
struct Closurise<ValueType, Lambda<Var, Body>, Env> {
    Closure<ValueType, Lambda<Var, Body>, Env> typedef res;
};

// Wykonaj funkcję dla danego argumentu
template<typename ValueType, ull Var, typename Body, typename Env, typename Value>
struct Invoke<Closure<ValueType, Lambda<Var, Body>, Env>, Value> {
    typename Eval<ValueType, Body, Binding<
            Var,
            typename Closurise<ValueType, Value, Env>::res,
            Env>>::res typedef res;
};

// Eval<Closure<...
template<typename ValueType, typename Fun, typename Env1, typename Env2>
struct Eval<ValueType, Closure<ValueType, Fun, Env1>, Env2> {
    Closure<ValueType, Fun, Env1> typedef res;
};

// Eval<Invoke<...
template<typename ValueType, typename Fun, typename Param, typename Env>
struct Eval<ValueType, Invoke<Fun, Param>, Env> {
    typename Invoke<
            typename Eval<ValueType, Fun, Env>::res,
            Param
            >::res typedef res;
};

// Eval<Ref<...
template<typename ValueType, ull Name, typename Env>
struct Eval<ValueType, Ref<Name>, Env> {
    typename Eval<ValueType, typename EnvDict<Name, Env>::res, Env>::res typedef res;
};

// Eval<Eq<...
template<typename ValueType, typename Expr1, typename Expr2, typename Env>
struct Eval<ValueType, Eq<Expr1, Expr2>, Env> {
    typename std::conditional<Eval<ValueType, Expr1, Env>::res::val == Eval<ValueType, Expr2, Env>::res::val,
                                True,
                                False>::type typedef res;

};

// Eval<Let<...
template<typename ValueType, ull Name, typename Value, typename Expression, typename Env>
struct Eval<ValueType, Let<Name, Value, Expression>, Env> {
    typename Eval<ValueType, Expression, Binding<
            Name,
            typename Closurise<ValueType, Value, Env>::res,
            Env>>::res typedef res;
};

// Wynik sumy wyrażeń to suma wyników wyrażeń
template<typename ValueType, typename Env, typename Expression, typename... Args >
struct SumRes {
    static constexpr int val = Eval<ValueType, Expression, Env>::res::val + SumRes<ValueType, Env, Args...>::val;
};

template<typename ValueType, typename Env, typename Expression1, typename Expression2>
struct SumRes<ValueType, Env, Expression1, Expression2> {
    static constexpr int val = Eval<ValueType, Expression1, Env>::res::val + Eval<ValueType, Expression2, Env>::res::val;
};

template <typename Arg>
struct Inc1 : Expr {};
template <typename Arg>
struct Inc10 : Expr {};

template<typename ValueType, typename... Expressions, typename Env>
struct Eval<ValueType, Sum<Expressions...>, Env> {
    SumRes<ValueType, Env, Expressions...> typedef res;
};

template<typename ValueType, typename Expression, typename Env>
struct Eval<ValueType, Inc1<Expression>, Env> {
    typename Eval<ValueType, Sum<Lit<Fib<1>>, Expression>, Env>::res typedef res;
};

template<typename ValueType, typename Expression, typename Env>
struct Eval<ValueType, Inc10<Expression>, Env> {
    typename Eval<ValueType, Sum<Lit<Fib<10>>, Expression>, Env>::res typedef res;
};

// ---------------
// STRUKTURA FIBIN
// ---------------

template<
        typename T,
        bool = std::is_integral<T>::value
> struct Fibin;

// Dla nieobsługiwanych typów
template<typename ValueType>
struct Fibin<ValueType, false> {
    template<
            typename E,
            typename = typename std::enable_if<std::is_base_of<Expr, E>::value, E>::type
    >
    static void eval() {
        std::cout << "Fibin doesn't support: " << typeid(ValueType).name() << std::endl;
    }
};

// Dla typów liczbowych
template<typename ValueType>
struct Fibin<ValueType, true> {
public:
    template<
            typename E,
            typename = typename std::enable_if<std::is_base_of<Expr, E>::value, E>::type
    >
    constexpr static ValueType eval() {
        return Eval<ValueType, E, NullEnv>::res::val;
    }
};

#endif //ZADANIE4_FIBIN_H
