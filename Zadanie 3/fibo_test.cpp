#include <cassert>
#include <string>
#include "fibo.h"
int main() {
	assert((Fibo("1001")+Fibo("10"))==Fibo("1011"));

	assert(Fibo("11")==Fibo("100"));
	assert(Fibo("111")==Fibo("1001"));
	assert(Fibo("1111")==Fibo("10100"));

	Fibo fib=Fibo("1001");
	assert(Fibo(fib)==Fibo("1001"));

	assert(Fibo(2)+Fibo(2)==Fibo(4));
	assert(Fibo("1")+Fibo()==Fibo("1"));
	assert(Fibo("10100")+Fibo("10100")==Fibo("1000001"));
	assert(Fibo("1010")+Fibo("1010")==Fibo("100001"));
	assert(Fibo("1")+Fibo("10")==Fibo("100"));
	assert(Fibo(10)+2==Fibo(12));
	assert(2+Fibo(10)==Fibo(12));


	std::string million(1000001, '0');
	million[0]='1';
	Fibo fi("1");
	fi<<=1000000;
	assert(fi==Fibo(million));
	//assert(Fibo("1") << 1000000 == Fibo(million));

	assert((Fibo("101")&Fibo("111"))==Fibo("1"));
	assert((Fibo("101")|Fibo("111"))==Fibo("1101"));
	assert(Fibo(10)<=Fibo(10));
	assert(!(Fibo(1)>Fibo(2)));
	assert(Fibo("1101")>=(Fibo("101")|Fibo("111")));
	assert(!(Fibo("111")!=Fibo("1001")));

	assert(Fibo("111").length()==Fibo("1010").length());
}