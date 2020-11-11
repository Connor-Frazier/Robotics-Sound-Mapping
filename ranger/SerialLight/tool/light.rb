#!/usr/bin/env -S ruby -W0

require 'serialport'

vv = ARGV[0]

port = SerialPort.new("/dev/ttyUSB0", 9600)
port.write(vv)
puts port.readline()
