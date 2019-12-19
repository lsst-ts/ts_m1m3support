/*
 * LimitDefinitions.h
 *
 *  Created on: Dec 26, 2018
 *      Author: ccontaxis
 */

#include <queue>

#ifndef LSST_M1M3_SS_DOMAIN_LIMITDEFINITIONS_H_
#define LSST_M1M3_SS_DOMAIN_LIMITDEFINITIONS_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct Limit {
    enum Type {
        NONE = 0,
        OK = 1,
        WARNING = 2,
        FAULT = 3,
    };
};

template <class a>
class LimitDefinition {
protected:
    bool hasFaultLevel;
    bool hasWarningLevel;

public:
    LimitDefinition();
    virtual ~LimitDefinition();

    void updateBase(bool hasWarningLevel, bool hasFaultLevel);

    virtual void reset();

    virtual Limit::Type check(a value);
};

class TimedLimitDefinition : public LimitDefinition<Limit::Type> {
protected:
    int period;
    int warningPeriod;
    int faultPeriod;
    std::queue<Limit::Type> history;
    int warningCount;
    int faultCount;

public:
    TimedLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, int period, int warningPeriod, int faultPeriod);

    void reset();

    Limit::Type check(Limit::Type value);
};

template <class a>
class EqualLimitDefinition : public LimitDefinition<a> {
protected:
    a warningLevel;
    a faultLevel;

public:
    EqualLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel);

    Limit::Type check(a value);
};

template <class a>
class NotEqualLimitDefinition : public LimitDefinition<a> {
protected:
    a warningLevel;
    a faultLevel;

public:
    NotEqualLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel);

    Limit::Type check(a value);
};

template <class a>
class LessThanLimitDefinition : public LimitDefinition<a> {
protected:
    a warningLevel;
    a faultLevel;

public:
    LessThanLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel);

    Limit::Type check(a value);
};

template <class a>
class LessThanEqualLimitDefinition : public LimitDefinition<a> {
protected:
    a warningLevel;
    a faultLevel;

public:
    LessThanEqualLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel);

    Limit::Type check(a value);
};

template <class a>
class GreaterThanLimitDefinition : public LimitDefinition<a> {
protected:
    a warningLevel;
    a faultLevel;

public:
    GreaterThanLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel);

    Limit::Type check(a value);
};

template <class a>
class GreaterThanEqualLimitDefinition : public LimitDefinition<a> {
protected:
    a warningLevel;
    a faultLevel;

public:
    GreaterThanEqualLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, a warningLevel, a faultLevel);

    Limit::Type check(a value);
};

template <class a>
class InRangeLimitDefinition : public LimitDefinition<a> {
protected:
    a lowWarningLevel;
    a highWarningLevel;
    a lowFaultLevel;
    a highFaultLevel;

public:
    InRangeLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, a lowWarningLevel, a highWarningLevel, a lowFaultLevel, a highFaultLevel);

    Limit::Type check(a value);
};

template <class a>
class NotInRangeLimitDefinition : public LimitDefinition<a> {
protected:
    a lowWarningLevel;
    a highWarningLevel;
    a lowFaultLevel;
    a highFaultLevel;

public:
    NotInRangeLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, a lowWarningLevel, a highWarningLevel, a lowFaultLevel, a highFaultLevel);

    Limit::Type check(a value);
};

class BitSetLimitDefinition : public LimitDefinition<unsigned long> {
protected:
    unsigned long warningMask;
    unsigned long faultMask;

public:
    BitSetLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, int warningBit, int faultBit);

    Limit::Type check(unsigned long value);
};

class NotBitSetLimitDefinition : public LimitDefinition<unsigned long> {
protected:
    unsigned long warningMask;
    unsigned long faultMask;

public:
    NotBitSetLimitDefinition();

    void update(bool hasWarningLevel, bool hasFaultLevel, int warningBit, int faultBit);

    Limit::Type check(unsigned long value);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_DOMAIN_LIMITDEFINITIONS_H_ */
