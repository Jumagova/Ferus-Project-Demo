#include <Arduino.h>
#include <screen_config.h>
#include <lvgl.h>
#include "ui.h"

#define DIR 21
#define PULL 14
#define MOTOR_OFF 0
#define MOTOR_ON 1

int motorState = MOTOR_OFF;
int motorDirection = 0; // 0 for forward, 1 for reverse
unsigned long motorPeriod = 5; // set period to 5ms
unsigned long motorLastTime = 0;
int motorDutyCycle = 0; // initial duty cycle value

LGFX tft;

/*Change to your screen resolution*/
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);
  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void updateMotor()
{
  unsigned long currentMillis = millis();
  if (motorState == MOTOR_ON && currentMillis - motorLastTime >= motorPeriod)
  {
    motorDutyCycle = (motorDutyCycle == 0) ? 255 : 0;
    analogWrite(PULL, motorDutyCycle);
    motorLastTime = currentMillis;
  }
}

void turnOnMotor(int direction)
{
  tft.setBrightness(255);
  motorDirection = direction;
  motorState = MOTOR_ON;
  Serial.println("Motor ON");
}

void turnOffMotor()
{
  tft.setBrightness(255);
  analogWrite(PULL, 255); // set duty cycle to a very low value
  motorState = MOTOR_OFF;
  Serial.println("Motor OFF");
}


void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  pinMode(PULL, OUTPUT); // set PULL as PWM output
  pinMode(DIR, OUTPUT);  // set DIR as Direction

  tft.begin();        /* TFT init */
  tft.setRotation(3); /* Landscape orientation, flipped */
  tft.setBrightness(255);

  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();

  Serial.println("Setup done");
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  updateMotor();
  delay(5);
}
