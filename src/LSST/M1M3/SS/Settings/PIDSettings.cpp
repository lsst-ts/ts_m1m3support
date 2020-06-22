#include <PIDSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void PIDSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    this->Fx.Timestep =
            boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fx/Timestep").node().child_value());
    this->Fx.P = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fx/P").node().child_value());
    this->Fx.I = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fx/I").node().child_value());
    this->Fx.D = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fx/D").node().child_value());
    this->Fx.N = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fx/N").node().child_value());
    this->Fy.Timestep =
            boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fy/Timestep").node().child_value());
    this->Fy.P = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fy/P").node().child_value());
    this->Fy.I = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fy/I").node().child_value());
    this->Fy.D = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fy/D").node().child_value());
    this->Fy.N = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fy/N").node().child_value());
    this->Fz.Timestep =
            boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fz/Timestep").node().child_value());
    this->Fz.P = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fz/P").node().child_value());
    this->Fz.I = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fz/I").node().child_value());
    this->Fz.D = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fz/D").node().child_value());
    this->Fz.N = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Fz/N").node().child_value());
    this->Mx.Timestep =
            boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mx/Timestep").node().child_value());
    this->Mx.P = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mx/P").node().child_value());
    this->Mx.I = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mx/I").node().child_value());
    this->Mx.D = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mx/D").node().child_value());
    this->Mx.N = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mx/N").node().child_value());
    this->My.Timestep =
            boost::lexical_cast<double>(doc.select_node("//PIDSettings/My/Timestep").node().child_value());
    this->My.P = boost::lexical_cast<double>(doc.select_node("//PIDSettings/My/P").node().child_value());
    this->My.I = boost::lexical_cast<double>(doc.select_node("//PIDSettings/My/I").node().child_value());
    this->My.D = boost::lexical_cast<double>(doc.select_node("//PIDSettings/My/D").node().child_value());
    this->My.N = boost::lexical_cast<double>(doc.select_node("//PIDSettings/My/N").node().child_value());
    this->Mz.Timestep =
            boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mz/Timestep").node().child_value());
    this->Mz.P = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mz/P").node().child_value());
    this->Mz.I = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mz/I").node().child_value());
    this->Mz.D = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mz/D").node().child_value());
    this->Mz.N = boost::lexical_cast<double>(doc.select_node("//PIDSettings/Mz/N").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
