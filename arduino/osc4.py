# Import needed modules from osc4py3
from osc4py3.as_eventloop import *
from osc4py3 import oscmethod as osm

ip = "0.0.0.0"
port = 7001

# Start the system.
osc_startup()

# Make server channels to receive packets.
osc_udp_server(ip, port, "server")

# Associate Python functions with message address patterns, using default
osc_method("/*", print)

# Periodically call osc4py3 processing method in your event loop.

try :
    while True:
        osc_process()

except KeyboardInterrupt :
    # Properly close the system.
    osc_terminate()