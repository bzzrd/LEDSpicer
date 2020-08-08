/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/**
 * @file      Filler.cpp
 * @since     Oct 26, 2018
 * @author    Patricio A. Rossi (MeduZa)
 *
 * @copyright Copyright © 2018 - 2020 Patricio A. Rossi (MeduZa)
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

#include "Filler.hpp"

using namespace LEDSpicer::Animations;

Filler::Filler(umap<string, string>& parameters, Group* const group) :
	StepActor(parameters, group, REQUIRED_PARAM_ACTOR_FILLER),
	Color(parameters["color"]),
	mode(str2mode(parameters["mode"]))
{
	if (mode == Modes::Random) {
		previousFrameAffectedElements.resize(totalFrames + 1, false);
		previousFrameAffectedElements.shrink_to_fit();
		cDirection = direction = Directions::Forward;
		generateNextRandom();
	}
}

void Filler::generateNextRandom() {
	// Extract candidates.
	vector<uint8_t> possibleElements;
	for (uint8_t e = 0; e <= totalFrames; ++e)
		if ((filling and not previousFrameAffectedElements[e]) or (not filling and previousFrameAffectedElements[e]))
			possibleElements.push_back(e);

	if (not possibleElements.size())
		// None left
		return;
	else if (possibleElements.size() > 1)
		// Roll dice.
		currentRandom = possibleElements[std::rand() / ((RAND_MAX + 1u) / (possibleElements.size() - 1))];
	else
		// only one left
		currentRandom = possibleElements[0];
}

void Filler::calculateElements() {
#ifdef DEVELOP
	cout << "Filler: " << DrawDirection(cDirection) << (filling ? " Filling " : " Emptying ");
#endif
	switch (mode) {
	// Normal
	case Modes::Normal:
		if (cDirection == Directions::Forward) {
			if (filling)
				fillElementsLinear({
					0,
					currentFrame,
					Directions::Forward,
				});
			else
				fillElementsLinear({
					currentFrame,
					totalFrames,
					Directions::Backward,
				});
		}
		else {
			if (filling)
				fillElementsLinear({
					currentFrame,
					totalFrames,
					Directions::Forward
				});
			else
				fillElementsLinear({
					0,
					currentFrame,
					Directions::Backward
				});
		}
		break;
	// Forward or backward will work in the same way for random modes.
	case Modes::Random:
		fillElementsRandom();
		break;
	}

	if (currentStepFrame == totalStepFrames and ((cDirection == Directions::Forward and currentFrame == totalFrames) or (cDirection == Directions::Backward and currentFrame == 0))) {
		filling = not filling;
		if (mode == Modes::Random)
			generateNextRandom();
	}
}

void Filler::fillElementsLinear(const Data& values) {
	for (uint8_t c = values.begin; c <= values.end; ++c)
		if ( c != currentFrame) {
			changeElementColor(c, *this, filter);
#ifdef DEVELOP
			cout << static_cast<int>(c) << " ";
#endif
		}
	if (speed != Speeds::VeryFast)
		changeFrameElement(*this, values.dir);
}

void Filler::fillElementsRandom() {

	for (uint8_t e = 0; e <= totalFrames; ++e)
		if (previousFrameAffectedElements[e]) {
			changeElementColor(e, *this, filter);
#ifdef DEVELOP
			cout << static_cast<int>(e) << " ";
#endif
		}
	if (speed == Speeds::VeryFast)
		changeElementColor(currentRandom, *this, filter);
	else
		changeFrameElement(currentRandom, *this, filling ? Directions::Forward : Directions::Backward);

	if (currentStepFrame == totalStepFrames) {
		previousFrameAffectedElements[currentRandom] = filling;
		generateNextRandom();
	}
}

void Filler::drawConfig() {
	cout <<
		"Type: Filler" << endl <<
		"Mode: " << mode2str(mode) << endl;
	StepActor::drawConfig();
	cout << "Color: ";
	this->drawColor();
	cout << endl << SEPARATOR << endl;
}

string Filler::mode2str(Modes mode) {
	switch (mode) {
	case Modes::Normal:
		return "Normal";
	case Modes::Random:
		return "Random";
	}
	return "";
}

Filler::Modes Filler::str2mode(const string& mode) {
	if (mode == "Normal")
		return Modes::Normal;
	if (mode == "Random")
		return Modes::Random;
	throw Error("Invalid mode " + mode);
}

void Filler::restart() {
	StepActor::restart();
	filling = true;
	if (mode == Modes::Random) {
		std::fill(previousFrameAffectedElements.begin(), previousFrameAffectedElements.end(), false);
		generateNextRandom();
	}
}
