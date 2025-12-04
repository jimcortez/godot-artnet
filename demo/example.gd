extends Node

# Example ArtNet usage
func _ready() -> void:
	var artnet := ArtNetController.new()
	
	# Configure ArtNet controller
	# Parameters: bind_address, port, net, subnet, universe, broadcast_address
	if not artnet.configure("0.0.0.0", 6454, 0, 0, 0, "255.255.255.255"):
		print("Failed to configure ArtNet controller")
		return
	
	# Start the controller
	if not artnet.start():
		print("Failed to start ArtNet controller")
		return
	
	print("ArtNet controller started successfully")
	
	# Create DMX data (512 channels)
	var dmx_data := PackedByteArray()
	dmx_data.resize(512)
	
	# Set some example DMX values
	for i in range(dmx_data.size()):
		dmx_data[i] = i % 256
	
	# Set DMX data for universe 0
	if artnet.set_dmx_data(0, dmx_data):
		print("DMX data set successfully")
		
		# Send the DMX data
		if artnet.send_dmx():
			print("DMX data sent successfully")
		else:
			print("Failed to send DMX data")
	else:
		print("Failed to set DMX data")
	
	# Clean up
	artnet.stop()
