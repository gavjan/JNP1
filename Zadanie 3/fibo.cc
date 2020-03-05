#include <iostream>
#include <algorithm>
#include <cassert>
#include <climits>

#include "fibo.h"

using namespace std;

Fibo::Fibo() {
    fibits.push_back(false);
}

Fibo::Fibo(const string &str) {
    assert(!str.empty());
    assert(is_string_correct(str));

    size_t i, j = 0;

    fibits = vector<bool> (str.size() - j);

    i = fibits.size();

    while (i > 0) {
        i--;
        fibits.at(i) = ((str.at(j) == '0') ? false : true);
        j++;
    }

    this->normalize();
}

Fibo::Fibo(const char* str) : Fibo(string(str)) {
    assert(str != NULL);
}

Fibo::Fibo(const unsigned long long n) {
    assert(n <= ULLONG_MAX);
    static vector<unsigned long long> fibonacci_numbers = {1, 2};

    int i = 0;
    size_t last_index = fibonacci_numbers.size() - 1;
    vector<unsigned long long >::iterator up;

    // dodajemy potrzebne liczby fibonacciego
    while (fibonacci_numbers.at(last_index) <= n) {
        fibonacci_numbers.push_back(fibonacci_numbers.at(last_index)
                                    + fibonacci_numbers.at(last_index - 1));
        last_index++;
    }

    up = upper_bound(fibonacci_numbers.begin(), fibonacci_numbers.end(), n);
    up--; // cofamy na najwiÄkszy mniejszy element
    i = up - fibonacci_numbers.begin();

    fibits = vector<bool> (i + 1);

    unsigned long long number = n;
    while (i >= 0) {
        if (fibonacci_numbers.at(i) <= number) {
            fibits.at(i) = true;
            number -= fibonacci_numbers.at(i);
        } else {
            fibits.at(i) = false;
        }
        i--;
    }

    if (n == 0) {
        fibits.push_back(false);
    }
}

Fibo::Fibo(int n) : Fibo((unsigned long long) n) {
    assert(n >= 0);
}

Fibo::Fibo(long n) : Fibo((unsigned long long) n) {
    assert(n >= 0);
}

Fibo::Fibo(long long n) : Fibo((unsigned long long) n) {
    assert(n >= 0);
}

Fibo::Fibo(unsigned long n) : Fibo((unsigned long long) n) {}

Fibo::Fibo(unsigned n) : Fibo((unsigned long long) n) {}

Fibo::Fibo(const Fibo &f) {
    this->fibits = f.fibits;
}

Fibo::Fibo(Fibo&& f) : fibits(move(f.fibits)) {}

size_t Fibo::length() const {
    return fibits.size();
}

Fibo& Fibo::operator=(const Fibo &f) {
    this->fibits = f.fibits;
    return *this;
}

Fibo& Fibo::operator=(Fibo&& f) {
    this->fibits = move(f.fibits);
    return *this;
}

Fibo& Fibo::operator=(const unsigned long long n) {
    this->fibits = Fibo(n).fibits;
    return *this;
}

