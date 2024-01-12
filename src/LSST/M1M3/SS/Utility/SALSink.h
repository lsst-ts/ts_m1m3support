/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Telescope and Site Systems.
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

#ifndef SALSINK_H_
#define SALSINK_H_

#include "spdlog/sinks/base_sink.h"

#include <SAL_MTM1M3.h>

#include <memory>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Sink to send all M1M3 spdlog messages to SAL using logMessage event.
 *
 * @see https://github.com/gabime/spdlog/wiki/4.-Sinks
 */
template <typename Mutex>
class SALSink : public spdlog::sinks::base_sink<Mutex> {
public:
    SALSink(std::shared_ptr<SAL_MTM1M3> m1m3SAL) {
        _m1m3SAL = m1m3SAL;
        _m1m3SAL->salEventPub((char *)"MTM1M3_logevent_logMessage");
    }

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

        MTM1M3_logevent_logMessageC message;
        message.name = "MTM1M3";
        message.level = msg.level * 10;
        message.message = fmt::to_string(msg.payload);
        message.traceback = "";
        message.filePath = msg.source.filename;
        message.functionName = msg.source.funcname;
        message.lineNumber = msg.source.line;
        message.process = getpid();

        _m1m3SAL->logEvent_logMessage(&message, 0);
    }

    void flush_() override {}

private:
    std::shared_ptr<SAL_MTM1M3> _m1m3SAL;
};

#include "spdlog/details/null_mutex.h"
#include <mutex>
using SALSink_mt = SALSink<std::mutex>;
using SALSink_st = SALSink<spdlog::details::null_mutex>;

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // SALSINK_H_
