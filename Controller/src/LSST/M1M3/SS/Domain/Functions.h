/*
 * Functions.h
 *
 *  Created on: Dec 26, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_DOMAIN_FUNCTIONS_H_
#define LSST_M1M3_SS_DOMAIN_FUNCTIONS_H_

namespace LSST {
namespace M1M3 {
namespace SS {

template <class iType, class rType>
class Function {
public:
	virtual ~Function();

	virtual rType evaluate(iType x);
};

template <class iType>
class LinearFunction : public Function<iType, iType> {
protected:
	iType m;
	iType b;

public:
	LinearFunction();

	void update(iType m, iType b);

	iType evaluate(iType x);
};

template <class iType>
class Poly2Function : public Function<iType, iType> {
protected:
	iType a;
	iType b;
	iType c;

public:
	Poly2Function();

	void update(iType a, iType b, iType c);

	iType evaluate(iType x);
};

template <class iType>
class Poly3Function : public Function<iType, iType> {
protected:
	iType a;
	iType b;
	iType c;
	iType d;

public:
	Poly3Function();

	void update(iType a, iType b, iType c, iType d);

	iType evaluate(iType x);
};

template <class iType>
class Poly4Function : public Function<iType, iType> {
protected:
	iType a;
	iType b;
	iType c;
	iType d;
	iType e;

public:
	Poly4Function();

	void update(iType a, iType b, iType c, iType d, iType e);

	iType evaluate(iType x);
};

template <class iType>
class Poly5Function : public Function<iType, iType> {
protected:
	iType a;
	iType b;
	iType c;
	iType d;
	iType e;
	iType f;

public:
	Poly5Function();

	void update(iType a, iType b, iType c, iType d, iType e, iType f);

	iType evaluate(iType x);
};

class BitSetFunction : public Function<unsigned long, bool> {
protected:
	unsigned long bitMask;

public:
	BitSetFunction();

	void update(int bit);

	bool evaluate(unsigned long x);
};

class NotBitSetFunction : public Function<unsigned long, bool> {
protected:
	unsigned long bitMask;

public:
	NotBitSetFunction();

	void update(int bit);

	bool evaluate(unsigned long x);
};

struct Matrix1x8FunctionInput {
	float A;
	float B;
	float C;
	float D;
	float E;
	float F;
	float G;
	float H;
};

class Matrix1x8Function : public Function<Matrix1x8FunctionInput, float> {
protected:
	float a;
	float b;
	float c;
	float d;
	float e;
	float f;
	float g;
	float h;

public:
	Matrix1x8Function();

	void update(float a, float b, float c, float d, float e, float f, float g, float h);

	float evaluate(Matrix1x8FunctionInput x);
};

struct AngularAccelerationFunctionInput {
	float a1x;
	float a1y;
	float a2x;
	float a2y;
	float a3x;
	float a3y;
	float a4x;
	float a4y;
};

class AngularAccelerationFunction : public Function<AngularAccelerationFunctionInput, float> {
protected:
	float distance;

public:
	AngularAccelerationFunction();

	void update(float distance);
};

class AngularAccelerationXFunction : public AngularAccelerationFunction {
public:
	float evaluate(AngularAccelerationFunctionInput x);
};

class AngularAccelerationYFunction : public AngularAccelerationFunction {
public:
	float evaluate(AngularAccelerationFunctionInput x);
};

class AngularAccelerationZFunction : public AngularAccelerationFunction {
public:
	float evaluate(AngularAccelerationFunctionInput x);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_DOMAIN_FUNCTIONS_H_ */
