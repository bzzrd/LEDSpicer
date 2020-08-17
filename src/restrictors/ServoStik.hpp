/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/**
 * @file      ServoStik.hpp
 * @since     Jul 9, 2020
 * @author    Patricio A. Rossi (MeduZa)
 *
 * @copyright Copyright © 2020 Patricio A. Rossi (MeduZa)
 *
 * @copyright LEDSpicer is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * @copyright LEDSpicer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * @copyright You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Restrictor.hpp"
#include "Brands.hpp"

#ifndef RESTRICTORS_SERVOSTIK_HPP_
#define RESTRICTORS_SERVOSTIK_HPP_ 1

#define SERVOSTIK_NAME       "ServoStick"
#define SERVOSTIK_PRODUCT    0x1700
#define SERVOSTIK_INTERFACE  0
#define SERVOSTIK_WVALUE     0x0200
#define SERVOSTIK_MAX_BOARDS 4

namespace LEDSpicer {
namespace Restrictors {

/**
 * LEDSpicer::Restrictor::ServoStick
 */
class ServoStik : public Restrictor {

public:

	ServoStik(umap<string, string>& options) :
		Restrictor(
			options,
			SERVOSTIK_WVALUE,
			SERVOSTIK_INTERFACE,
			Utility::parseNumber(options["boardId"], "Invalid Board ID"),
			SERVOSTIK_MAX_BOARDS
		) {}


	virtual ~ServoStik() = default;

	virtual void rotate(Ways way);

	virtual uint16_t getVendor();

	virtual uint16_t getProduct();

	virtual string getName();
};

} /* namespace Restrictors */
} /* namespace LEDSpicer */

#endif /* RESTRICTORS_SERVOSTIK_HPP_ */
