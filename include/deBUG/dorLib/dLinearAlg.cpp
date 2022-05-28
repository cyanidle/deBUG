#include "dLinearAlg.h"

dVect::dVect(VECTOR_TYPE x, VECTOR_TYPE y) : base_vect{x,y,1} {
}
dVect::dVect(VECTOR_TYPE x, VECTOR_TYPE y, VECTOR_TYPE scale) : base_vect{ x,y,scale } {
}
dVect::dVect() : base_vect{0, 0, 1} {
}

VECTOR_TYPE& dVect::x()
{
	return this->_get_arr()[0][0];
}

const VECTOR_TYPE dVect::get_x() const
{
	return this->_get_arr()[0][0];
}

VECTOR_TYPE& dVect::y()
{
	return this->_get_arr()[0][1];
}

const VECTOR_TYPE dVect::get_y() const
{
	return this->_get_arr()[0][1];
}

void dVect::set_scale(VECTOR_TYPE scale)
{
	this->_get_arr()[0][2] = scale;
}

VECTOR_TYPE dVect::get_scale() const
{
	return this->_get_arr()[0][2];
}

VECTOR_TYPE& dVect::operator[](int ind)
{
	if ((ind > 1) or (ind < 0))
		throw std::exception("Vector index out of range");
	return this->_get_arr()[0][ind];
}

dVect dVect::operator+(const dVect& src) const
{
	dVect result;
	result.x() = this->_get_arr()[0][0] + src._get_arr()[0][0];
	result.y() = this->_get_arr()[0][1] + src._get_arr()[0][1];
	result.set_scale(this->_get_arr()[0][2] > src._get_arr()[0][2] ? this->_get_arr()[0][2] : src._get_arr()[0][2]);
	return result;
}

dVect& dVect::operator+=(const dVect& src)
{
	this->_get_arr()[0][0] += src._get_arr()[0][0];
	this->_get_arr()[0][1] += src._get_arr()[0][1];
	this->set_scale(this->_get_arr()[0][2] > src._get_arr()[0][2] ? this->_get_arr()[0][2] : src._get_arr()[0][2]);
	return *this;
}

dVect dVect::operator-(const dVect& src) const
{
	dVect result;
	result.x() = this->_get_arr()[0][0] - src._get_arr()[0][0];
	result.y() = this->_get_arr()[0][1] - src._get_arr()[0][1];
	result.set_scale(this->_get_arr()[0][2] > src._get_arr()[0][2] ? this->_get_arr()[0][2] : src._get_arr()[0][2]);
	return result;
}

dVect& dVect::operator-()
{
	this->_get_arr()[0][0] = -this->_get_arr()[0][0];
	this->_get_arr()[0][1] = -this->_get_arr()[0][1];
	return *this;
}

dVect& dVect::operator-=(const dVect& src)
{
	this->_get_arr()[0][0] -= src._get_arr()[0][0];
	this->_get_arr()[0][1] -= src._get_arr()[0][1];
	this->set_scale(this->_get_arr()[0][2] > src._get_arr()[0][2] ? this->_get_arr()[0][2] : src._get_arr()[0][2]);
	return *this;
}

dVect dVect::operator+(const VECTOR_TYPE& src) const
{
	dVect result;
	result.x() = this->_get_arr()[0][0] + src;
	result.y() = this->_get_arr()[0][1] + src;
	result.set_scale(this->_get_arr()[0][2]);
	return result;
}

dVect& dVect::operator+=(const VECTOR_TYPE& src)
{
	this->_get_arr()[0][0] += src;
	this->_get_arr()[0][1] += src;
	return *this;
}

dVect dVect::operator-(const VECTOR_TYPE& src) const
{
	dVect result;
	result.x() = this->_get_arr()[0][0] - src;
	result.y() = this->_get_arr()[0][1] - src;
	result.set_scale(this->_get_arr()[0][2]);
	return result;
}

dVect& dVect::operator-=(const VECTOR_TYPE& src)
{
	this->_get_arr()[0][0] -= src;
	this->_get_arr()[0][1] -= src;
	return *this;
}

dVect operator*(const dVect& base, const VECTOR_TYPE& src)
{
	dVect result;
	result.x() = base._get_arr()[0][0] * src;
	result.y() = base._get_arr()[0][1] * src;
	result.set_scale(base._get_arr()[0][2]);
	return result;
}

dVect& operator*=(dVect& base, const VECTOR_TYPE& src)
{
	base._get_arr()[0][0] *= src;
	base._get_arr()[0][1] *= src;
	return base;
}

dVect operator/(const dVect& base, const VECTOR_TYPE& src)
{
	dVect result;
	result.x() = base._get_arr()[0][0] / src;
	result.y() = base._get_arr()[0][1] / src;
	result.set_scale(base._get_arr()[0][2]);
	return result;
}

dVect& operator/=(dVect& base, const VECTOR_TYPE& src)
{
	base._get_arr()[0][0] /= src;
	base._get_arr()[0][1] /= src;
	return base;
}

int operator+(const dVect& base, const dTransform2d& src)
{
	throw std::exception("Not allowed!");
}

int operator-(const dVect& base, const dTransform2d& src)
{
	throw std::exception("Not allowed!");
}

dVect operator/(const dVect& base, const dVect& src)
{
	throw std::exception("Not allowed!");
}

dTransform2d::dTransform2d(VECTOR_TYPE rotation) : 
	base_transform{ cosf(rotation),sinf(rotation),0,
					-sinf(rotation),cosf(rotation),0,
					0, 0, 1}
{
}

dTransform2d::dTransform2d(VECTOR_TYPE rotation, const dVect& scaler, const dVect& mover) :
	base_transform{ cosf(rotation) * scaler.get_x(), sinf(rotation), 0,
					-sinf(rotation), cosf(rotation) * scaler.get_y(), 0,
					mover.get_x()* mover.get_scale(), mover.get_y()* mover.get_scale() ,scaler.get_scale()}
{
}

dTransform2d::dTransform2d() : base_transform{1,0,0, 0,1,0, 0,0,1}
{
}

dTransform2d::dTransform2d(base_transform bs) : base_transform(bs)
{
}

