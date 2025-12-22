# Classic Mini fuel gauge
A tool to diagnose a classic mini fuel tank sender.

Using an ESP32 and a TFT screen to show some data.

The fuel sender is connected via a ADC pin on the ESP32

![screen](/docs/screen1.jpg "screen example")

## Components
| Name |
|:----  |
| ESP32-Wroom-32 | 
| 2.2" TFT SPI (240*320) | 

## ESP32 pinout

| PIN | Screen |
|:----  |:-------|
| 5V | 5V |  
| GND | GND | 
| D2 | DC (Screen)  |
| D4 | RESET (Screen) |
| D5 | CS (Screen) |
| D18 | SCK (Screen)  |
| D19 | MISO (Screen)  |
| D23 | MOSI (Screen)  |
| D34 | (ADC) Fuel tank sender input | 

## Sender voltage divider

To get a reading from the sender we need to add a voltage to it.
Then use a voltage divider and connect it to a ADC (Analog-to-digital converter).

With some math we can calculate the percentage to the sender

```text
3.3 V ── 220 Ω ──┬── ADC (ESP)
                 |
               Rsender
                 |
                GND
```