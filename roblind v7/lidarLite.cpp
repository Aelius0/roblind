/*
LidarLite - Wiring based interface to LidarLite for RaspberryPi and ODROID C1
Copyright (c) 2015, Answer 17
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANSWER 17 BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cmath>
#include "lidarLite.h"
    
lidarLite::lidarLite(Grille* grille, positionBaseRoulante* posBR, bool isLeft, float hauteur){
    m_addr=0x62;
    m_measurVal=0x04;
    m_measurReg=0x00;
    m_statusReg=0x47;
    m_distanceRegLo=0x0f;
    m_distanceRegHi=0x10;
    m_versionReg=0x41;
    m_errorRead=-1;
    m_statBusy=0x01;
    m_statRefOver=0x02;
    m_statSigOver=0x04;
    m_statPin=0x08;
    m_statSecondPeak=0x10;
    m_statTime=0x20;
    m_statInvalid=0x40;
    m_statEye=0x80;

    m_isLeft = isLeft;
    m_hauteurLidar = hauteur;
    m_Grille = grille;
    m_posBR = posBR;
}


   
// Initialize wiring I2C interface to LidarLite
int lidarLite::lidar_init(bool dbg) {
    int fd;
    m_dbg=dbg;
    if (m_dbg) printf("LidarLite V0.1\n\n");
    fd = wiringPiI2CSetup(m_addr);
    if (fd != -1) {
        lidar_status(fd);  // Dummy request to wake up device
        usleep (100000);
        }
    return(fd);
    }

// Read distance in cm from LidarLite
int lidarLite::lidar_read(int fd) {
   int hiVal, loVal; //i=0;

   // send "measure" command
   hiVal= wiringPiI2CWriteReg8(fd,m_measurReg,m_measurVal);
   if (m_dbg) printf("write res=%d\n", hiVal);
   usleep(20000);

   // Read second byte and append with first
   loVal = read_byteNZ(fd, m_distanceRegLo) ;
   if (m_dbg) printf(" Lo=%d\n", loVal);

   // read first byte
   hiVal = read_byte(fd, m_distanceRegHi) ;
   if (m_dbg) printf ("Hi=%d ", hiVal);

   return ( (hiVal << 8) + loVal);
}

unsigned char lidarLite::lidar_version(int fd) {
    return( (unsigned char) read_byteNZ(fd, m_versionReg) );
    }

unsigned char lidarLite::lidar_status(int fd) {
    return( (unsigned char) wiringPiI2CReadReg8(fd, m_statusReg) );
    }

void lidarLite::lidar_status_print(unsigned char status) {
if (status != 0)
printf("STATUS BYTE: 0x%x ", (unsigned int) status);

if (status & m_statBusy) printf("busy \n");
if (status & m_statRefOver) printf("reference overflow \n");
if (status & m_statSigOver) printf("signal overflow \n");
if (status & m_statPin) printf("mode select pin \n");
if (status & m_statSecondPeak) printf("second peak \n");
if (status & m_statTime) printf("active between pairs \n");
if (status & m_statInvalid) printf("no signal \n");
if (status & m_statEye) printf(" eye safety \n");

//if (status & 0b100000) printf("HEALTY");
}

// Read a byte from I2C register.  Repeat if not ready
unsigned char  lidarLite::read_byte(int fd, int reg)
{
return lidarLite::read_byte_raw(fd, reg, true);
}

// Read Lo byte from I2C register.  Repeat if not ready or zero
unsigned char  lidarLite::read_byteNZ(int fd, int reg) {
return read_byte_raw(fd, reg, false);
}

// Read byte from I2C register.  Special handling for zero value
unsigned char  lidarLite::read_byte_raw(int fd, int reg, bool allowZero) {
    int i;
    unsigned char val;
    usleep(1000);
    while (true) {
        val = wiringPiI2CReadReg8(fd, reg);

        // Retry on error
        if (val == m_errorRead || (val==0 && !allowZero) ) {
            usleep (20000) ;		// ms
            // if (_dbg) printf(".");
            if (i++ > 50) {
                // Timeout
                printf("Timeout");
                return (m_errorRead);
            }
        }
        else return(val);
    }
}


void* lidarLite::runLidar(void* lidar) {
    lidarLite* lid = static_cast<lidarLite*>(lidar);
    int fd = wiringPiI2CSetup(lid->m_addr); // le bon fd ??
    while (true) {
        float resolution = lid->m_Grille->getResolution();
        int distance = lid->lidar_read(fd);
        float angle = 0; // getAngle() ?

        float realDist = distance * sin(angle);
        lid->lidar_status_print(lid->lidar_status(fd));
        std::cout << "VERSION : " << lid->lidar_version(fd) << std::endl;
        std::cout << "DISTANCE = " << distance << std::endl;
        float thDist = lid->m_hauteurLidar * tan(angle);
        if (abs(realDist - thDist) > MARGE_OBSTACLE) {
            lid->m_Grille->marquerObstacles(lid->m_posBR->x + resolution * cos(lid->m_posBR->alpha + (lid->m_isLeft ? M_PI/2 : -M_PI/2)),
                                            lid->m_posBR->y + resolution * sin(lid->m_posBR->alpha + (lid->m_isLeft ? M_PI/2 : -M_PI/2)));
        } else {
            lid->m_Grille->marquerLibre(lid->m_posBR->x + resolution * cos(lid->m_posBR->alpha + (lid->m_isLeft ? M_PI/2 : -M_PI/2)),
                                        lid->m_posBR->y + resolution * sin(lid->m_posBR->alpha + (lid->m_isLeft ? M_PI/2 : -M_PI/2)));
        }

        delay(5);
    }
    return 0;
}
