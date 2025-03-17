/*
 * This file is part of LSST M1M3 tests. Tests Range functions.
 *
 * Developed for the Telescope & Site Software Systems.  This product includes
 * software developed by the LSST Project (https://www.lsst.org). See the
 * COPYRIGHT file at the top-level directory of this distribution for details
 * of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <catch2/catch_all.hpp>

#include <Range.h>

using namespace LSST::M1M3::SS;

class SimpleLimitTrigger : public LimitTrigger<int> {
public:
    SimpleLimitTrigger(int occurences) : executedVal(0) {
        reset();
        _occurences = occurences;
    }

    int executedVal;

protected:
    bool trigger() override { return ((++_count) % _occurences) == 0; }
    void execute(int val) override { executedVal = val; }
    void reset() override { _count = 0; }

private:
    int _count;
    int _occurences;
};

TEST_CASE("InRangeAndCoerce integer", "[Range]") {
    int ival = 10;
    CHECK(Range::InRangeAndCoerce(1, 4, ival, ival) == false);
    CHECK(ival == 4);

    CHECK(Range::InRangeAndCoerce(1, 4, ival, ival) == true);

    CHECK(Range::InRangeAndCoerce(5, 8, ival, ival) == false);
    CHECK(ival == 5);

    CHECK(Range::InRangeAndCoerce(5, 8, ival, ival) == true);
}

TEST_CASE("InRangeAndCoerce double", "[Range]") {
    double dval = 10.123;
    CHECK(Range::InRangeAndCoerce(1.23, 4.56, dval, dval) == false);
    CHECK(dval == 4.56);

    CHECK(Range::InRangeAndCoerce(1.23, 4.56, dval, dval) == true);

    CHECK(Range::InRangeAndCoerce(5.81, 8.92, dval, dval) == false);
    CHECK(dval == 5.81);

    CHECK(Range::InRangeAndCoerce(5.81, 8.92, dval, dval) == true);
}

TEST_CASE("InRangeTrigger", "[Range]") {
    SimpleLimitTrigger trigger(2);
    CHECK(Range::InRangeTrigger(1.23, 1.25, 1.24, trigger, 1) == true);
    CHECK(trigger.executedVal == 0);

    CHECK(Range::InRangeTrigger(1.23, 1.25, 1.26, trigger, 2) == false);
    CHECK(trigger.executedVal == 0);

    CHECK(Range::InRangeTrigger(1.23, 1.25, 1.22, trigger, 3) == false);
    CHECK(trigger.executedVal == 3);
}
