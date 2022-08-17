/*!
 * @file    GL9306.ino
 * @brief   Output the optFlow data
 * @license The MIT License (MIT)
 * @author  [HunterL](liuyunzhe2002@outlook.com)
 * @version V2.1
 * @date    2022-7-22
 * @url     https://github.com/HunterLYZ/GL9306OpticalFlow
 */

#ifndef GL9306_H
#define GL9306_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef USE_SERIAL
#define USE_SERIAL Serial
#endif

#ifndef GL9306_SERIAL
#define GL9306_SERIAL Serial1
#endif
class GL9306
{
public:
    int speed_x;
    int speed_y;
    unsigned char qual;

    GL9306(int8_t rxPin = 0, int8_t txPin = 0) // txPin will be wasted for no data should sent to GL9306
    {

        GL9306_SERIAL.begin(19200);
        if (rxPin != 0 && txPin != 0)
            GL9306_SERIAL.setPins(rxPin, txPin);
    }
    bool available()
    {
        if (readUart())
            if (decode())
            {
                return true;
            }
        return false;
    }
    void printValue()
    {
        USE_SERIAL.printf("speed_x=%4d, ", this->speed_x);
        USE_SERIAL.printf("speed_y=%4d; ", this->speed_y);
        USE_SERIAL.printf("quality=%4d; ", this->qual);
        USE_SERIAL.println("");
    }

private:
    byte UartRxOpticalFlow[9];
    bool decode()
    {
        uint8_t Check_sum = 0;
        static int16_t flow_x, flow_y;
        if (UartRxOpticalFlow[0] == 0xfe) //校验头
        {
            Check_sum = (uint8_t)(UartRxOpticalFlow[2] + UartRxOpticalFlow[3] + UartRxOpticalFlow[4] + UartRxOpticalFlow[5]);
            if (Check_sum == UartRxOpticalFlow[6]) //校验和正确
            {
                flow_x = UartRxOpticalFlow[2] + (UartRxOpticalFlow[3] << 8);
                flow_y = UartRxOpticalFlow[4] + (UartRxOpticalFlow[5] << 8);
                this->qual = UartRxOpticalFlow[7];
                this->speed_x = flow_x;
                this->speed_y = flow_y;
                return true;
            }
        }
        Serial.println("decode failure");
        return false;
    }
    bool readUart()
    {
        int j = Serial1.available();
        if (j != 0)
        {
            for (int n = 0; n < j; n++)
            {
                UartRxOpticalFlow[n] = GL9306_SERIAL.read();
            }
            return true;
        }
        return false;
    }
};

#endif
