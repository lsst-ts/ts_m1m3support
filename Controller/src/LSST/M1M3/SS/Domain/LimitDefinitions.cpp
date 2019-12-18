/*
 * LimitDefinitions.cpp
 *
 *  Created on: Dec 26, 2018
 *      Author: ccontaxis
 */

#include <LimitDefinitions.h>

namespace LSST {
namespace M1M3 {
namespace SS {

template <class a>
LimitDefinition<a>::LimitDefinition() {
    this->updateBase(false, false);
}

template <class a>
LimitDefinition<a>::~LimitDefinition() {
}

template <class a>
void LimitDefinition<a>::updateBase(bool hasWarningLevel, bool hasFaultLevel) {
    this->hasWarningLevel = hasWarningLevel;
    this->hasFaultLevel = hasFaultLevel;
}

template <class a>
void LimitDefinition<a>::reset() {
}

template <class a>
Limit::Type LimitDefinition<a>::check(a value) {
    return Limit::NONE;
}

TimedLimitDefinition::TimedLimitDefinition()
        : LimitDefinition<Limit::Type>() {
    this->update(false, false, 0, 0, 0);
}

void TimedLimitDefinition::update(bool hasWarningLevel, bool hasFaultLevel, int period, int warningPeriod, int faultPeriod) {
    this->updateBase(hasWarningLevel, hasFaultLevel);
    this->period = period;
    this->warningPeriod = warningPeriod;
    this->faultPeriod = faultPeriod;
    this->history = std::queue<Limit::Type>();
    for (int i = 0; i < this->period; ++i) {
        this->history.push(Limit::NONE);
    }
    this->warningCount = 0;
    this->faultCount = 0;
}

void TimedLimitDefinition::reset() {
    for (int i = 0; i < this->period; ++i) {
        this->history.pop();
        this->history.push(Limit::NONE);
    }
    this->warningCount = 0;
    this->faultCount = 0;
}

Limit::Type TimedLimitDefinition::check(Limit::Type value) {
    Limit::Type oldLimit = this->history.front();
    this->history.pop();
    if (oldLimit == Limit::FAULT) {
        --this->faultCount;
    } else if (oldLimit == Limit::WARNING) {
        --this->warningCount;
    }
    Limit::Type limit = value;
    this->history.push(limit);
    if (limit == Limit::FAULT) {
        ++this->faultCount;
    } else if (limit == Limit::WARNING) {
        ++this->warningCount;
    }
    if (this->hasFaultLevel && this->faultCount >= this->faultPeriod) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && this->warningCount >= this->warningPeriod) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

template <class a>
EqualLimitDefinition<a>::EqualLimitDefinition()
        : LimitDefinition<a>() {
    this->update(false, false, a(), a());
}

template <class a>
void EqualLimitDefinition<a>::update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel) {
    this->warningLevel = warningLevel;
    this->faultLevel = faultLevel;
}

template <class a>
Limit::Type EqualLimitDefinition<a>::check(a value) {
    if (this->hasFaultLevel && value == this->faultLevel) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && value == this->warningLevel) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

template <class a>
NotEqualLimitDefinition<a>::NotEqualLimitDefinition()
        : LimitDefinition<a>() {
    this->update(false, false, a(), a());
}

template <class a>
void NotEqualLimitDefinition<a>::update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel) {
    this->warningLevel = warningLevel;
    this->faultLevel = faultLevel;
}

template <class a>
Limit::Type NotEqualLimitDefinition<a>::check(a value) {
    if (this->hasFaultLevel && value != this->faultLevel) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && value != this->warningLevel) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

template <class a>
LessThanLimitDefinition<a>::LessThanLimitDefinition()
        : LimitDefinition<a>() {
    this->update(false, false, a(), a());
}

template <class a>
void LessThanLimitDefinition<a>::update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel) {
    this->warningLevel = warningLevel;
    this->faultLevel = faultLevel;
}

template <class a>
Limit::Type LessThanLimitDefinition<a>::check(a value) {
    if (this->hasFaultLevel && value < this->faultLevel) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && value < this->warningLevel) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

template <class a>
LessThanEqualLimitDefinition<a>::LessThanEqualLimitDefinition()
        : LimitDefinition<a>() {
    this->update(false, false, a(), a());
}

template <class a>
void LessThanEqualLimitDefinition<a>::update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel) {
    this->warningLevel = warningLevel;
    this->faultLevel = faultLevel;
}