Fibo& Fibo::operator+=(const Fibo &f) {
    const int WINDOW_SIZE = 4;
    vector<char> sum;

    if (f == Zero()) {
        return *this;
    }
    
    if (this->fibits.size() <= f.fibits.size()) {
        sum = vector<char>(f.fibits.size());
        this->add_leading_zeros(f.fibits.size() - this->fibits.size());
        for (size_t i = 0; i < f.fibits.size(); i++)
            sum.at(i) = this->fibits.at(i) + f.fibits.at(i);
    } else {
        sum = vector<char>(this->fibits.size());
        size_t shorter_length = f.fibits.size();
        for (size_t i = 0; i < shorter_length; i++)
            sum.at(i) = this->fibits.at(i) + f.fibits.at(i);
        for (size_t i = shorter_length; i < this->fibits.size(); i++)
            sum.at(i) = this->fibits.at(i);
    }

    while(sum.size() < WINDOW_SIZE) sum.push_back(0);
    if(sum.back() != 0) sum.push_back(0);
    reverse(sum.begin(), sum.end());

    for(size_t i = 0; i <= sum.size() - WINDOW_SIZE; i++) {
        if(sum.at(i) == 0 && sum.at(i + 1) != 0) {
            char x = sum.at(i + 3);
            vector<char> clean_a = make_clean(0, 2, 0, x);
            vector<char> clean_b = make_clean(0, 3, 0, x);
            vector<char> clean_c = make_clean(0, 2, 1, x);
            vector<char> clean_d = make_clean(0, 1, 2, x);
            auto first = sum.begin() + i;
            auto last = sum.begin() + i + 4;
            vector<char> temp(first, last);
            if(temp == clean_a) modify_clean(i, &sum, 1, 0, 0, x + 1);
            else if(temp == clean_b) modify_clean(i, &sum, 1, 1, 0, x + 1);
            else if(temp == clean_c) modify_clean(i, &sum, 1, 1, 0, x);
            else if(temp == clean_d) modify_clean(i, &sum, 1, 0, 1, x);
        }
    }

    size_t i=sum.size() - WINDOW_SIZE;
    char x = sum.at(i);
    char y = sum.at(i+1);
    vector<char> clean_a = make_clean(x, 0, 3, 0);
    vector<char> clean_b = make_clean(x, 0, 2, 0);
    vector<char> clean_c = make_clean(0, 1, 2, 0);
    vector<char> clean_d = make_clean(x, y, 0, 3);
    vector<char> clean_e = make_clean(x, y, 0, 2);
    vector<char> clean_f = make_clean(x, 0, 1, 2);
    vector<char> clean_g = make_clean(0, 0, 2, 1);
    vector<char> clean_h = make_clean(1, 0, 2, 1);
    auto first = sum.begin() + i;
    auto last = sum.begin() + i + 4;
    vector<char> temp(first, last);
    if(temp == clean_a) modify_clean(i, &sum, x, 1, 1, 1);
    else if(temp == clean_b) modify_clean(i, &sum, x, 1, 0, 1);
    else if(temp == clean_c) modify_clean(i, &sum, 1, 0, 1, 0);
    else if(temp == clean_d) modify_clean(i, &sum, x, y, 1, 1);
    else if(temp == clean_e) modify_clean(i, &sum, x, y, 1, 0);
    else if(temp == clean_f) modify_clean(i, &sum, x, 1, 0, 1);
    else if(temp == clean_g) modify_clean(i, &sum, 1, 0, 0, 0);
    else if(temp == clean_h) modify_clean(i, &sum, 1, 1, 1, 0);

    for(size_t i = 0; i < sum.size(); i++)
        sum.at(i) = '0' + sum.at(i);
    string str(sum.begin(), sum.end());

    size_t j = 0;

    while (j != str.size() && str.at(j) == '0')
        j++;

    fibits = vector<bool> (str.size() - j);

    i = fibits.size();
    while (i > 0) {
        i--;
        fibits.at(i) = str.at(j) != '0';
        j++;
    }

    this->normalize();

    return *this;
}

Fibo Fibo::operator+=(const unsigned long long n) {
    return (*this += Fibo(n));
}

Fibo Fibo::operator+(const Fibo &f) const {
    Fibo copy (*this);
    return (copy += f);
}

Fibo Fibo::operator+(const unsigned long long n) {
    Fibo copy (*this);
    return (copy + Fibo(n));
}

Fibo operator+(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) + f);
}

Fibo& Fibo::operator&=(const Fibo &f) {
    if (this->fibits.size() < f.fibits.size()) {
        this->add_leading_zeros(f.fibits.size() - this->fibits.size());
    }

    size_t shorter_length = min(this->fibits.size(), f.fibits.size());

    for (size_t i = 0; i < shorter_length; i++) {
        this->fibits.at(i) = this->fibits.at(i) && f.fibits.at(i);
    }

    for (size_t i = shorter_length; i < this->fibits.size(); i++) {
        this->fibits.at(i) = false;
    }

    this->normalize();

    return *this;
}

Fibo Fibo::operator&=(const unsigned long long n) {
    return (*this &= Fibo(n));
}

Fibo Fibo::operator&(const Fibo &f) const {
    Fibo copy (*this);
    return (copy &= f);
}

Fibo Fibo::operator&(const unsigned long long n) {
    Fibo copy (*this);
    return (copy & Fibo(n));
}

Fibo operator&(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) & f);
}

Fibo &Fibo::operator|=(const Fibo &f) {
    if (this->fibits.size() < f.fibits.size()) {
        this->add_leading_zeros(f.fibits.size() - this->fibits.size());
    }

    size_t shorter_length = min(this->fibits.size(), f.fibits.size());

    for (size_t i = 0; i < shorter_length; i++) {
        this->fibits.at(i) = this->fibits.at(i) || f.fibits.at(i);
    }

    this->normalize();

    return *this;
}

Fibo Fibo::operator|(const Fibo &f) const {
    Fibo copy (*this);
    return (copy |= f);
}

Fibo Fibo::operator|=(const unsigned long long n) {
    return (*this |= Fibo(n));
}

Fibo Fibo::operator|(const unsigned long long n) {
    Fibo copy (*this);
    return (copy | Fibo(n));
}

Fibo operator|(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) | f);
}

Fibo& Fibo::operator^=(const Fibo &f) {
    if (this->fibits.size() < f.fibits.size()) {
        this->add_leading_zeros(f.fibits.size() - this->fibits.size());
    }

    size_t shorter_length = min(this->fibits.size(), f.fibits.size());

    for (size_t i = 0; i < shorter_length; i++) {
        if (this->fibits.at(i) && f.fibits.at(i)) {
            this->fibits.at(i) = false;
        } else if (f.fibits.at(i)) {
            this->fibits.at(i) = true;
        }
    }

    this->normalize();

    return *this;
}

