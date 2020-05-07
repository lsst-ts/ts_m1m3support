#include <PPSThread.h>
#include <FPGA.h>
#include <M1M3SSPublisher.h>
#include <spdlog/spdlog.h>
#include <Timestamp.h>
#include <FPGAAddresses.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PPSThread::PPSThread(M1M3SSPublisher* publisher) {
    this->publisher = publisher;
    this->keepRunning = true;
}

void PPSThread::run() {
    spdlog::info("PPSThread: Start");
    while (this->keepRunning) {
        if (IFPGA::get().waitForPPS(2500) == 0) {
            IFPGA::get().ackPPS();
            uint64_t timestamp = Timestamp::toFPGA(this->publisher->getTimestamp());
            if (this->keepRunning) {
                IFPGA::get().writeTimestampFIFO(timestamp);
            }
        } else {
            spdlog::warn("PPSThread: Failed to receive pps");
        }
    }
    spdlog::info("PPSThread: Completed");
}

void PPSThread::stop() { this->keepRunning = false; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
