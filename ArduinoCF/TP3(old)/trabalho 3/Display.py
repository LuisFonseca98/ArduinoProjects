import serial
import time
import pygame
import sys
import datetime
pygame.init()

screen = pygame.display.set_mode([800,800])

timer = pygame.time.Clock()
t0 = pygame.time.get_ticks()

Black =(0,0,0)
cor_fundo = (255,255,255)
fundo = pygame.image.load("fundo.jpg")

def Window():
    screen.fill(cor_fundo)
    screen.blit(fundo, [0,0])


com = 'COM3'
baudrate = 9600

def comInit(com, baudrate):
    try:
        Serie = serial.Serial(com, baudrate)
        print("Sucesso na ligacao ao Arduino")
        print("Ligado ao " + Serie.portstr)
        return Serie
    except Exception as e:
        print("Insucesso na ligacao ao Arduino")
        print (e)
        return None

def stringReceive(Serie):
    try:
        return Serie.readline().strip()
    except Exception as e:
        print("Erro na comunicacao (stringReceive)")
        print(e)
        Serie.close()


def temp(txt, size):
    Fonte = pygame.font.Font("times.ttf", size)
    Texto = Fonte.render(txt, True, Black)
    screen.blit(Texto, [7, 160])


def tempTexto(txt, size):
    Fonte = pygame.font.Font("times.ttf", size)
    Texto = Fonte.render(txt, True, Black)
    screen.blit(Texto, [110, 230])


def press(txt, size):
    Fonte = pygame.font.Font("times.ttf", size)
    Texto = Fonte.render(txt, True, Black)
    screen.blit(Texto, [10, 325])
    

def pressTexto(txt, size):
    Fonte = pygame.font.Font("times.ttf", size)
    Texto = Fonte.render(txt, True, Black)
    screen.blit(Texto, [110, 395])

def tick(txt,size):
    Fonte = pygame.font.Font("DS-DIGI.ttf", size)
    Texto = Fonte.render(txt, True, Black)
    screen.blit(Texto, [400, 400])
    

s = comInit(com, baudrate)
print(s)
a = [None]*5
imprimir_1_vez = True
imprimir_cada_30sec = True


Window()

while (s != None):
    now = datetime.datetime.now()
    if( now.hour < 10):
        b = ("0" + str(now.hour))
    else:
        b = str(now.hour)
    if( now.minute < 10):
        c = ("0" + str(now.minute))
    else:
        c = str(now.minute)
    if( now.second < 10):
        d = ("0" + str(now.second))
    else:
        d = str(now.second)
    tick( b + ":" + c + ":" + d,70)
        
    for indice in range(5):
        a[indice] = stringReceive(s)

    if (imprimir_1_vez == True):
        Window();
        temp(a[0], 55)
        print(a[0])
        tempTexto(a[1], 70)
        print(a[1])
        press(a[2] , 55)
        print(a[2])
        pressTexto(a[3], 70)
        print(a[3])
        imprimir_1_vez = False


    t1 = pygame.time.get_ticks()
    if(t1 - t0 > 30000):
        if (imprimir_cada_30sec == True):
            Window();
            temp(a[0], 55)
            print(a[0])
            tempTexto(a[1], 70)
            print(a[1])
            press(a[2] , 55)
            print(a[2])
            pressTexto(a[3], 70)
            print(a[3])
            a[4] = None
            imprimir_cada_30sec = False


        for c in a:
            if c == None:
                a = [None] * 5
                t0 = t1
                imprimir_cada_30sec = True




    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()


    pygame.display.update()
    timer.tick(60)
pygame.quit()
