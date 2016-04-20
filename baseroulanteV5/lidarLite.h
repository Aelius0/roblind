/*
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
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#ifndef LIDARLITE_H
#define LIDARLITE_H

#include <stdio.h>
#include <wiringPiI2C.h>
#include <stdbool.h>
#include <unistd.h>

class lidarLite
{
private:
    bool m_dbg;
    int m_addr;
    int m_measurVal;
    int m_measurReg;
    int m_statusReg;
    int m_distanceRegLo;
    int m_distanceRegHi;
    int m_versionReg;
    int m_errorRead;
    int m_statBusy;
    int m_statRefOver;
    int m_statSigOver;
    int m_statPin;
    int m_statSecondPeak;
    int m_statTime;
    int m_statInvalid;
    int m_statEye;

public:
    lidarLite();
    int lidar_init(bool);
    int lidar_read(int);
    unsigned char read_byte(int, int);
    unsigned char read_byteNZ(int, int);
    unsigned char read_byte_raw(int, int, bool);
    unsigned char lidar_version(int) ;
    unsigned char lidar_status(int);
    void lidar_status_print(unsigned char status);

};

#endif
