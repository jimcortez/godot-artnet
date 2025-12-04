# godot-artnet

A GDExtension plugin for Godot 4.0+ that provides ArtNet DMX transmission capabilities. This plugin wraps the [lib-artnet-4-cpp](https://github.com/gastonmorixe/lib-artnet-4-cpp) library, enabling you to send ArtNet commands from GDScript.

## Features

- Send DMX512 data over Art-Net protocol
- Support for multiple universes
- Thread-safe operations
- Simple GDScript API
- Cross-platform support (Linux, macOS, Windows, Android, iOS, Web)

## Requirements

- Godot 4.1 or higher
- C++17 compatible compiler
- CMake 3.17 or higher (for CMake builds)
- SCons (for SCons builds)
- Network connectivity for ArtNet transmission

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/jecortez/godot-artnet.git
cd godot-artnet
```

### 2. Initialize Submodules

This project depends on `godot-cpp` and `lib-artnet-4-cpp` as git submodules:

```bash
git submodule update --init --recursive
```

### 3. Build the Extension

#### Using SCons (Recommended)

```bash
scons
```

This will build the extension and copy it to the `demo/bin/` directory for your platform.

#### Using CMake

```bash
mkdir build
cd build
cmake ..
make
```

The compiled library will be in `bin/<platform>/` directory.

## Usage

### Basic Example

```gdscript
extends Node

var artnet: ArtNetController

func _ready():
    artnet = ArtNetController.new()
    
    # Configure ArtNet controller
    # Parameters: bind_address, port, net, subnet, universe, broadcast_address
    if not artnet.configure("0.0.0.0", 6454, 0, 0, 0, "255.255.255.255"):
        print("Failed to configure ArtNet controller")
        return
    
    # Start the controller
    if not artnet.start():
        print("Failed to start ArtNet controller")
        return
    
    print("ArtNet controller started")

func _process(_delta):
    # Create DMX data (512 channels)
    var dmx_data := PackedByteArray()
    dmx_data.resize(512)
    
    # Set DMX channel values (0-255)
    dmx_data[0] = 255  # Channel 1 at full intensity
    dmx_data[1] = 128  # Channel 2 at half intensity
    # ... set other channels as needed
    
    # Set and send DMX data
    if artnet.set_dmx_data(0, dmx_data):
        artnet.send_dmx()

func _exit_tree():
    if artnet:
        artnet.stop()
```

### API Reference

#### ArtNetController

The main class for ArtNet operations.

##### Methods

- **`configure(bind_address: String, port: int, net: int, subnet: int, universe: int, broadcast_address: String = "255.255.255.255") -> bool`**
  
  Configures the ArtNet controller with network and universe settings.
  
  - `bind_address`: Local IP address to bind to (use "0.0.0.0" to bind to all interfaces)
  - `port`: UDP port to use (default Art-Net port is 6454)
  - `net`: Art-Net net value (0-127)
  - `subnet`: Art-Net subnet value (0-15)
  - `universe`: Art-Net universe value (0-15)
  - `broadcast_address`: Broadcast address to send packets to (default: "255.255.255.255")
  
  Returns `true` if configuration was successful.

- **`start() -> bool`**
  
  Starts the ArtNet controller and begins network operations.
  
  Returns `true` if started successfully.

- **`stop() -> void`**
  
  Stops the ArtNet controller and stops all network operations.

- **`is_running() -> bool`**
  
  Returns `true` if the controller is currently running.

- **`set_dmx_data(universe: int, data: PackedByteArray) -> bool`**
  
  Sets the DMX data for a specific universe. The data array should contain up to 512 channel values (0-255).
  
  - `universe`: The universe number to set data for
  - `data`: A PackedByteArray containing DMX channel values (0-255)
  
  Returns `true` if the data was set successfully.

- **`send_dmx() -> bool`**
  
  Sends the configured DMX data as an ArtNet packet.
  
  Returns `true` if the packet was sent successfully.

## Art-Net Protocol

Art-Net is a protocol for transmitting DMX512 data over Ethernet networks. It's commonly used in professional lighting control systems.

### Universe Addressing

Art-Net uses a three-level addressing scheme:
- **Net**: 0-127 (high-level network)
- **Subnet**: 0-15 (sub-network within a net)
- **Universe**: 0-15 (universe within a subnet)

The total number of addressable universes is: Net × 16 × 16 = up to 32,768 universes.

### DMX Channels

Each universe supports up to 512 DMX channels, with values ranging from 0-255.

## Demo Project

A demo project is included in the `demo/` directory. Open it in Godot to see a working example of ArtNet transmission.

## Building for Different Platforms

The extension supports building for:
- macOS (Intel and Apple Silicon)
- Linux (x86_64, ARM64, RISC-V)
- Windows (x86_32, x86_64)
- Android (x86_64, ARM64)
- iOS (ARM64)
- Web (WASM32)

Build artifacts are placed in `bin/<platform>/` and automatically copied to `demo/bin/<platform>/` when using SCons.

## Troubleshooting

### Controller fails to start

- Ensure your network interface is properly configured
- Check that the specified port (default 6454) is not in use
- Verify firewall settings allow UDP traffic on the ArtNet port

### DMX data not received

- Verify the broadcast address matches your network configuration
- Check that Art-Net net, subnet, and universe values are correct
- Ensure the receiving device is on the same network segment

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

This project is licensed under the same license as the template it's based on. The lib-artnet-4-cpp library is licensed under GPL-3.0.

## Acknowledgments

- [lib-artnet-4-cpp](https://github.com/gastonmorixe/lib-artnet-4-cpp) by Gaston Morixe
- [godot-cpp](https://github.com/godotengine/godot-cpp) by the Godot Engine team
- Based on the official Art-Net 4 Protocol Specification
