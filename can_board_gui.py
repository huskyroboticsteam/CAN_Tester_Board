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
pygame.display.set_caption('epic CAN board')
ser = None
filter_id = None

class Text(Component):
    def __init__(self, text, font):
        self.root = font.render(text, True, BLACK)
        self.width = self.root.get_width()
        self.height = self.root.get_height()
    
def set_filter(filter):
    global filter_id
    filter_id = filter.id

def connect(menu):
    global ser
    if ser: ser.close()
    dev = devices[menu.value]
    try:
        ser = serial.Serial(dev, 115200, timeout=1)
        print(f'Connected on {dev}')
    except serial.serialutil.SerialException:
        menu.value = ''
        print(f'Failed to connect to {dev}')
        
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
    amelia = pygame.image.load('amelia.PNG')
    amelia = pygame.transform.rotate(amelia,90)
except FileNotFoundError:
    amelia = pygame.Surface((0,0))            

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

filters = [
    ((0,0) ,(530,100), 'broadcast'),
    ((2,1) ,(530,125), 'jetson'),
    ((3,1) ,(530,200), 'battery management'),
    ((3,2) ,(530,225), 'chassis power distro'),
    ((3,3) ,(530,250), 'chassis drive PDB left'),
    ((3,4) ,(530,275), 'chassis drive PDB right'),
    ((3,5) ,(530,300), 'power arm lower 1'),
    ((3,6) ,(530,325), 'power aem lower 2'),
    ((3,7) ,(530,350), 'power science'),
    ((7,1) ,(530,425), 'science station'),
    ((7,2) ,(530,450), 'drill arm motor'),
    ((7,3) ,(530,475), 'drill motor'),
    ((4,1) ,(650,100), 'base rotation'),
    ((4,2) ,(650,125), 'shoulder'),
    ((4,3) ,(650,150), 'elbow'),
    ((4,4) ,(650,175), 'forearm'),
    ((4,5) ,(650,200), 'wrist diff left'),
    ((4,6) ,(650,225), 'hand'),
    ((4,7) ,(650,250), 'linear actuator'),
    ((4,8) ,(650,275), 'chassis front left'),
    ((4,24),(650,300), 'chassis front left swivel'),
    ((4,9) ,(650,325), 'chassis front right'),
    ((4,25),(650,350), 'chassis front right swivel'),
    ((4,10),(650,375), 'chassis back left'),
    ((4,26),(650,400), 'chassis back left swivel'),
    ((4,11),(650,425), 'chassis back right'),
    ((4,27),(650,450), 'chassis back right swivel'),
    ((4,12),(650,475), 'wrist diff right')
]

for filter in filters:
    def f(button):
        global filter_id
        filter_id = button.filter[0]
        print(f'\r\nShowing packets for {button.filter[2]}')
    b = Button(text=f'DG {hex(filter[0][0])} SN {hex(filter[0][1])}', font=small_font).set_callback(f)
    b.filter = filter
    man.add(b, filter[1])

id_to_filter = dict()
for filter in filters:
    id_to_filter[filter[0]] = filter

refresh_button = Button(text='Refresh',font=med_font).set_callback(refresh_devices)

man.add(device_menu, (10, 10))
man.add(refresh_button, (520, 10))

packet_pattern = r'([0-1]) ([0-9a-fA-F]{2}) ([0-9a-fA-F]{2})'

refresh_devices(0)
clock = pygame.time.Clock()

# Main loop
while True:
    events = pygame.event.get()
    for event in events:
        if event.type == pygame.QUIT:
            sys.exit()

    root.fill((200, 200, 200))
    root.blit(amelia,(winW-200,10))

    if ser:
        while ser.in_waiting:
            line_raw = ser.readline()
            try:
                line = line_raw.decode('utf-8').replace('\n', '')
            except UnicodeDecodeError as err:
                print(err)
                continue

            match = re.match(packet_pattern, line)

            if match:
                pri,dg,sn = match.groups()
                id = int(dg,16), int(sn,16)
            else:
                print(line)
                continue
    
            if filter_id == None or id == filter_id:
                print(line)

            if id in id_to_filter.keys():
                pos = id_to_filter[id][1]
                pygame.draw.circle(root, BLUE, pos, 4)

    man.update(pygame.mouse.get_pos(), events, root)

    pygame.display.update()
    clock.tick(30)