# -*- coding: utf-8 -*-

"""
	Turret.py
	
	Class to control the webcam turret
	
"""

__author__ = 'David Estávez Fernández'
__license__ = 'GPLv3'

# Imports
# ------------------------------------------------------------------------------
import serial
import sys

class Turret:
	def __init__(self):
		self.serialPort = None
		self.num_servos = 2
		
	def setupSerialPort(self, name, baudRate):
		"""
			Creates a serial connection with the specified parameters
		"""	

		# Create a serial object
		self.serialPort = serial.Serial()
	
		# Set the main parameters
		self.serialPort.port = name
		self.serialPort.baudrate = baudRate
	
		# Open port
		self.serialPort.open()
		
		# Feedback
		if self.serialPort.isOpen():
			print '[+] Connected successfully to ' + name + ' at ' + str(baudRate) + ' bauds'
		else: 
			print '[+] Error connecting to ' + name + ' at ' + str(baudRate) + ' bauds'



	def readPort(self):
		"""
			Reads a string from the serial port
		"""
		
		string = ''
		
		while self.serialPort.inWaiting() > 0:
			string += self.serialPort.read(1)

		return string


	def reset(self):	
		"""
			Resets the robot
		"""		
		self.serialPort.setDTR(False)
		time.sleep(0.5)
		self.serialPort.setDTR(True)
	

	def start(self, portName, baudRate):
		"""
			Starts the system
		"""
		self.setupSerialPort(portName, baudRate)

		print '[+] Waiting for the robot...'
		self.reset()
	
		# Wait for the robot to be ready
		read = False
		while not read or read == ' ' or read == '':
			read = self.readPort()
		if read == '[Debug] Ok!':
			read = True 
		time.sleep(0.05)
		
	def sendPosAll( self, pos):
		# Compose the command
		command = chr( 0x50 ) 
		command += ''.join( [chr(int(pos)) for i in range(self.num_servos)])
		
		# Send the command
		try:		
			self.serialPort.write( command )
 		except AttributeError, e:
			print 'Not connected: [' + str(e) + ']'	
			
	def sendPosSingle( self, joint, pos):
		# Compose the command
		command = chr(0x51)
		command += chr( joint) + chr( int(pos))
		
		# Send the command
		try:		
			self.serialPort.write( command )
 		except AttributeError, e:
			print 'Not connected: [' + str(e) + ']'	
	
	def sendToggleLED( self ):
		# Send the command
		try:		
			self.serialPort.write( chr(0x5F) )
 		except AttributeError, e:
			print 'Not connected: [' + str(e) + ']'	
			
	def __del__(self):
		# Close the serial port
		self.serialPort.close()


	
