#pragma once
#include "dLinearAlg.h"


#define _INTERNAL_DMAKE_SIMPLE_OPERATORS(dtype,op)  \
dVariant operator op (dtype base) { \
 switch (this->type) { \
case dVariant::dVarType::INT: \
	return dVariant(base op this->container.m_INT); \
case dVariant::dVarType::FLOAT: \
	return dVariant(base op this->container.m_FLOAT); \
case dVariant::dVarType::DOUBLE: \
	return dVariant(base op this->container.m_DOUBLE); \
case dVariant::dVarType::dTRANSFORM: \
	return dVariant(this->container.m_dTRANSFORM op base); \
case dVariant::dVarType::dVECT: \
	return dVariant(this->container.m_dVECT op base); \
default: \
	return dVariant(); \
}  \
};

#define _INTERNAL_DMAKE_SIMPLE_OPERATORS_NOTRANSFORM(dtype,op)  \
dVariant operator op (dtype base) { \
 switch (this->type) { \
case dVariant::dVarType::INT: \
	return dVariant(base op this->container.m_INT); \
case dVariant::dVarType::FLOAT: \
	return dVariant(base op this->container.m_FLOAT); \
case dVariant::dVarType::DOUBLE: \
	return dVariant(base op this->container.m_DOUBLE); \
case dVariant::dVarType::dVECT: \
	return dVariant(this->container.m_dVECT op base); \
default: \
	return dVariant(); \
}  \
};

#define _INTERNAL_DMAKE_FULL_BODY(op) \
dVariant operator op (dVariant& src) { \
switch (type) { \
case dVariant::dVarType::INT: \
	return dVariant(src op container.m_INT); \
case dVariant::dVarType::FLOAT: \
	return dVariant(src op container.m_FLOAT); \
case dVariant::dVarType::DOUBLE: \
	return dVariant(src op container.m_DOUBLE); \
case dVariant::dVarType::dTRANSFORM: \
	return dVariant(src op container.m_dTRANSFORM); \
case dVariant::dVarType::dVECT: \
	return dVariant(src op container.m_dVECT); \
default: \
	return dVariant(); \
}  \
};

#define _INTERNAL_DMAKE_FULL_BODY_NOTRANSFORM(op) \
dVariant operator op (dVariant& src) { \
switch (type) { \
case dVariant::dVarType::INT: \
	return dVariant(src op container.m_INT); \
case dVariant::dVarType::FLOAT: \
	return dVariant(src op container.m_FLOAT); \
case dVariant::dVarType::DOUBLE: \
	return dVariant(src op container.m_DOUBLE); \
case dVariant::dVarType::dTRANSFORM: \
	throw std::runtime_error("Not allowed!"); \
case dVariant::dVarType::dVECT: \
	return dVariant(src op container.m_dVECT); \
default: \
	return dVariant(); \
}  \
};

#define _INTERNAL_DMAKE_VARIANTS_OPERATOR(op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS(int, op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS(float, op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS(double, op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS(dTransform2d, op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS(dVect, op); \
_INTERNAL_DMAKE_FULL_BODY(op)

#define _INTERNAL_DMAKE_VARIANTS_OPERATOR_NOTRANSFORM(op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS_NOTRANSFORM(int, op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS_NOTRANSFORM(float, op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS_NOTRANSFORM(double, op) \
_INTERNAL_DMAKE_SIMPLE_OPERATORS_NOTRANSFORM(dVect, op); \
_INTERNAL_DMAKE_FULL_BODY_NOTRANSFORM(op)

struct dVariant
{
private:
	union _dVariant {
		int m_INT;
		float m_FLOAT;
		double m_DOUBLE;
		bool m_BOOL;
		dTransform2d m_dTRANSFORM;
		dVect m_dVECT;
		explicit _dVariant() : m_INT(0) { ; }
		explicit _dVariant(int i) : m_INT(i) { ; }
		explicit _dVariant(float f) : m_FLOAT(f) { ; }
		explicit _dVariant(double d) : m_DOUBLE(d) { ; }
		explicit _dVariant(dTransform2d b) : m_dTRANSFORM(b) { ; }
		explicit _dVariant(base_transform b) : m_dTRANSFORM(b) { ; }
		explicit _dVariant(dVect b) : m_dVECT(b) { ; }
		~_dVariant() {};
	};
public:
	enum class dVarType {
		INT,
		FLOAT,
		DOUBLE,
		dTRANSFORM,
		dVECT
	};
	_dVariant container;
	dVarType type = dVarType::INT;
	explicit dVariant() : type(dVariant::dVarType::INT), container((int)0) {
	}
	explicit dVariant(int i) : type(dVariant::dVarType::INT), container(i) {
	}
	explicit dVariant(float f) :type(dVariant::dVarType::FLOAT), container(f) {
	}
	explicit dVariant(double d) : type(dVariant::dVarType::DOUBLE), container(d) {
	}
	explicit dVariant(dTransform2d tr) : type(dVariant::dVarType::dTRANSFORM), container(tr) {
	}
	explicit dVariant(base_transform tr) : type(dVariant::dVarType::dTRANSFORM), container(tr) {
	}
	explicit dVariant(dVect vect) : type(dVariant::dVarType::dVECT), container(vect) {
	}
	dVariant(const dVariant& src) {
		if (this != &src) {
			switch (src.type) {
			case dVarType::INT:
				this->container.m_INT = src.container.m_INT;
				break;
			case dVarType::FLOAT:
				this->container.m_FLOAT = src.container.m_FLOAT;
				break;
			case dVarType::DOUBLE:
				this->container.m_DOUBLE = src.container.m_DOUBLE;
				break;
			case dVarType::dTRANSFORM:
				this->container.m_dTRANSFORM = src.container.m_dTRANSFORM;
				break;
			case dVarType::dVECT:
				this->container.m_dVECT = src.container.m_dVECT;
				break;
			}
		}
	}
	dVariant& operator=(const dVariant&& src) noexcept{
		if (this != &src) {
			switch (src.type) {
			case dVarType::INT:
				this->container.m_INT = src.container.m_INT;
				break;
			case dVarType::FLOAT:
				this->container.m_FLOAT = src.container.m_FLOAT;
				break;
			case dVarType::DOUBLE:
				this->container.m_DOUBLE = src.container.m_DOUBLE;
				break;
			case dVarType::dTRANSFORM:
				this->container.m_dTRANSFORM = src.container.m_dTRANSFORM;
				break;
			case dVarType::dVECT:
				this->container.m_dVECT = src.container.m_dVECT;
				break;
			}
		}
		return *this;
	}

	_INTERNAL_DMAKE_VARIANTS_OPERATOR(+);
	_INTERNAL_DMAKE_VARIANTS_OPERATOR(-);
	_INTERNAL_DMAKE_VARIANTS_OPERATOR(*);
	_INTERNAL_DMAKE_VARIANTS_OPERATOR_NOTRANSFORM(/ );

	dVariant& operator+=(dVariant& src) {
		*this = (*this + src);
		return *this;
	}
	dVariant& operator-=(dVariant& src) {
		*this = (*this - src);
		return *this;
	}
	dVariant& operator*=(dVariant& src) {
		*this = (*this * src);
		return *this;
	}
	dVariant& operator/=(dVariant& src) {
		*this = (*this / src);
		return *this;
	}


	int& Int() { return container.m_INT; }
	float& Float() { return container.m_FLOAT; }
	double& Double() { return container.m_DOUBLE; }
	bool& Bool() { return container.m_BOOL; }
	dTransform2d& Transform() { return container.m_dTRANSFORM; }
	dVect& Vect() { return container.m_dVECT; }
};


