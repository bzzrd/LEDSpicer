/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/**
 * @file      Reader.cpp
 * @since     May 23, 2019
 * @author    Patricio A. Rossi (MeduZa)
 *
 * @copyright Copyright © 2019 Patricio A. Rossi (MeduZa)
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

#include "Reader.hpp"

using namespace LEDSpicer::Inputs;

vector<input_event> Reader::events;
umap<string, int> Reader::listenEvents;
Input* Reader::readController = nullptr;

Reader::Reader(umap<string, string>& parameters) : Input(parameters) {
	if (parameters.count("listenEvents"))
		for (auto& e : Utility::explode(parameters["listenEvents"], ',')) {
			Utility::trim(e);
			listenEvents.emplace(e, -1);
		}
}

void Reader::activate() {
	for (auto& l : listenEvents) {
		LogDebug("Opening " DEV_INPUT + l.first);
		l.second = open((DEV_INPUT + l.first).c_str(), O_RDONLY | O_NONBLOCK);
		if (l.second < 0)
			LogWarning("Unable to open " DEV_INPUT + l.first);
	}
}

void Reader::deactivate() {
	for (auto& l : listenEvents) {
		if (l.second < 0)
			return;
		close(l.second);
	}
}

void Reader::readAll(Input* who) {

	if (not readController)
		readController = who;

	if (readController != who)
		return;

	events.clear();
	for (auto& l : listenEvents) {
		if (l.second < 0)
			continue;

		input_event event;
		char buffer[sizeof(event)];
		while (true) {
			ssize_t r = read(l.second, reinterpret_cast<void*>(&event), sizeof(event));
			if (r < 1)
				break;
			if (event.type != EV_KEY) // and event.type != EV_REL))
				continue;
			LogDebug("r: " + std::to_string(r) + "ev: " + std::to_string(event.type) + " code: " + std::to_string(event.code) + string(event.value ? " ON" : " OFF"));
			events.push_back(event);
		}
	}
}