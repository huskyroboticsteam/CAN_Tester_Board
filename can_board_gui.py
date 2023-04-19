import sys, serial, re
from serial.tools import list_ports

import pygame
pygame.init()

from guipy.manager import GUIManager
from guipy.components import *
from guipy.utils import *

med_font = pygame.font.SysFont("comicsansms", 20)
small_font = pygame.font.SysFont("comicsansms", 12)

w, h = pygame.display.get_desktop_sizes()[0]

if (w > h):
    winH = h/2
    winW = winH*16/9
else:
    winW = w/2
    winH = winW*9/16

root = pygame.display.set_mode((winW, winH))

ser = None

class Text(Component):
    def __init__(self, text, font):
        self.root = font.render(text, True, BLACK)
        self.width = self.root.get_width()
        self.height = self.root.get_height()
    
def connect(menu):
    global ser
    if ser: ser.close()
    dev = devices[menu.value]
    ser = serial.Serial(dev, 115200, timeout=1)
    print(f'Connected on {dev}')

device_menu = Dropdown(500, med_font).set_callback(connect)
devices = {}
def refresh_devices(_):
    device_menu.options = []
    devs = list_ports.comports()
    for dev in devs:
        devices[dev.description] = dev.device
        device_menu.add(dev.description)

def send(text):
    if ser:
        data = (text+'\r\n').encode()
        ser.write(data)

man = GUIManager()

try:
    christian = pygame.image.load('Christian.PNG')
    christian = pygame.transform.rotate(christian,90)
except FileNotFoundError:
    christian = pygame.Surface((0,0))


def match(pattern):
    pattern = re.compile(pattern)
    def func(textbox):
        if not pattern.match(textbox.text):
            textbox.text = ''
    return func
            

class PacketField:
    def __init__(self):
        self.pri_switch = Switch(30,20)
        self.dg_text = Textbox(width=25, font=med_font, default_text='F')
        self.sn_text = Textbox(width=35, font=med_font, default_text='3F')
        self.data_text = Textbox(width=250, font=med_font, default_text='FF FF FF FF FF FF FF FF')
        self.send_butt = Button(text='Send').set_callback(self.send)

    def send(self, _):
        packet = f'{int(self.pri_switch.state)} {self.dg_text.text} {self.sn_text.text} {self.data_text.text}'
        send(packet)

man.add(Text('Priority',small_font),(35,80))
man.add(Text('DG',small_font),(90,80))
man.add(Text('SN',small_font),(130,80))
man.add(Text('Data',small_font),(180,80))

fields = []
for i in range(10):
    y = 100 + i*40
    x = 10

    field = PacketField()
    fields.append(field)

    man.add(Text(f'{i}:',med_font),(10,y))
    man.add(field.pri_switch, (x+30,y))
    man.add(field.dg_text, (x+80,y))
    man.add(field.sn_text, (x+120,y))
    man.add(field.data_text, (x+170,y))
    man.add(field.send_butt, (x+430,y))

refresh_button = Button(text='Refresh',font=med_font).set_callback(refresh_devices)

man.add(device_menu, (10, 10))
man.add(refresh_button, (520, 10))

last_rx = ''
repeat_count = 0

refresh_devices(0)
while True:
    events = pygame.event.get()
    for event in events:
        if event.type == pygame.QUIT:
            sys.exit()

    root.fill((200, 200, 200))

    root.blit(christian,(winW-140,10))
    if ser:
        if ser.in_waiting:
            try:
                s = ser.readline().decode('utf-8').strip()
                if s[:9] == last_rx[:9]:
                    repeat_count += 1
                    print('\r'+s, end = f' ({repeat_count})')
                else:
                    repeat_count = 0
                    print(f'\n{s}', end='')
                last_rx = s
            except UnicodeDecodeError:
                pass
    man.update(pygame.mouse.get_pos(), events, root)

    pygame.display.update()