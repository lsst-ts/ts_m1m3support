#ifndef EXPANSIONFPGAAPPLICATIONSETTINGS_H_
#define EXPANSIONFPGAAPPLICATIONSETTINGS_H_

#include <DataTypes.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ExpansionFPGAApplicationSettings {
    bool Enabled;
    std::string Resource;

    void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* EXPANSIONFPGAAPPLICATIONSETTINGS_H_ */
