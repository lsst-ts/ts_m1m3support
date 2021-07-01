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

#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

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

TEST_CASE("InRangeTrigger", "[Range]") {
    SimpleLimitTrigger trigger(2);
    REQUIRE(Range::InRangeTrigger(1.23, 1.25, 1.24, trigger, 1) == true);
    REQUIRE(trigger.executedVal == 0);

    REQUIRE(Range::InRangeTrigger(1.23, 1.25, 1.26, trigger, 2) == false);
    REQUIRE(trigger.executedVal == 0);

    REQUIRE(Range::InRangeTrigger(1.23, 1.25, 1.22, trigger, 3) == false);
    REQUIRE(trigger.executedVal == 3);
}
