#include "./Patch.h"

std::optional<std::vector<uint8_t>> Patch::stringToBytes(const std::string& byteString) {
	std::vector<uint8_t> bytes;
	std::istringstream stream(byteString);
	std::string byteStr;

	try {
		while (stream >> byteStr) {
			uint8_t byte = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
			bytes.push_back(byte);
		}
	}
	catch (const std::exception&) {
		return std::nullopt;
	}

	return bytes;
}

std::optional<size_t> Patch::hexStringToAddress(const std::string& hexString) {
	size_t address = 0;
	try {
		address = std::stoull(hexString, nullptr, 16);
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
	return address;
}

bool Patch::init(std::string addressString, std::string onBytesString, std::string offBytesString) {

	auto addressOpt = hexStringToAddress(addressString);
	auto onBytesOpt = stringToBytes(onBytesString);
	auto offBytesOpt = stringToBytes(offBytesString);


	if (!onBytesOpt.has_value() || !offBytesOpt.has_value() || !addressOpt.has_value()) return false;

	this->address = addressOpt.value();
	this->onBytes = onBytesOpt.value();
	this->offBytes = offBytesOpt.value();

	return true;
}

void Patch::patch(size_t address, std::vector<std::uint8_t> bytes) {
	auto size = bytes.size();
	DWORD old_prot;

	size_t absolute_address = base + address;

	VirtualProtect(reinterpret_cast<void*>(absolute_address), size, PAGE_EXECUTE_READWRITE, &old_prot);
	memcpy(reinterpret_cast<void*>(absolute_address), bytes.data(), size);
	VirtualProtect(reinterpret_cast<void*>(absolute_address), size, old_prot, &old_prot);
}

void Patch::apply(bool isActive) {
	patch(address, isActive ? onBytes : offBytes);
}

Patch* Patch::create(std::string addressString, std::string onBytesString, std::string offBytesString) {
	auto ret = new Patch();
	if (ret && ret->init(addressString, onBytesString, offBytesString)) {
		return ret;
	}
	delete ret;
	return nullptr;
}