#include "dorLib/dLinearAlg.h"
#include "ros/ros.h"
//#include "dCorelib.h"
//#include "dVariant.h"
#include <iostream>

int main(int argc, char* argv[]) {
	dTransform2d var;
	dVect vect(2,2);
	auto newv = vect + 5;
	newv = vect - 5;
	newv += newv;
	printf("kek\n");
}