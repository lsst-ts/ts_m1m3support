#ifndef XMLDOCLOAD_H
#define XMLDOCLOAD_H

#include <pugixml.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Try to load XML document.
 *
 * @param filename filename to load
 * @param doc returned document
 *
 * @throw std::runtime_error on error
 */
void XMLDocLoad(const std::string& filename, pugi::xml_document &doc);

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* XMLDOCLOAD_H */
