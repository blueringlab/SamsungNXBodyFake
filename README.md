# Faking Samsung NX Body with Arduino Nano

Below image shows how to wire Arduino Nano and NX Body. 

![alt tag](https://github.com/blueringlab/SamsungNXBodyFake/blob/master/NXBodyFakeWire.png)

Once Arduino starts, it will perform 1 commands (0x03 0xAA 0x55) and response back (0x03 0xAA 0x55) to NX camera body (tested with NX20 camera).

Since Arduino Nano logic level (includes SPI) is 5V and NX body uses 3.3V, SPI logic should be converted between +5V and +3.3V. 

In order to convert +5V <--> +3.3V SPI logic level convert, I used Pololu 4 channel bidirectional logic level converter. (https://www.pololu.com/product/2595)
