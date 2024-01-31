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

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

#define __WINDOWS__

#endif

#if defined(__linux__) || defined(__CYGWIN__)

#include <endian.h>

#elif defined(__APPLE__)

#include <libkern/OSByteOrder.h>

#define be64toh(x) OSSwapBigToHostInt64(x)

#define __BYTE_ORDER BYTE_ORDER
#define __BIG_ENDIAN BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#define __PDP_ENDIAN PDP_ENDIAN

#elif defined(__OpenBSD__)

#include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#include <sys/endian.h>

#define be64toh(x) betoh64(x)

#elif defined(__WINDOWS__)

#include <windows.h>

#if BYTE_ORDER == LITTLE_ENDIAN

#if defined(_MSC_VER)
#include <stdlib.h>
#define be64toh(x) _byteswap_uint64(x)

#elif defined(__GNUC__) || defined(__clang__)

#define be64toh(x) __builtin_bswap64(x)
#else
#error platform not supported
#endif

#else

#error byte order not supported

#endif

#define __BYTE_ORDER BYTE_ORDER
#define __BIG_ENDIAN BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#define __PDP_ENDIAN PDP_ENDIAN

#else

#error platform not supported

#endif

#include <fstream>
#include <iostream>

#include <NiFpga_M1M3SupportFPGA.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Decodes raw DC accelerometer output. Needs at least one file "
                     "as argument."
                  << std::endl;
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
            // auto file = std::ifstream(argv[i], std::ifstream::in |
            // std::fstream::binary);
            file.open(argv[i], std::ifstream::binary);
            file.exceptions(std::ios::eofbit | std::ios::badbit | std::ios::failbit);
            int i = 0;
            while (!file.eof()) {
                uint64_t data;
                file.read(reinterpret_cast<char *>(&data) + 5, 3);
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
        } catch (const std::ios_base::failure &e) {
            if (file.eof()) {
                continue;
            }
            std::cerr << "Cannot read raw DC accelerometer file " << argv[i] << ": " << e.what() << std::endl;
        }
    }
}