template <class a>
Limit::Type LessThanEqualLimitDefinition<a>::check(a value) {
    if (this->hasFaultLevel && value <= this->faultLevel) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && value <= this->warningLevel) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

template <class a>
GreaterThanLimitDefinition<a>::GreaterThanLimitDefinition()
        : LimitDefinition<a>() {
    this->update(false, false, a(), a());
}

template <class a>
void GreaterThanLimitDefinition<a>::update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel) {
    this->warningLevel = warningLevel;
    this->faultLevel = faultLevel;
}

template <class a>
Limit::Type GreaterThanLimitDefinition<a>::check(a value) {
    if (this->hasFaultLevel && value > this->faultLevel) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && value > this->warningLevel) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

template <class a>
GreaterThanEqualLimitDefinition<a>::GreaterThanEqualLimitDefinition()
        : LimitDefinition<a>() {
    this->update(false, false, a(), a());
}

template <class a>
void GreaterThanEqualLimitDefinition<a>::update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel) {
    this->warningLevel = warningLevel;
    this->faultLevel = faultLevel;
}

template <class a>
Limit::Type GreaterThanEqualLimitDefinition<a>::check(a value) {
    if (this->hasFaultLevel && value >= this->faultLevel) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && value >= this->warningLevel) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

template <class a>
InRangeLimitDefinition<a>::InRangeLimitDefinition()
        : LimitDefinition<a>() {
    this->update(false, false, a(), a(), a(), a());
}

template <class a>
void InRangeLimitDefinition<a>::update(bool hasWarningLevel, bool hasFaultLevel, a lowWarningLevel, a highWarningLevel, a lowFaultLevel, a highFaultLevel) {
    this->lowWarningLevel = lowWarningLevel;
    this->highWarningLevel = highWarningLevel;
    this->lowFaultLevel = lowFaultLevel;
    this->highFaultLevel = highFaultLevel;
}

template <class a>
Limit::Type InRangeLimitDefinition<a>::check(a value) {
    if (this->hasFaultLevel && (value >= this->lowFaultLevel && value <= this->highFaultLevel)) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && (value >= this->lowWarningLevel && value <= this->highWarningLevel)) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

template <class a>
NotInRangeLimitDefinition<a>::NotInRangeLimitDefinition()
        : LimitDefinition<a>() {
    this->update(false, false, a(), a(), a(), a());
}

template <class a>
void NotInRangeLimitDefinition<a>::update(bool hasWarningLevel, bool hasFaultLevel, a lowWarningLevel, a highWarningLevel, a lowFaultLevel, a highFaultLevel) {
    this->lowWarningLevel = lowWarningLevel;
    this->highWarningLevel = highWarningLevel;
    this->lowFaultLevel = lowFaultLevel;
    this->highFaultLevel = highFaultLevel;
}

template <class a>
Limit::Type NotInRangeLimitDefinition<a>::check(a value) {
    if (this->hasFaultLevel && (value < this->lowFaultLevel || value > this->highFaultLevel)) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && (value < this->lowWarningLevel || value > this->highWarningLevel)) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

BitSetLimitDefinition::BitSetLimitDefinition()
        : LimitDefinition<unsigned long>() {
    this->update(false, false, 0, 0);
}

void BitSetLimitDefinition::update(bool hasWarningLevel, bool hasFaultLevel, int warningBit, int faultBit) {
    this->warningMask = (1ul << warningBit);
    this->faultMask = (1ul << faultBit);
}

Limit::Type BitSetLimitDefinition::check(unsigned long value) {
    if (this->hasFaultLevel && (value && this->faultMask) != 0) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && (value && this->warningMask) != 0) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

NotBitSetLimitDefinition::NotBitSetLimitDefinition()
        : LimitDefinition<unsigned long>() {
    this->update(false, false, 0, 0);
}

void NotBitSetLimitDefinition::update(bool hasWarningLevel, bool hasFaultLevel, int warningBit, int faultBit) {
    this->warningMask = (1ul << warningBit);
    this->faultMask = (1ul << faultBit);
}

Limit::Type NotBitSetLimitDefinition::check(unsigned long value) {
    if (this->hasFaultLevel && (value && this->faultMask) == 0) {
        return Limit::FAULT;
    }
    if (this->hasWarningLevel && (value && this->warningMask) == 0) {
        return Limit::WARNING;
    }
    return Limit::OK;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
