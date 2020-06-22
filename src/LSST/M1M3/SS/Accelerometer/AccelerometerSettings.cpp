/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
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

#include <AccelerometerSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void AccelerometerSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);

    this->GsToMetersPerSecondSqrd = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/GsToMetersPerSecondSqrd").node().child_value());
    this->AngularAccelerationXDistance = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/AngularAccelerationXDistance").node().child_value());
    this->AngularAccelerationYDistance = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/AngularAccelerationYDistance").node().child_value());
    this->AngularAccelerationZDistance = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/AngularAccelerationZDistance").node().child_value());
    this->AccelerometerBias[0] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer1/Bias").node().child_value());
    this->AccelerometerSensitivity[0] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer1/Sensitivity").node().child_value());
    this->AccelerometerOffsets[0] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer1/Offset").node().child_value());
    this->AccelerometerScalars[0] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer1/Scalar").node().child_value());
    this->AccelerometerBias[1] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer2/Bias").node().child_value());
    this->AccelerometerSensitivity[1] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer2/Sensitivity").node().child_value());
    this->AccelerometerOffsets[1] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer2/Offset").node().child_value());
    this->AccelerometerScalars[1] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer2/Scalar").node().child_value());
    this->AccelerometerBias[2] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer3/Bias").node().child_value());
    this->AccelerometerSensitivity[2] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer3/Sensitivity").node().child_value());
    this->AccelerometerOffsets[2] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer3/Offset").node().child_value());
    this->AccelerometerScalars[2] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer3/Scalar").node().child_value());
    this->AccelerometerBias[3] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer4/Bias").node().child_value());
    this->AccelerometerSensitivity[3] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer4/Sensitivity").node().child_value());
    this->AccelerometerOffsets[3] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer4/Offset").node().child_value());
    this->AccelerometerScalars[3] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer4/Scalar").node().child_value());
    this->AccelerometerBias[4] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer5/Bias").node().child_value());
    this->AccelerometerSensitivity[4] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer5/Sensitivity").node().child_value());
    this->AccelerometerOffsets[4] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer5/Offset").node().child_value());
    this->AccelerometerScalars[4] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer5/Scalar").node().child_value());
    this->AccelerometerBias[5] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer6/Bias").node().child_value());
    this->AccelerometerSensitivity[5] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer6/Sensitivity").node().child_value());
    this->AccelerometerOffsets[5] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer6/Offset").node().child_value());
    this->AccelerometerScalars[5] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer6/Scalar").node().child_value());
    this->AccelerometerBias[6] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer7/Bias").node().child_value());
    this->AccelerometerSensitivity[6] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer7/Sensitivity").node().child_value());
    this->AccelerometerOffsets[6] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer7/Offset").node().child_value());
    this->AccelerometerScalars[6] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer7/Scalar").node().child_value());
    this->AccelerometerBias[7] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer8/Bias").node().child_value());
    this->AccelerometerSensitivity[7] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer8/Sensitivity").node().child_value());
    this->AccelerometerOffsets[7] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer8/Offset").node().child_value());
    this->AccelerometerScalars[7] = boost::lexical_cast<float>(
            doc.select_node("//AccelerometerSettings/Accelerometer8/Scalar").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
