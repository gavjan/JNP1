#ifndef __FIBO_H__
#define __FIBO_H__

#include <vector>

using namespace std;

class Fibo {

public:

    Fibo();

    Fibo(const string &str);

    Fibo(const char* str);

    Fibo(unsigned long long n);

    Fibo(int n);

    Fibo(long n);

    Fibo(long long n);

    Fibo(unsigned long n);

    Fibo(unsigned n);

    Fibo(Fibo const &f);

    Fibo(Fibo&& f);

    Fibo(bool) = delete;

    Fibo(char) = delete;

    Fibo(float) = delete;

    Fibo(double) = delete;

    size_t length() const;

    Fibo& operator=(const Fibo& f);

    Fibo& operator=(Fibo&& f);

    Fibo& operator=(unsigned long long n);

    Fibo& operator+=(const Fibo& f);

    Fibo operator+=(unsigned long long n);

    Fibo operator+(const Fibo& f) const;

    Fibo operator+(unsigned long long n);

    friend Fibo operator+(unsigned long long n, const Fibo &f);

    Fibo& operator&=(const Fibo& f);

    Fibo operator&=(unsigned long long n);

    Fibo operator&(const Fibo& f) const;

    Fibo operator&(unsigned long long n);

    friend Fibo operator&(unsigned long long n, const Fibo &f);

    Fibo& operator|=(const Fibo& f);

    Fibo operator|=(unsigned long long n);

    Fibo operator|(const Fibo& f) const;

    Fibo operator|(unsigned long long n);

    friend Fibo operator|(unsigned long long n, const Fibo &f);

    Fibo& operator^=(const Fibo& f);

    Fibo operator^=(unsigned long long n);

    Fibo operator^(const Fibo &f) const;

    Fibo operator^(unsigned long long n);

    friend Fibo operator^(unsigned long long n, const Fibo &f);

    Fibo& operator<<=(unsigned long long n);

    Fibo operator<<(unsigned long long n) const;

    bool operator==(const Fibo& f) const;

    bool operator==(unsigned long long n);

    friend bool operator==(unsigned long long n, const Fibo& f);

    bool operator!=(const Fibo& f) const;

    bool operator!=(unsigned long long n);

    friend bool operator!=(unsigned long long n, const Fibo& f);

    bool operator<(const Fibo& f) const;

    bool operator<(unsigned long long n);

    friend bool operator<(unsigned long long n, const Fibo& f);

    bool operator>(const Fibo& f) const;

    bool operator>(unsigned long long n);

    friend bool operator>(unsigned long long n, const Fibo& f);

    bool operator<=(const Fibo& f) const;

    bool operator<=(unsigned long long n);

    friend bool operator<=(unsigned long long n, const Fibo& f);

    bool operator>=(const Fibo& f) const;

    bool operator>=(unsigned long long n);

    friend bool operator>=(unsigned long long n, const Fibo& f);

    friend ostream& operator<<(ostream& os, const Fibo& f);

private:

    vector<bool> fibits;

    // zmienia zapis na postać unormowaną
    void normalize();

    // dodaje n zer na początku
    void add_leading_zeros(int n);

    // sprawdza czy str zaczyna się od 1 i czy zawiera tylko 0 i 1
    bool is_string_correct(const string &str);

    vector<char> make_clean(char a, int b, int c, int d);

    void modify_clean(size_t i, vector<char> *sum, int a, int b, int c, int d);

};

const Fibo& Zero();

const Fibo& One();

#endif /* __FIBO_H__ */
