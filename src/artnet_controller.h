#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/string.hpp"

#include "../lib-artnet-4-cpp/artnet/ArtNetController.h"

using namespace godot;

class ArtNetController : public RefCounted {
	GDCLASS(ArtNetController, RefCounted)

protected:
	static void _bind_methods();

private:
	ArtNet::ArtNetController *controller;

public:
	ArtNetController();
	~ArtNetController() override;

	// Configuration
	bool configure(const String &bind_address, int port, int net, int subnet, int universe, const String &broadcast_address = "255.255.255.255");

	// Network Control
	bool start();
	void stop();
	bool is_running() const;

	// DMX Operations
	bool set_dmx_data(int universe, const PackedByteArray &data);
	bool send_dmx();
};

