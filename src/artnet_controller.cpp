#include "artnet_controller.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ArtNetController::_bind_methods() {
	ClassDB::bind_method(D_METHOD("configure", "bind_address", "port", "net", "subnet", "universe", "broadcast_address"), &ArtNetController::configure, DEFVAL("255.255.255.255"));
	ClassDB::bind_method(D_METHOD("start"), &ArtNetController::start);
	ClassDB::bind_method(D_METHOD("stop"), &ArtNetController::stop);
	ClassDB::bind_method(D_METHOD("is_running"), &ArtNetController::is_running);
	ClassDB::bind_method(D_METHOD("set_dmx_data", "universe", "data"), &ArtNetController::set_dmx_data);
	ClassDB::bind_method(D_METHOD("send_dmx"), &ArtNetController::send_dmx);
}

ArtNetController::ArtNetController() {
	controller = new ArtNet::ArtNetController();
}

ArtNetController::~ArtNetController() {
	if (controller) {
		controller->stop();
		delete controller;
		controller = nullptr;
	}
}

bool ArtNetController::configure(const String &bind_address, int port, int net, int subnet, int universe, const String &broadcast_address) {
	if (!controller) {
		return false;
	}

	std::string bind_addr = std::string(bind_address.utf8().get_data());
	std::string broadcast_addr = std::string(broadcast_address.utf8().get_data());

	return controller->configure(bind_addr, port, static_cast<uint8_t>(net), static_cast<uint8_t>(subnet), static_cast<uint8_t>(universe), broadcast_addr);
}

bool ArtNetController::start() {
	if (!controller) {
		return false;
	}
	return controller->start();
}

void ArtNetController::stop() {
	if (controller) {
		controller->stop();
	}
}

bool ArtNetController::is_running() const {
	if (!controller) {
		return false;
	}
	return controller->isRunning();
}

bool ArtNetController::set_dmx_data(int universe, const PackedByteArray &data) {
	if (!controller) {
		return false;
	}

	// Convert PackedByteArray to std::vector<uint8_t>
	std::vector<uint8_t> dmx_data;
	dmx_data.reserve(data.size());
	for (int i = 0; i < data.size(); i++) {
		dmx_data.push_back(data[i]);
	}

	return controller->setDmxData(static_cast<uint16_t>(universe), dmx_data);
}

bool ArtNetController::send_dmx() {
	if (!controller) {
		return false;
	}
	return controller->sendDmx();
}

