/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <fstream>
#include <iostream>

#include <NiFpga_M1M3SupportFPGA.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Decodes raw DC accelerometer output. Needs at least one file as argument." << std::endl;
        return -1;
    }
    std::ifstream file;

    for (int i = 1; i <= 8; i++) {
        std::cout << "Acc" << +i;
        if (i < 8) {
            std::cout << ",";
        }
    }

    std::cout << std::endl;

    for (int i = 1; i < argc; i++) {
        try {
            // auto file = std::ifstream(argv[i], std::ifstream::in | std::fstream::binary);
            file.open(argv[i], std::ifstream::binary);
            file.exceptions(std::ios::eofbit | std::ios::badbit | std::ios::failbit);
            int i = 0;
            while (!file.eof()) {
                uint64_t data;
                file.read(reinterpret_cast<char*>(&data) + 5, 3);
                data = be64toh(data);
                std::cout << NiFpga_ConvertFromFxpToFloat(
                        NiFpga_M1M3SupportFPGA_TargetToHostFifoFxp_RawAccelerometer_TypeInfo, data);
                i++;
                if (i == 8) {
                    std::cout << std::endl;
                    i = 0;
                } else {
                    std::cout << ",";
                }
            }
        } catch (const std::ios_base::failure& e) {
            if (file.eof()) {
                continue;
            }
            std::cerr << "Cannot read raw DC accelerometer file " << argv[i] << ": " << e.what() << std::endl;
        }
    }
}
