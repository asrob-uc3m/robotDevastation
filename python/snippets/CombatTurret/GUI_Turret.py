# -*- coding: utf-8 -*-

"""
	Graphic User Interface for CombatTurret
	
"""
__author__ = 'David Estévez Fernández'
__license__ = 'GPLv3'

import wx
import glob
import time
import Turret

class MainWin( wx.Frame):

	def __init__(self, parent ):

		# Create a turret object
		self.turret = Turret.Turret();
		
		# Construct the window
		super( MainWin, self).__init__( parent, title='TurretControl', size = ( 300, 100), style=wx.DEFAULT_FRAME_STYLE)

		self.InitUI()
		self.Centre()
		self.Show()


	def InitUI(self):
		self.panel = wx.Panel(self)

		font = wx.SystemSettings_GetFont(wx.SYS_SYSTEM_FONT)
		font.SetPointSize(9)
	
		# Controls
		self.textX = wx.StaticText( self.panel, label = 'X Axis: ', pos=( 10, 12), size=(50, 20))
		self.sliderX = wx.Slider( self.panel , value=90, minValue=0, maxValue=180, 
								  pos=(50, 5), size=(230, 30), style=wx.SL_HORIZONTAL) 
		self.sliderX.Bind( wx.EVT_SCROLL, self.OnSliderScrollX)	
		
		self.textY = wx.StaticText( self.panel, label = 'Y Axis: ', pos=( 10, 42), size=(50, 20))
		self.sliderY = wx.Slider( self.panel , value=90, minValue=0, maxValue=180, 
								  pos=(50, 35), size=(230, 30), style=wx.SL_HORIZONTAL) 							  
		self.sliderY.Bind( wx.EVT_SCROLL, self.OnSliderScrollY)	
		
		self.connectButton = wx.Button( self.panel, label='Connect', pos = ( 10, 70 ), size = (100, 25) )
		self.connectButton.Bind( wx.EVT_BUTTON, self.OnConnectPressed)

		
	def OnConnectPressed( self, e):
		self.turret.start( '/dev/ttyUSB0', 57600)
		
	def OnSliderScrollX( self, e):
		posX = self.sliderX.GetValue()
		self.turret.sendPosSingle(  0, posX)
		
	def OnSliderScrollY( self, e):
		posY = self.sliderY.GetValue()
		self.turret.sendPosSingle(  1, posY)	
		
if __name__ == '__main__':
	app = wx.App()
	MainWin(None)
	app.MainLoop()
