#include <PositionControllerSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void PositionControllerSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    this->ForceToStepsCoefficient = boost::lexical_cast<double>(
            doc.select_node("//PositionControllerSettings/ForceToStepsCoefficient").node().child_value());
    this->EncoderToStepsCoefficient = boost::lexical_cast<double>(
            doc.select_node("//PositionControllerSettings/EncoderToStepsCoefficient").node().child_value());
    this->MaxStepsPerLoop = boost::lexical_cast<int32_t>(
            doc.select_node("//PositionControllerSettings/MaxStepsPerLoop").node().child_value());
    this->RaiseLowerForceLimitLow = boost::lexical_cast<double>(
            doc.select_node("//PositionControllerSettings/RaiseLowerForceLimitLow").node().child_value());
    this->RaiseLowerForceLimitHigh = boost::lexical_cast<double>(
            doc.select_node("//PositionControllerSettings/RaiseLowerForceLimitHigh").node().child_value());
    this->RaiseLowerTimeoutInSeconds = boost::lexical_cast<double>(
            doc.select_node("//PositionControllerSettings/RaiseLowerTimeoutInSeconds").node().child_value());
    this->ReferencePositionEncoder1 = boost::lexical_cast<int32_t>(
            doc.select_node("//PositionControllerSettings/ReferencePosition/HP1Encoder")
                    .node()
                    .child_value());
    this->ReferencePositionEncoder2 = boost::lexical_cast<int32_t>(
            doc.select_node("//PositionControllerSettings/ReferencePosition/HP2Encoder")
                    .node()
                    .child_value());
    this->ReferencePositionEncoder3 = boost::lexical_cast<int32_t>(
            doc.select_node("//PositionControllerSettings/ReferencePosition/HP3Encoder")
                    .node()
                    .child_value());
    this->ReferencePositionEncoder4 = boost::lexical_cast<int32_t>(
            doc.select_node("//PositionControllerSettings/ReferencePosition/HP4Encoder")
                    .node()
                    .child_value());
    this->ReferencePositionEncoder5 = boost::lexical_cast<int32_t>(
            doc.select_node("//PositionControllerSettings/ReferencePosition/HP5Encoder")
                    .node()
                    .child_value());
    this->ReferencePositionEncoder6 = boost::lexical_cast<int32_t>(
            doc.select_node("//PositionControllerSettings/ReferencePosition/HP6Encoder")
                    .node()
                    .child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
