#include <Settings.h>
#include <boost/lexical_cast.hpp>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

// Settings::Settings() {
//	// TODO Auto-generated constructor stub
//
//}
//
// bool Settings::loadSettings(std::string filePath) {
//	try {
//		this->loadAccelerometerSettings(filePath);
//		return true;
//	}
//	catch (...) {
//		Log.Error("Failed to load settings.");
//		return false;
//	}
//}
//
// void Settings::loadAccelerometerSettings(std::string filePath) {
//	AccelerometerSettings settings;
//
//	pugi::xml_document doc;
//	doc.load_file(filePath.c_str());
//	this->loadLinearFunctionFloat(settings.RawVoltageToCorrectedVoltage[0],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='RawVoltageToCorrectedVoltage' and
//@index='0']").node()); 	this->loadLinearFunctionFloat(settings.RawVoltageToCorrectedVoltage[1],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='RawVoltageToCorrectedVoltage' and
//@index='1']").node()); 	this->loadLinearFunctionFloat(settings.RawVoltageToCorrectedVoltage[2],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='RawVoltageToCorrectedVoltage' and
//@index='2']").node()); 	this->loadLinearFunctionFloat(settings.RawVoltageToCorrectedVoltage[3],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='RawVoltageToCorrectedVoltage' and
//@index='3']").node()); 	this->loadLinearFunctionFloat(settings.RawVoltageToCorrectedVoltage[4],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='RawVoltageToCorrectedVoltage' and
//@index='4']").node()); 	this->loadLinearFunctionFloat(settings.RawVoltageToCorrectedVoltage[5],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='RawVoltageToCorrectedVoltage' and
//@index='5']").node()); 	this->loadLinearFunctionFloat(settings.RawVoltageToCorrectedVoltage[6],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='RawVoltageToCorrectedVoltage' and
//@index='6']").node()); 	this->loadLinearFunctionFloat(settings.RawVoltageToCorrectedVoltage[7],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='RawVoltageToCorrectedVoltage' and
//@index='7']").node());
//	this->loadLinearFunctionFloat(settings.CorrectedVoltageToLinearAcceleration[0],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='CorrectedVoltageToLinearAcceleration'
//and @index='0']").node());
//	this->loadLinearFunctionFloat(settings.CorrectedVoltageToLinearAcceleration[1],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='CorrectedVoltageToLinearAcceleration'
//and @index='1']").node());
//	this->loadLinearFunctionFloat(settings.CorrectedVoltageToLinearAcceleration[2],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='CorrectedVoltageToLinearAcceleration'
//and @index='2']").node());
//	this->loadLinearFunctionFloat(settings.CorrectedVoltageToLinearAcceleration[3],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='CorrectedVoltageToLinearAcceleration'
//and @index='3']").node());
//	this->loadLinearFunctionFloat(settings.CorrectedVoltageToLinearAcceleration[4],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='CorrectedVoltageToLinearAcceleration'
//and @index='4']").node());
//	this->loadLinearFunctionFloat(settings.CorrectedVoltageToLinearAcceleration[5],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='CorrectedVoltageToLinearAcceleration'
//and @index='5']").node());
//	this->loadLinearFunctionFloat(settings.CorrectedVoltageToLinearAcceleration[6],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='CorrectedVoltageToLinearAcceleration'
//and @index='6']").node());
//	this->loadLinearFunctionFloat(settings.CorrectedVoltageToLinearAcceleration[7],
//doc.select_node("//Setting[@model='AccelerometerSystem' and @field='CorrectedVoltageToLinearAcceleration'
//and @index='7']").node());
//}
//
// void Settings::loadLinearFunctionFloat(LinearFunction<float>& function, pugi::xml_node node) {
//	float m = boost::lexical_cast<float>(node.attribute("m").as_string(""));
//	float b = boost::lexical_cast<float>(node.attribute("b").as_string(""));
//	function.update(m, b);
//}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
