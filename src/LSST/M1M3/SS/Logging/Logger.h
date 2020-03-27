#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdarg.h>
#include <ctime>

namespace LSST {
namespace M1M3 {
namespace SS {

struct Levels {
    enum Type { Trace = 1, Debug = 2, Info = 3, Warn = 4, Error = 5, Fatal = 6 };
};

struct Loggers {
    enum Type {
        Main = 1 << 0,
        Accelerometer = 1 << 1,
        AirController = 1 << 2,
        AutomaticOperationsController = 1 << 3,
        BusLists = 1 << 4,
        CommandFactory = 1 << 5,
        Commands = 1 << 6,
        Context = 1 << 7,
        Controller = 1 << 8,
        Displacement = 1 << 9,
        ForceController = 1 << 10,
        FPGA = 1 << 11,
        Gyro = 1 << 12,
        ILC = 1 << 13,
        Inclinometer = 1 << 14,
        Include = 1 << 15,
        InterlockController = 1 << 16,
        Modbus = 1 << 17,
        Model = 1 << 18,
        PID = 1 << 19,
        PositionController = 1 << 20,
        PowerController = 1 << 21,
        ProfileController = 1 << 22,
        Publisher = 1 << 23,
        SafetyController = 1 << 24,
        Settings = 1 << 25,
        StateFactory = 1 << 26,
        States = 1 << 27,
        Subscriber = 1 << 28,
        Threads = 1 << 29,
        Utility = 1 << 30
    };
};

#ifdef __GNUC__
#define PRINTFFORMAT __attribute__((format(printf, 2, 3)))
#else
#define PRINTFFORMAT
#endif

class Logger {
private:
    Levels::Type LOGGER_LEVEL;

public:
    void SetLevel(Levels::Type level);

    void Trace(const char* format, ...) PRINTFFORMAT;
    void Debug(const char* format, ...) PRINTFFORMAT;
    void Info(const char* format, ...) PRINTFFORMAT;
    void Warn(const char* format, ...) PRINTFFORMAT;
    void Error(const char* format, ...) PRINTFFORMAT;
    void Fatal(const char* format, ...) PRINTFFORMAT;

private:
    void log(const char* color, const char* level, const char* message);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LOGGER_H_ */
