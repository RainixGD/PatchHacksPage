#pragma once
#include "./includes.h"

class Patch {
	size_t address;
	std::vector<uint8_t> onBytes;
	std::vector<uint8_t> offBytes;

	std::optional<std::vector<uint8_t>> stringToBytes(const std::string& byteString);
	std::optional<size_t> hexStringToAddress(const std::string& hexString);
	bool init(std::string addressString, std::string onBytesString, std::string offBytesString);
	void patch(size_t address, std::vector<std::uint8_t> bytes);

	Patch() {}

public:

	void apply(bool isActive);

	static Patch* create(std::string addressString, std::string onBytesString, std::string offBytesString);
};