Fibo Fibo::operator^(const Fibo &f) const {
    Fibo copy (*this);
    return (copy ^= f);
}

Fibo Fibo::operator^=(const unsigned long long n) {
    return (*this ^= Fibo(n));
}

Fibo Fibo::operator^(const unsigned long long n) {
    Fibo copy (*this);
    return (copy ^ Fibo(n));
}

Fibo operator^(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) ^ f);
}

Fibo &Fibo::operator<<=(const unsigned long long n) {
    std::reverse(this->fibits.begin(), this->fibits.end());

    for (int i = n; i > 0; i--) {
        this->fibits.push_back(false);
    }

    std::reverse(this->fibits.begin(), this->fibits.end());

    return *this;
}

Fibo Fibo::operator<<(const unsigned long long n) const {
    Fibo copy (*this);
    return (copy <<= n);
}

bool Fibo::operator==(const Fibo &f) const {
    return (this->fibits == f.fibits);
}

bool Fibo::operator==(const unsigned long long n) {
    return (*this == Fibo(n));
}

bool operator==(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) == f);
}

bool Fibo::operator!=(const Fibo &f) const {
    return (this->fibits != f.fibits);
}

bool Fibo::operator!=(const unsigned long long n) {
    return (*this != Fibo(n));
}

bool operator!=(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) != f);
}

bool Fibo::operator<(const Fibo &f) const {
    if (this->fibits.size() == f.fibits.size()) {
        size_t i = this->fibits.size() - 1;

        while ((i > 0) && (this->fibits.at(i) == f.fibits.at(i))) {
            i--;
        }

        return (this->fibits.at(i) < f.fibits.at(i));
    } else {
        return (this->fibits.size() < f.fibits.size());
    }
}

bool Fibo::operator<(const unsigned long long n) {
    return (*this < Fibo(n));
}

bool operator<(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) < f);
}

bool Fibo::operator>(const Fibo &f) const {
    return (!(*this < f) && (*this != f));
}

bool Fibo::operator>(const unsigned long long n) {
    return (*this > Fibo(n));
}

bool operator>(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) > f);
}

bool Fibo::operator<=(const Fibo &f) const {
    return !(*this > f);
}

bool Fibo::operator<=(const unsigned long long n) {
    return (*this <= Fibo(n));
}

bool operator<=(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) <= f);
}

bool Fibo::operator>=(const Fibo &f) const {
    return !(*this < f);
}

bool Fibo::operator>=(const unsigned long long n) {
    return (*this >= Fibo(n));
}

bool operator>=(const unsigned long long n, const Fibo &f) {
    return (Fibo(n) >= f);
}

ostream &operator<<(ostream &os, const Fibo& f) {
    size_t i = f.fibits.size();

    while (i > 0) {
        i--;
        if (!f.fibits.at(i)) {
            os << "0";
        } else {
            os << "1";
        }
    }

    return os;
}

void Fibo::normalize() {
    if (fibits.at(fibits.size() - 1)) {
        fibits.push_back(false);
    }

    size_t i = fibits.size() - 2;
    size_t j = i;

    while (i > 0) {
        // korygujemy jeĹli checmy dostawiÄ jedynkÄ obok innej jedynki
        while ((j < fibits.size() - 1) && fibits.at(j)
                && (fibits.at(j) == fibits.at(j - 1))) {
            fibits.at(j) = false;
            fibits.at(j - 1) = false;
            fibits.at(j + 1) = true;
            j += 2;
        }
        i--;
        j = i;
    }

    // usuwanie powstaĹych zer na poczÄtku
    while (!fibits.at(fibits.size() - 1) && (fibits.size() != 1)) {
        fibits.pop_back();
    }
}

void Fibo::add_leading_zeros(int n) {
    for (int i = 0; i < n; i++) {
        fibits.push_back(false);
    }
}

bool Fibo::is_string_correct(const string &str) {
    if (str.at(0) == '0') {
        return false;
    }

    for (size_t i = 0; i < str.size(); i++) {
        if (str.at(i) != '0' && str.at(i) != '1') {
            return false;
        }
    }

    return true;
}

vector<char> Fibo::make_clean(char a, int b, int c, int d) {
    vector<char> clean = vector<char>(4);
    clean.at(0) = a;
    clean.at(1) = b;
    clean.at(2) = c;
    clean.at(3) = d;
    return clean;
}

void Fibo::modify_clean(size_t i, vector<char> *sum, int a, int b, int c, int d) {
    sum->at(i) = a;
    sum->at(i + 1) = b;
    sum->at(i + 2) = c;
    sum->at(i + 3) = d;

}

const Fibo& Zero() {
    static const Fibo zero = Fibo();
    return zero;
}

const Fibo& One() {
    static const Fibo one = Fibo(1);
    return one;
}