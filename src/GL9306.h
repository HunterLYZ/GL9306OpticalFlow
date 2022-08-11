#ifndef Arduino_h
#include <Arduino.h>
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
    int integral_x = 0;
    int integral_y = 0;

    void begin(int8_t rxPin = 0, int8_t txPin = 0)//txPin will be wasted for no data should sent to GL9306 
    {
        GL9306_SERIAL.begin(19200);
        if (rxPin != 0 && txPin != 0)
            GL9306_SERIAL.setPins(rxPin, txPin);
    }
    bool update()
    {
        if (readUart())
            if (decode())
            {
                integral();
                frameCount++;
                return true;
            }
        return false;
    }
    void printValue()
    {
        USE_SERIAL.printf("frameCount=%4d, ", frameCount);
        USE_SERIAL.printf("speed_x=%4d, ", speed_x);
        USE_SERIAL.printf("speed_y=%4d; ", speed_y);
        USE_SERIAL.printf("integral_x=%4d, ", integral_x);
        USE_SERIAL.printf("integral_y=%4d", integral_y);
        USE_SERIAL.println("");
    }

private:
    byte UartRxOpticalFlow[9];
    unsigned short frameCount = 0;
    signed short gyro_z_rate_integral;
    unsigned char qual;
    unsigned char sum;

    bool decode()//return false if
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
    void integral()
    {
        integral_x += speed_x;
        integral_y += speed_y;
    }
} optFlow;