/*
 * Functions.cpp
 *
 *  Created on: Dec 26, 2018
 *      Author: ccontaxis
 */

#include <Functions.h>

namespace LSST {
namespace M1M3 {
namespace SS {

template <class iType, class rType>
Function<iType, rType>::~Function() {

}

template <class iType, class rType>
rType Function<iType, rType>::evaluate(iType x) {
	return rType();
}

template <class iType>
LinearFunction<iType>::LinearFunction() {
	this->update(iType(), iType());
}

template <class iType>
void LinearFunction<iType>::update(iType m, iType b) {
	this->m = m;
	this->b = b;
}

template <class iType>
iType LinearFunction<iType>::evaluate(iType x) {
	return this->m * x + this->b;
}

template <class iType>
Poly2Function<iType>::Poly2Function() {
	this->update(iType(), iType(), iType());
}

template <class iType>
void Poly2Function<iType>::update(iType a, iType b, iType c) {
	this->a = a;
	this->b = b;
	this->c = c;
}

template <class iType>
iType Poly2Function<iType>::evaluate(iType x) {
	return x * ((x * this->a) + this->b) + this->c;
}

template <class iType>
Poly3Function<iType>::Poly3Function() {
	this->update(iType(), iType(), iType(), iType());
}

template <class iType>
void Poly3Function<iType>::update(iType a, iType b, iType c, iType d) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

template <class iType>
iType Poly3Function<iType>::evaluate(iType x) {
	return x * (x * ((x * this->a) + this->b) + this->c) + this->d;
}

template <class iType>
Poly4Function<iType>::Poly4Function() {
	this->update(iType(), iType(), iType(), iType(), iType());
}

template <class iType>
void Poly4Function<iType>::update(iType a, iType b, iType c, iType d, iType e) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->e = e;
}

template <class iType>
iType Poly4Function<iType>::evaluate(iType x) {
	return x * (x * (x * ((x * this->a) + this->b) + this->c) + this->d) + this->e;
}

template <class iType>
Poly5Function<iType>::Poly5Function() {
	this->update(iType(), iType(), iType(), iType(), iType(), iType());
}

template <class iType>
void Poly5Function<iType>::update(iType a, iType b, iType c, iType d, iType e, iType f) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->e = e;
	this->f = f;
}

template <class iType>
iType Poly5Function<iType>::evaluate(iType x) {
	return x * (x * (x * (x * ((x * this->a) + this->b) + this->c) + this->d) + this->e) + this->f;
}

BitSetFunction::BitSetFunction() {
	this->update(0);
}

void BitSetFunction::update(int bit) {
	this->bitMask = 1ul << bit;
}

bool BitSetFunction::evaluate(unsigned long x) {
	return (x & this->bitMask) != 0;
}

NotBitSetFunction::NotBitSetFunction() {
	this->update(0);
}

void NotBitSetFunction::update(int bit) {
	this->bitMask = 1ul << bit;
}

bool NotBitSetFunction::evaluate(unsigned long x) {
	return (x & this->bitMask) == 0;
}

Matrix1x8Function::Matrix1x8Function() {
	this->update(0, 0, 0, 0, 0, 0, 0, 0);
}

void Matrix1x8Function::update(float a, float b, float c, float d, float e, float f, float g, float h) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->e = e;
	this->f = f;
	this->g = g;
	this->h = h;
}

float Matrix1x8Function::evaluate(Matrix1x8FunctionInput x) {
	return x.A * this->a + x.B * this->b + x.C * this->c + x.D * this->d + x.E * this->e + x.F * this->f + x.G * this->g + x.H * this->h;
}

AngularAccelerationFunction::AngularAccelerationFunction() {
	this->update(0);
}

void AngularAccelerationFunction::update(float distance) {
	this->distance = distance;
}

float AngularAccelerationXFunction::evaluate(AngularAccelerationFunctionInput x) {
	return (x.a4y - x.a3y) / this->distance;
}

float AngularAccelerationYFunction::evaluate(AngularAccelerationFunctionInput x) {
	return (x.a2x - x.a1x) / this->distance;
}

float AngularAccelerationZFunction::evaluate(AngularAccelerationFunctionInput x) {
	return (x.a1x + x.a2x - x.a3x - x.a4x) / (this->distance * 2.0f);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
