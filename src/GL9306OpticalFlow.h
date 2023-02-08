#include <Arduino.h>

#ifndef USE_SERIAL
#define USE_SERIAL Serial
#endif

#ifndef GL9306OpticalFlow_H
#define GL9306OpticalFlow_H

/**
 * @brief
 * 2023.2.1
 * updated: deprecated function Serial.setPins()
 * -HunterL
 * 2023.2.6
 * updated: more robust on Serial reading process
 * updated: check bit fully funtional
 * -HunterL
 */

class GL9306
{
public:
    int speed_x;
    int speed_y;
    unsigned char qual;

    GL9306(HardwareSerial *mySerial, int8_t rxPin = -1, int8_t txPin = -1) // txPin will be wasted for no data should sent to GL9306
    {
        optSerial = mySerial;
        optSerial->begin(19200, SERIAL_8N1, rxPin, txPin);
        USE_SERIAL.print("GL9306 begin");
    }

    bool available()
    {
        if (optSerial->available() >= messageLength)
            if (0xfe == optSerial->read()) // 校验头
                if (readUart())
                    if (decode())
                        return true;
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
    HardwareSerial *optSerial;
    const static uint8_t messageLength = 7;
    byte UartRxOpticalFlow[messageLength];
    bool decode()
    {
        uint8_t Check_sum = 0;
        static int16_t flow_x, flow_y;
        if (UartRxOpticalFlow[0] == 0x04)
        {
            Check_sum = (uint8_t)(UartRxOpticalFlow[1] + UartRxOpticalFlow[2] + UartRxOpticalFlow[3] + UartRxOpticalFlow[4]);
            if (Check_sum == UartRxOpticalFlow[5]) // 校验和正确
            {
                flow_y = UartRxOpticalFlow[1] + (UartRxOpticalFlow[2] << 8);
                flow_x = UartRxOpticalFlow[3] + (UartRxOpticalFlow[4] << 8);
                qual = UartRxOpticalFlow[6];
                speed_x = flow_x;
                speed_y = flow_y;
                return true;
            }
        }
        USE_SERIAL.println("Opt decode failure");
        return false;
    }
    bool readUart()
    {
        uint8_t i = 0;
        while (optSerial->available() <= messageLength)
            if (i++ > 50)
            {
                USE_SERIAL.println("Opt read failure");
                return false;
            }
        for (uint8_t n = 0; n < messageLength; n++)
        {
            UartRxOpticalFlow[n] = optSerial->read();
        }
        return true;
    }
};
#endif
