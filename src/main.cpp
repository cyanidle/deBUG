#include "dLinearAlg.h"
//#include "dCorelib.h"
#include "dVariant.h"
#include <iostream>

int main(int argc, char* argv[]) {
	dVariant var(3);
	dVariant vect(dVect(0,0));
	auto newv = vect + 5;
	newv = vect - 5;
	newv += newv;
	printf("kek");
}