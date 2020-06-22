#ifndef ALIASAPPLICATIONSETTINGS_H_
#define ALIASAPPLICATIONSETTINGS_H_

#include <Alias.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

struct AliasApplicationSettings {
    std::vector<Alias> Aliases;

    void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ALIASAPPLICATIONSETTINGS_H_ */
