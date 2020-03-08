/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/**
 * @file      FrameActor.cpp
 * @since     Jul 18, 2019
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

#include "FrameActor.hpp"

using namespace LEDSpicer::Animations;

FrameActor::FrameActor(
	umap<string, string>& parameters,
	Group* const group,
	const vector<string>& requiredParameters
) :
	Actor(parameters, group, requiredParameters),
	Speed(parameters["speed"]),
	startFrame(parameters.count("startFrame") ? Utility::parseNumber(parameters["startFrame"], "Invalid Value for start frame") : 0),
	cycles(parameters.count("cycles") ? Utility::parseNumber(parameters["cycles"], "Invalid Value for cycles") : 0)
{
	// default frames calculation.
	switch (speed) {
	case Speeds::VeryFast:
		totalFrames = FPS / 3;
		break;
	case Speeds::Fast:
		totalFrames = FPS / 2;
		break;
	case Speeds::Normal:
		totalFrames = FPS;
		break;
	case Speeds::Slow:
		totalFrames = FPS * 2;
		break;
	case Speeds::VerySlow:
		totalFrames = FPS * 3;
		break;
	}
	Utility::verifyValue(startFrame, static_cast<uint8_t>(0), totalFrames);
}

void FrameActor::drawConfig() {
	cout <<
		"Speed: " << speed2str(speed) << endl <<
		"Total Frames: " << static_cast<uint>(totalFrames) << endl;
	Actor::drawConfig();
}

bool FrameActor::isFirstFrame() const {
	return currentFrame == 0;
}

bool FrameActor::isLastFrame() const {
	return currentFrame == totalFrames;
}

void FrameActor::draw() {
	if (drawing)
		Actor::draw();
	advanceFrame();
}

bool FrameActor::isRunning() {

	if (not Actor::isRunning())
		return false;

	if (not drawing) {
		if (startFrame == currentFrame + 1) {
#ifdef DEVELOP
			LogDebug("Starting Actor at frame " + to_string(startFrame));
#endif
			drawing = true;
			return true;
		}
		advanceFrame();
		return false;
	}

	if (cycles) {
		if (cycle > cycles)
			return false;

		if (cycle == cycles) {
#ifdef DEVELOP
			LogDebug("Ending Actor after " + to_string(cycles) + " cycles.");
#endif
			++cycle;
			return false;
		}
		if (isLastFrame())
			++cycle;
	}

	return true;
}

void FrameActor::advanceFrame() {
	if (currentFrame == totalFrames) {
		currentFrame = 0;
		return ;
	}
	++currentFrame;
}
