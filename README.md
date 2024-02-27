This project will help you replace MAX7219 and LEDs with a single OLED screen (SSD1306, SH1106). It emulates MAX7219 7dig LED controller.

I'm used Arduino Pro Mini, but you can use any you want

Connection:

Max7219 emulation:
DIN Mobiflight - D11 Arduino Pro Mini
CS Mobiflight - D2 Arduino Pro Mini
CLK Mobiflight - D13 Arduino Pro Mini

Leds:
Led0 Mobiflight - D4 Arduino Pro Mini
Led1 Mobiflight - D5 Arduino Pro Mini

OLED:
A4 - I2C SDA
A5 - I2C SCL

How it's looks like, Wokwi example:
https://wokwi.com/projects/390879110714803201
