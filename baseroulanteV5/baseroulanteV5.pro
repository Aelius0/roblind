######################################################################
# Automatically generated by qmake (2.01a) ven. avr. 8 02:40:32 2016
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += Adafruit_ADS1015.h \
           analogInput.h \
           AX13.h \
           baseroulante.h \
           canne.h \
           capteurForce.h \
           CcapteurIR.h \
           joystick.h \
           kangaroo.h \
           lidarLite.h \
           relay.h \
           roblind.h \
           serialport.h \
           vibreur.h \
    capteurpresence.h \
    buzzer.h
SOURCES += Adafruit_ADS1015.cpp \
           analogInput.cpp \
           AX13.cpp \
           baseroulante.cpp \
           canne.cpp \
           capteurForce.cpp \
           CcapteurIR.cpp \
           joystick.cpp \
           kangaroo.cpp \
           lidarLite.cpp \
           main.cpp \
           relay.cpp \
           roblind.cpp \
           serialport.cpp \
           vibreur.cpp \
    capteurpresence.cpp \
    buzzer.cpp

LIBS +=  -L/usr/local/include -lwiringPi
