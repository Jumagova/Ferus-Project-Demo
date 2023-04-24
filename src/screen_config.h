#define LGFX_USE_V1

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{

  lgfx::Panel_ST7796 _panel_instance;
  lgfx::Bus_Parallel8 _bus_instance; // 8-bit parallel bus instance (ESP32 only)
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_FT5x06 _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436

public:
  LGFX(void)
  {
    {                                    // Configure bus control settings.
      auto cfg = _bus_instance.config(); // Get the structure for bus configuration.
                                         // 8-bit parallel bus settings
      // cfg.i2s_port = I2S_NUM_0; // Select the I2S port to use (I2S_NUM_0 or I2S_NUM_1) (Use ESP32's I2S LCD mode)
      cfg.freq_write = 20000000;              // Transmit clock (maximum 20MHz, rounded to 80MHz divided by an integer)
      cfg.pin_wr = 47;                        // pin number connecting WR
      cfg.pin_rd = -1;                        // pin number connecting RD
      cfg.pin_rs = 0;                         // Pin number connecting RS(D/C)
      cfg.pin_d0 = 9;                         // pin number connecting D0
      cfg.pin_d1 = 46;                        // pin number connecting D1
      cfg.pin_d2 = 3;                         // pin number connecting D2
      cfg.pin_d3 = 8;                         // pin number connecting D3
      cfg.pin_d4 = 18;                        // pin number connecting D4
      cfg.pin_d5 = 17;                        // pin number connecting D5
      cfg.pin_d6 = 16;                        // pin number connecting D6
      cfg.pin_d7 = 15;                        // pin number connecting D7
      _bus_instance.config(cfg);              // Apply the settings to the bus.
      _panel_instance.setBus(&_bus_instance); // Sets the bus to the panel.
    }

    {                                      // Set display panel control.
      auto cfg = _panel_instance.config(); // Get the structure for display panel configuration.

      cfg.pin_cs = -1;   // Pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = 4;   // pin number where RST is connected (-1 = disable)
      cfg.pin_busy = -1; // pin number to which BUSY is connected (-1 = disable)

      // * The following setting values are general initial values for each panel, so please try commenting out any unknown items.

      cfg.panel_width = 320;    // actual displayable width
      cfg.panel_height = 480;   // actual displayable height
      cfg.offset_x = 0;         // Panel offset in X direction
      cfg.offset_y = 0;         // Panel offset in Y direction
      cfg.offset_rotation = 0;  // Rotation value offset 0~7 (4~7 are upside down)
      cfg.dummy_read_pixel = 8; // number of dummy read bits before pixel read
      cfg.dummy_read_bits = 1;  // number of dummy read bits before non-pixel data read
      cfg.readable = true;      // set to true if data can be read
      cfg.invert = true;        // Set to true if panel brightness is inverted
      cfg.rgb_order = false;    // set to true if red and blue on the panel are swapped
      cfg.dlen_16bit = false;   // Set to true for panels that transmit data length in 16-bit units with 16-bit parallel or SPI
      cfg.bus_shared = true;    // Set to true when the bus is shared with the SD card (bus control with drawJpgFile etc.)

      // Please set the following only when the display shifts with a driver with a variable number of pixels such as ST7735 or ILI9163.
      // cfg.memory_width = 240; // Maximum width supported by driver IC
      // cfg.memory_height = 320; // Maximum height supported by driver IC

      _panel_instance.config(cfg);
    }

    //*
    {                                      // Set backlight control. (delete if not necessary)
      auto cfg = _light_instance.config(); // Get the structure for backlight configuration.

      cfg.pin_bl = 45;     // pin number to which the backlight is connected
      cfg.invert = false;  // true to invert backlight brightness
      cfg.freq = 44100;    // backlight PWM frequency
      cfg.pwm_channel = 7; // PWM channel number to use

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // Sets the backlight to the panel.
    }
    //*/

    //*
    { // Configure settings for touch screen control. (delete if not necessary)
      auto cfg = _touch_instance.config();

      cfg.x_min = 0;           // Minimum X value (raw value) obtained from the touchscreen
      cfg.x_max = 319;         // Maximum X value (raw value) obtained from the touchscreen
      cfg.y_min = 0;           // Minimum Y value obtained from touchscreen (raw value)
      cfg.y_max = 479;         // Maximum Y value (raw value) obtained from the touchscreen
      cfg.pin_int = 7;         // pin number to which INT is connected
      cfg.bus_shared = true;   // Set true when using a common bus with the screen
      cfg.offset_rotation = 0; // Adjust when the display and touch orientation do not match Set with a value of 0 to 7
                               // For I2C connection
      cfg.i2c_port = 1;        // Select I2C to use (0 or 1)
      cfg.i2c_addr = 0x38;     // I2C device address number
      cfg.pin_sda = 6;         // pin number where SDA is connected
      cfg.pin_scl = 5;         // pin number to which SCL is connected
      cfg.freq = 400000;       // set I2C clock

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
    }
    //*/
    setPanel(&_panel_instance); // Sets the panel to use.
  }
};