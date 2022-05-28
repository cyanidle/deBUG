#pragma once
#include "dMatrix.h"

#define VECTOR_TYPE float
#define D_PI 3.1415926535f
typedef dMatrix<VECTOR_TYPE, 3, 1> base_vect;
typedef dMatrix<VECTOR_TYPE, 3, 3> base_transform;
class dVect : public base_vect {
public:
	dVect(VECTOR_TYPE x, VECTOR_TYPE y);
	dVect(VECTOR_TYPE x, VECTOR_TYPE y, VECTOR_TYPE scale);
	dVect();
	VECTOR_TYPE& x();
	const VECTOR_TYPE get_x() const;
	VECTOR_TYPE& y();
	const VECTOR_TYPE get_y() const;
	void set_scale(VECTOR_TYPE scale);
	VECTOR_TYPE get_scale() const;
	VECTOR_TYPE& operator[](int ind);
	dVect operator+(const dVect& src) const;
	dVect& operator+=(const dVect& src);
	dVect operator-(const dVect& src) const;
	dVect& operator-();
	dVect& operator-=(const dVect& src);

	dVect operator+(const VECTOR_TYPE& src) const;
	dVect& operator+=(const VECTOR_TYPE& src);
	dVect operator-(const VECTOR_TYPE& src) const;
	dVect& operator-=(const VECTOR_TYPE& src);
};

dVect operator*(const dVect& base, const VECTOR_TYPE& src);
dVect& operator*=(dVect& base, const VECTOR_TYPE& src);
dVect operator/(const dVect& base, const VECTOR_TYPE& src);
dVect& operator/=(dVect& base, const VECTOR_TYPE& src);
class dTransform2d : public base_transform {
public:
	dTransform2d(VECTOR_TYPE rotation);
	dTransform2d(VECTOR_TYPE rotation,
		const dVect& scaler,
		const dVect& mover = dVect(0,0));
	dTransform2d();
	dTransform2d(base_transform bs);
};

int operator+(const dVect& base, const dTransform2d& src);
int operator-(const dVect& base, const dTransform2d& src);
dVect operator/(const dVect& base, const dVect& src);