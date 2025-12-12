extends Node3D

# Light Demo - 4x4 grid of orbs with DMX output
const UNIVERSE = 1  # ArtNet universe to send data on
const GRID_SIZE = 4  # 4x4 grid = 16 orbs
const ORBS_COUNT = GRID_SIZE * GRID_SIZE  # 16 orbs
const CHANNELS_PER_ORB = 3  # RGB = 3 channels per orb
const TOTAL_DMX_CHANNELS = ORBS_COUNT * CHANNELS_PER_ORB  # 48 channels

# Network configuration
# See README.md for instructions on how to find these values for your system
const BIND_ADDRESS = "192.168.2.52"  # Your computer's IP address on the network
const BROADCAST_ADDRESS = "192.168.15.255"  # Your network's broadcast address

# Timing
const COLOR_CHANGE_INTERVAL = 2.0  # Change colors every 2 seconds
const DMX_SEND_INTERVAL = 0.01  # Send DMX every 10ms

var artnet: ArtNetController
var orbs: Array[MeshInstance3D] = []
var target_colors: Array[Color] = []
var current_colors: Array[Color] = []
var time_since_color_change: float = 0.0
var time_since_dmx_send: float = 0.0
var dmx_error_printed: bool = false

func _ready() -> void:
	# Initialize ArtNet controller
	artnet = ArtNetController.new()
	
	# Configure ArtNet controller
	if not artnet.configure(BIND_ADDRESS, 6454, 0, 0, UNIVERSE, BROADCAST_ADDRESS):
		print("Failed to configure ArtNet controller")
		return
	
	# Start the controller
	if not artnet.start():
		print("Failed to start ArtNet controller")
		return
	
	print("ArtNet controller started successfully")
	print("Sending DMX data for ", ORBS_COUNT, " orbs (", TOTAL_DMX_CHANNELS, " channels) on universe ", UNIVERSE)
	
	# Initialize color arrays
	target_colors.resize(ORBS_COUNT)
	current_colors.resize(ORBS_COUNT)
	
	# Set initial random colors
	for i in range(ORBS_COUNT):
		target_colors[i] = _random_color()
		current_colors[i] = target_colors[i]
	
	# Create orbs
	_create_orbs()

func _create_orbs() -> void:
	var orb_spacing = 2.0  # Distance between orbs
	var grid_offset = (GRID_SIZE - 1) * orb_spacing * 0.5
	
	# Create sphere mesh resource (shared)
	var sphere_mesh = SphereMesh.new()
	sphere_mesh.radius = 0.5
	sphere_mesh.height = 1.0
	sphere_mesh.radial_segments = 32
	sphere_mesh.rings = 16
	
	# Create plastic-like material with emission
	var material = StandardMaterial3D.new()
	material.emission_enabled = true
	material.emission_energy_multiplier = 2.0
	# Plastic properties: shiny, reflective, smooth
	material.metallic = 0.8  # High metallic for plastic look
	material.roughness = 0.2  # Low roughness for shininess
	material.clearcoat = 0.5  # Add clearcoat for extra plastic shine
	material.clearcoat_roughness = 0.1  # Smooth clearcoat
	
	for x in range(GRID_SIZE):
		for z in range(GRID_SIZE):
			var index = x * GRID_SIZE + z
			
			# Create orb mesh instance
			var orb = MeshInstance3D.new()
			orb.mesh = sphere_mesh
			orb.material_override = material.duplicate()
			orb.position = Vector3(
				x * orb_spacing - grid_offset,
				1.0,  # Height above floor
				z * orb_spacing - grid_offset
			)
			add_child(orb)
			orbs.append(orb)
			
			# Set initial color
			_update_orb_color(index, current_colors[index])

func _random_color() -> Color:
	return Color(
		randf(),
		randf(),
		randf(),
		1.0
	)

func _update_orb_color(index: int, color: Color) -> void:
	if index >= orbs.size():
		return
	
	var orb = orbs[index]
	if orb and orb.material_override:
		var material = orb.material_override as StandardMaterial3D
		if material:
			material.emission = color
			# Also set albedo for better visibility
			material.albedo_color = color

func _process(delta: float) -> void:
	if not artnet:
		return
	
	time_since_color_change += delta
	time_since_dmx_send += delta
	
	# Change target colors every 2 seconds
	if time_since_color_change >= COLOR_CHANGE_INTERVAL:
		time_since_color_change = 0.0
		for i in range(ORBS_COUNT):
			target_colors[i] = _random_color()
	
	# Smoothly interpolate colors towards target
	var lerp_speed = 2.0  # How fast colors transition
	for i in range(ORBS_COUNT):
		current_colors[i] = current_colors[i].lerp(target_colors[i], lerp_speed * delta)
		_update_orb_color(i, current_colors[i])
	
	# Send DMX data every 10ms
	if time_since_dmx_send >= DMX_SEND_INTERVAL:
		time_since_dmx_send = 0.0
		_send_dmx_data()

func _send_dmx_data() -> void:
	# Create DMX data array (full 512 channel universe)
	# Some ArtNet devices expect full universe, so we'll pad unused channels to 0
	var dmx_data: PackedByteArray
	dmx_data.resize(512)  # Full universe
	dmx_data.fill(0)  # Initialize all channels to 0
	
	# Map each orb's RGB color to DMX channels
	for i in range(ORBS_COUNT):
		var color = current_colors[i]
		var base_channel = i * CHANNELS_PER_ORB
		
		# Convert color values (0.0-1.0) to DMX values (0-255)
		# DMX channels are 1-indexed in the protocol, but array is 0-indexed
		# Channel 1 = Red, Channel 2 = Green, Channel 3 = Blue
		dmx_data[base_channel + 0] = int(color.r * 255)  # Red
		dmx_data[base_channel + 1] = int(color.g * 255)  # Green
		dmx_data[base_channel + 2] = int(color.b * 255)  # Blue
	
	# Set DMX data for the configured universe
	if artnet.set_dmx_data(UNIVERSE, dmx_data):
		# Send the DMX data
		if artnet.send_dmx():
			# Success - reset error flag if it was set
			dmx_error_printed = false
		else:
			# Only print error once to avoid spam
			if not dmx_error_printed:
				print("Warning: Failed to send DMX data (this is normal if no ArtNet hardware is connected)")
				dmx_error_printed = true
	else:
		if not dmx_error_printed:
			print("Warning: Failed to set DMX data")
			dmx_error_printed = true

func _exit_tree() -> void:
	if artnet:
		artnet.stop()
		artnet = null
