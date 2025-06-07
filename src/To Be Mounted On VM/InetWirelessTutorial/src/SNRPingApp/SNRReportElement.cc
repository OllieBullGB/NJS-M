//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "SNRReportElement.h"

SNRReportElement::SNRReportElement() {
    // TODO Auto-generated constructor stub

}

SNRReportElement::~SNRReportElement() {
    // TODO Auto-generated destructor stub
}

bool SNRReportElement::isValid() const
{
    // Check if node pointer is not null
    if (node == nullptr)
        return false;

    // Check that all relevant doubles are finite and not NaN
    if (!std::isfinite(snr) || !std::isfinite(rsuTransmissionStrength) || !std::isfinite(jammerTransmissionStrength))
        return false;

    // Check coordinates
    if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z))
        return false;

    if (getRsuTransmissionStrength() <= 0 || !std::isfinite(getRsuTransmissionStrength())) {
        return false;
    }

    return true;
}

