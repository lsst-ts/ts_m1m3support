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

#ifndef LIMITLOG_H_
#define LIMITLOG_H_

/**
 * Defines time guard error log with counter. Log error every tg seconds.
 *
 * @param tg time guard, chrono literal how often logging shall be done
 * @param ... __VA_ARGS__ passed to SPDLOG_ERROR
 */
#define TG_LOG_ERROR(tg, ...)                                                                                \
    {                                                                                                        \
        static auto last_executed =                                                                          \
                std::chrono::steady_clock::now() - std::chrono::duration_cast<std::chrono::seconds>(tg * 2); \
        if ((std::chrono::steady_clock::now() - last_executed) > tg) {                                       \
            SPDLOG_ERROR(__VA_ARGS__);                                                                       \
            last_executed = std::chrono::steady_clock::now();                                                \
        }                                                                                                    \
    }

/**
 * Defines time guard warning log with counter. Log warning every tg seconds.
 *
 * @param tg time guard, chrono literal how often logging shall be done
 * @param ... __VA_ARGS__ passed to SPDLOG_ERROR
 */
#define TG_LOG_WARN(tg, ...)                                                                                 \
    {                                                                                                        \
        static auto last_executed =                                                                          \
                std::chrono::steady_clock::now() - std::chrono::duration_cast<std::chrono::seconds>(tg * 2); \
        if ((std::chrono::steady_clock::now() - last_executed) > tg) {                                       \
            SPDLOG_WARN(__VA_ARGS__);                                                                        \
            last_executed = std::chrono::steady_clock::now();                                                \
        }                                                                                                    \
    }

#endif  // !LIMITLOG_H_
