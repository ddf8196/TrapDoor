//
// Created by xhy on 2020/12/12.
//

#include "Graphics.h"
#include <string>
#include "Particle.h"

std::map<float, int> graphics::binSplit(float start, float end) {
	std::map<float, int> lengthMap;
	int length = static_cast<int>(end - start);

	while (length >= 512) {
		length -= 256;
		auto point = static_cast<float>(128.0 + start);
		start += 256.0;
		lengthMap.insert({point, 256});
	}

	for (auto defaultLength = 256; defaultLength >= 1; defaultLength /= 2) {
		if (length >= defaultLength) {
			length -= defaultLength;
			auto point = static_cast<float>(0.5 * defaultLength + start);
			start += defaultLength;
			lengthMap.insert({point, defaultLength});
		}
	}

	/*
	const int defaultLength = 8;
	int numOf8 = length / defaultLength;
	for (auto i = 0; i < numOf8; i++) {
		auto point = static_cast<float>(1.0 * i * defaultLength + start + 4);
		lengthMap.insert({point, defaultLength});
	}


	for (auto j = numOf8 * defaultLength + static_cast<int>(start); j < end;
	j++) { lengthMap.insert({1.0 * j + 0.5, 1});
	}*/
	return lengthMap;
}

void graphics::drawLine(const Vec3& originPoint,
						FACING direction,
						float length,
						COLOR color) {
	if (length <= 0)
		return;
	float start = 0, end = 0;
	switch (direction) {
		case FACING::NEG_Y:
			start = originPoint.y - length;
			end = originPoint.y;
			break;
		case FACING::POS_Y:
			start = originPoint.y;
			end = originPoint.y + length;
			break;
		case FACING::NEG_Z:
			start = originPoint.z - length;
			end = originPoint.z;
			break;
		case FACING::POS_Z:
			start = originPoint.z;
			end = originPoint.z + length;
			break;
		case FACING::NEG_X:
			start = originPoint.x - length;
			end = originPoint.x;
			break;
		case FACING::POS_X:
			start = originPoint.x;
			end = originPoint.x + length;
			break;
	}
	// split point list;

	auto list = binSplit(start, end);
	std::map<Vec3, int> positionList;
	if (facingIsX(direction)) {
		for (auto i : list)
			positionList.insert(
				{{i.first, originPoint.y, originPoint.z}, i.second});
	} else if (facingIsY(direction)) {
		for (auto i : list)
			positionList.insert(
				{{originPoint.x, i.first, originPoint.z}, i.second});
	} else if (facingIsZ(direction)) {
		for (auto i : list)
			positionList.insert(
				{{originPoint.x, originPoint.y, i.first}, i.second});
	}

	for (auto points : positionList) {
		// if (points.second == 8) {
		auto particleType =
			getLineParticleType(points.second, direction, color);
		auto particleTypeinv =
			getLineParticleType(points.second, invFacing(direction), color);
		spawnParticle(points.first, particleType);
		spawnParticle(points.first, particleTypeinv);
	}
}

std::string graphics::getLineParticleType(int length,
										  FACING direction,
										  graphics::COLOR color) {
	std::string str = "trapdoor:line";
	str += std::to_string(length);
	switch (direction) {
		case FACING::NEG_Y:
			str += "Yp";
			break;
		case FACING::POS_Y:
			str += "Ym";
			break;
		case FACING::NEG_Z:
			str += "Zp";
			break;
		case FACING::POS_Z:
			str += "Zm";
			break;
		case FACING::NEG_X:
			str += "Xp";
			break;
		case FACING::POS_X:
			str += "Xm";
			break;
	}

	switch (color) {
		case COLOR::WHITE:
			str += "W";
			break;
		case COLOR::RED:
			str += "R";
			break;
		case COLOR::YELLOW:
			str += "Y";
			break;
		case COLOR::BLUE:
			str += "B";
			break;
		case COLOR::GREEN:
			str += "G";
			break;
	}
	// return "minecraft:dragon_breath_trail";
	return str;
}