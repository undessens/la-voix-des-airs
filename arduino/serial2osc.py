import io
import subprocess
import codecs
import time

import serial
import serial.tools.list_ports as list_ports

from pythonosc.udp_client import SimpleUDPClient

DEBUG = False

def list_devices():
    ports = list_ports.comports()
    for port in ports:
        print(port.device)

def read_serial(serial, decoder):
    "Receive characters encoded in UTF-8 from serial device"
    ser_bytes = serial.read(1)  # <class 'bytes'>
    if len(ser_bytes) == 0 :
        return None

    if DEBUG:
        print("DEBUG:", ser_bytes)
    
    decoded_str = decoder.decode(ser_bytes)
    return decoded_str

def send_osc(client, msg, address="/debug"):
    "Send character ASCII number"
    ascii_id = ord(msg)
    if DEBUG :
        print("(",ascii_id,")", end="" , flush=True)        
    client.send_message(address, ascii_id )
    # time.sleep(1)

def main(ser_port, ser_brate, osc_dest, osc_port, encoding="utf-8", enc_err="replace", ):
    # Serial
    ser = serial.Serial(ser_port, ser_brate, timeout=0)
    ser.flushInput()
    rx_encoder = codecs.getincrementaldecoder(encoding)(enc_err)
    end_char = "\n" if DEBUG else ""

    # Osc
    osc_client = SimpleUDPClient(osc_dest, osc_port)  # Create client

    msg = ""
    while True:
        try:
            new_char = read_serial(ser, rx_encoder)
            if new_char:
                msg += new_char
                print(new_char, end=end_char , flush=True)        
                send_osc(osc_client, new_char)
            # if decoded_str == "\n" :
            #     subprocess.call(["say", msg, "-v", "Thomas"])
            #     msg = ""

        except KeyboardInterrupt:
            print("quitting...")
            break

        time.sleep(0.1)


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description='Read from a serial device and send it to OSC')
    parser.add_argument("--sdev", help='The name of the serial device to read data', dest="serial_device")
    parser.add_argument("--srate", help='The baud rate for serial device', dest="serial_brate", default=38400)
    parser.add_argument("--odest", help='The IP to send OSC information ', dest="osc_server", default="localhost")
    parser.add_argument("--oport", help='The IP to send OSC information ', dest="osc_port", default=8000)

    parser.add_argument("-l", "--slist", help='list serial devices', dest="print_devices", action="store_true")
    parser.add_argument('--debug', help='the name of the serial device', action="store_true")

    args = parser.parse_args()

    if args.print_devices:
        list_devices()
    
    if args.debug:
        DEBUG = True

    if args.serial_device:
        print("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
        print("Start Serial to OSC bridge with next configurationn\n")
        print("\tReading from\t\t", args.serial_device, "brate", args.serial_brate)
        print("\tSending OSC messages to\t", args.osc_server, "port", args.osc_port)
        print("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")        
        main(args.serial_device, args.serial_brate,args.osc_server, int(args.osc_port))