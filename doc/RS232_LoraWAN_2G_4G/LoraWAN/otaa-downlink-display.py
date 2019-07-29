#!/usr/bin/python
# -*- encoding: utf-8 -*-
import os
import time
import serial
import pygame

# Serial port

p = serial.Serial("/dev/ttyAMA0" , 57600 , timeout=5)

# Uplink message to send

up="aa"

# Write to the RN2483 and if downlink is received, return it

def send(data):
   p.write(data+"\x0d\x0a")
   data.rstrip()
   print(data)
   time.sleep(2)

   down = None

   while True:
      rdata=p.readline()
      if rdata[:6] == "mac_rx":
         down=rdata[9:-2]
      else:
         print rdata[:-1]
      if rdata == "":
         break

   return down

# Pygame class for writing to the framebuffer

class fb:
   screen = None;
    
   def __init__(self):
      "Ininitializes a new pygame screen using the framebuffer"
      # Based on "Python GUI in Linux frame buffer"
      # http://www.karoltomala.com/blog/?p=679
      disp_no = os.getenv("DISPLAY")
      if disp_no:
         print "I'm running under X display = {0}".format(disp_no)
        
      # Check which frame buffer drivers are available
      # Start with fbcon since directfb hangs with composite output
      drivers = ['fbcon', 'directfb', 'svgalib']
      found = False
      for driver in drivers:
         # Make sure that SDL_VIDEODRIVER is set
         if not os.getenv('SDL_VIDEODRIVER'):
             os.putenv('SDL_VIDEODRIVER', driver)
         try:
             pygame.display.init()
         except pygame.error:
             print 'Driver: {0} failed.'.format(driver)
             continue
         found = True
         break
    
      if not found:
         raise Exception('No suitable video driver found!')
       
      size = (pygame.display.Info().current_w, pygame.display.Info().current_h)
      print "Framebuffer size: %d x %d" % (size[0], size[1])
      self.screen = pygame.display.set_mode(size, pygame.FULLSCREEN)
      # Clear the screen to start
      self.screen.fill((0, 0, 0))        
      # Initialise font support
      pygame.font.init()
      # Render the screen
      pygame.display.update()
 
   def __del__(self):
      "Destructor to make sure pygame shuts down, etc."
 
   def test(self):
      # Fill the screen with red (255, 0, 0)
      red = (255, 0, 0)
      self.screen.fill(red)
      # Update the display
      pygame.display.update()

   def text(self, string):
      # Get a font and use it render some text on a Surface.
      font = pygame.font.Font(None, 450)
      # Clear the screen
      self.screen.fill((0, 0, 0))
      # Render text 
      text_surface = font.render(string, True, (255, 255, 255))
      # Blit the text
      self.screen.blit(text_surface, (300, 300))
      # Update the display
      pygame.display.update()

# Create an instance of the fb class
fb = fb()

# Write some text as a test
fb.text('HELLO')

# Reset the RN2483 module

send("sys reset")
time.sleep(1)

# Configure the network parameters

send("mac set appeui AAAAAAAAAAAAAAAA")
send("mac set appkey KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK")
send("mac set adr off")
send("mac save")
time.sleep(5)

# Do an OTAA join

send("mac join otaa")

time.sleep(5)

# Send uplink and if we get downlink, update the display

while True:
   down = send("mac tx uncnf 1 "+up)
   if down:
      print "DOWNLINK: "+down
      fb.text(down) 
   time.sleep(10)
