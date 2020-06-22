#include <GyroSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void GyroSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    this->DataRate =
            boost::lexical_cast<int>(doc.select_node("//GyroSettings/DataRate").node().child_value());
    this->AngularVelocityXOffset = boost::lexical_cast<float>(
            doc.select_node("//GyroSettings/AngularVelocityXOffset").node().child_value());
    this->AngularVelocityYOffset = boost::lexical_cast<float>(
            doc.select_node("//GyroSettings/AngularVelocityYOffset").node().child_value());
    this->AngularVelocityZOffset = boost::lexical_cast<float>(
            doc.select_node("//GyroSettings/AngularVelocityZOffset").node().child_value());
    std::string axesMatrix = doc.select_node("//GyroSettings/AxesMatrix").node().child_value();
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    tokenizer tokenize(axesMatrix);
    this->AxesMatrix.clear();
    for (tokenizer::iterator token = tokenize.begin(); token != tokenize.end(); ++token) {
        this->AxesMatrix.push_back(boost::lexical_cast<double>(*token));
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
