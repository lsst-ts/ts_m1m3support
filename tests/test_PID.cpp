/*
 * This file is part of LSST M1M3 SS test suite. Tests software PID.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#include <memory>
#include <cmath>
#include <iostream>

#include <SAL_MTM1M3.h>

#include <PID.h>

using namespace LSST::M1M3::SS;

TEST_CASE("Constant_PID", "[PID]") {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    m1m3SAL->setDebugLevel(0);
    M1M3SSPublisher publisher = M1M3SSPublisher(m1m3SAL);

    PID *pids[6];
    PIDParameters pparams;
    pparams.Timestep = 1;
    pparams.P = 1;
    pparams.I = 0;
    pparams.D = 0;
    pparams.N = 1;

    for (int i = 0; i < 6; i++) {
        pids[i] = new PID(i, pparams, &publisher);
    }

    double te = 0;

    for (int i = 0; i < 6; i++) {
        for (int n = 0; n < 1000; n++) {
            te += pids[i]->process(n, n);
        }
    }

    REQUIRE(te == 0);

    for (int i = 0; i < 6; i++) {
        delete pids[i];
    }
}

TEST_CASE("PID_convergence", "[PID]") {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    m1m3SAL->setDebugLevel(0);
    M1M3SSPublisher publisher = M1M3SSPublisher(m1m3SAL);

    PID *pids[6];
    PIDParameters pparams;
    pparams.Timestep = 0.1;
    pparams.P = 0.5;
    pparams.I = 0.4;
    pparams.D = 0.1;
    pparams.N = 0.2;

    for (int i = 0; i < 6; i++) {
        pids[i] = new PID(i, pparams, &publisher);
    }

    for (int i = 0; i < 6; i++) {
        double u0 = pids[i]->process(1000, 0);
        REQUIRE(u0 == 520);
        for (int n = 0; n < 1000; n += 100) {
            REQUIRE(fabs(pids[i]->process(1000, n)) < u0 * 1.2);
        }

        double m = 1000;

        int n = 1000000;
        bool eLow = false;
        double u;

        for (; n > 0; n -= 1) {
            u = pids[i]->process(1000, m);

            m = 1000 + (n / 10000) * sin((180 * M_PI) / n);

            if (fabs(u) < 1 && eLow == false) {
                REQUIRE(n < 890000);
                eLow = true;
            }
        }

        REQUIRE(u < 1);
    }

    for (int i = 0; i < 6; i++) {
        delete pids[i];
    }
}
