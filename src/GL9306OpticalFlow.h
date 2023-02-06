#include <Arduino.h>

#ifndef USE_SERIAL
#define USE_SERIAL Serial
#endif

#ifndef GL9306OpticalFlow_H
#define GL9306OpticalFlow_H
class GL9306
{
public:
    int speed_x;
    int speed_y;
    unsigned char qual;

    GL9306(HardwareSerial *mySerial, int8_t rxPin = -1, int8_t txPin = -1)
    {
        optSerial = mySerial;
        optSerial->begin(19200, SERIAL_8N1, rxPin, txPin);
        USE_SERIAL.print("GL9306 begin");
    }
//     function setPins is deprecated
    
//     GL9306(HardwareSerial *mySerial, int8_t rxPin, int8_t txPin) // txPin will be wasted for no data should sent to GL9306
//     {
//         optSerial = mySerial;
//         optSerial->begin(19200);
//         if (rxPin != 0 && txPin != 0)
//         {
//             optSerial->setPins(rxPin, txPin);
//             USE_SERIAL.print("GL9306 begin");
//         }
//         else
//         {
//             USE_SERIAL.print("GL9306OpticalFlow negetive Uart Pin Number");
//             while (true)
//                 ;
//         }
//     }

    bool available()
    {
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
                flow_y = UartRxOpticalFlow[2] + (UartRxOpticalFlow[3] << 8);
                flow_x = UartRxOpticalFlow[4] + (UartRxOpticalFlow[5] << 8);
                qual = UartRxOpticalFlow[7];
                speed_x = flow_x;
                speed_y = flow_y;
                return true;
            }
        }
        Serial.println("decode failure");
        return false;
    }
    bool readUart()
    {
        int j = optSerial->available();
        if (j != 0)
        {
            for (int n = 0; n < j; n++)
            {
                UartRxOpticalFlow[n] = optSerial->read();
            }
            return true;
        }
        return false;
    }
};
#endif
