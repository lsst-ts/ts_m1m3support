#!/usr/bin/env python

# This file is part of LSST M1M3 SS test suite. Tests Simulator run.
# 
# Developed for the LSST Telescope and Site Systems.
# This product includes software developed by the LSST Project
# (https://www.lsst.org).
# See the COPYRIGHT file at the top-level directory of this distribution
# for details of code ownership.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import asyncio

from lsst.ts.salobj import Remote,Domain

async def run():
    async with Domain() as domain:
        async with Remote(domain=domain, name="MTM1M3", start=True) as m1m3:
            await m1m3.cmd_start.start(timeout=20)
            await m1m3.cmd_enable.start()
            assert m1m3.evt_raisingLoweringInfo.get().weightSupportedPercent == 0
            await m1m3.cmd_raiseM1M3.start()
            await asyncio.sleep(3)
            await m1m3.cmd_abortRaiseM1M3.start()
            await m1m3.cmd_disable.start()
            await m1m3.cmd_standby.start()
            await m1m3.cmd_exitControl.start()

asyncio.run(run())
