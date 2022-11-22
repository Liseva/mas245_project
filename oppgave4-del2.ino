#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <FlexCAN.h>

//Tillegskode for IMU - for å lese av
Adafruit_MPU6050 mpu;






#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

static CAN_message_t msg;
static uint8_t hex[17] = "0123456789abcdef";


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   11
#define OLED_CLK   13
#define OLED_DC    6
#define OLED_CS    10
#define OLED_RESET 5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


// til CAN
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}


void setup() 
{

//IMU:
 Serial.begin(115200);

 while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
 switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }




  // put your setup code here, to run once:


  //Serial.begin(9600);
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.drawRoundRect(1, 1, 126 , 62,5 ,SSD1306_WHITE); // tegne avrundet firkant rundt

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(12, 5);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.drawLine(12, 13, 128-12, 13, SSD1306_WHITE );

  display.write("MAS245 - Gruppe 5");

  display.setCursor(12, 15); 

  display.write("CAN-statistikk");
  display.display();


  // CAN-bus
  delay(1000);
  //Serial.println(F("Hello Teensy 3.6 dual CAN Test."));

  Can0.begin();  
  Can1.begin();

  //if using enable pins on a transceiver they need to be set on
  pinMode(2, OUTPUT);
  pinMode(35, OUTPUT);

  digitalWrite(2, HIGH);
  digitalWrite(35, HIGH);

  msg.ext = 0;
  msg.id = 0x22;
  msg.len = 8;
  msg.buf[0] = 0;
  msg.buf[1] = 20;
  msg.buf[2] = 0;
  msg.buf[3] = 100;
  msg.buf[4] = 128;
  msg.buf[5] = 64;
  msg.buf[6] = 32;
  msg.buf[7] = 16;

  

}

int count = 0;
int count_out = 0;
// -------------------------------------------------------------
void loop(void)
{
    /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
 
  CAN_message_t inMsg;
  Can0.available();
  Can0.read(inMsg);
  if(inMsg.id == 33) //33 er lik 0x21 i hexadesimale tall
  {
    while (1) 
    {
     mpu.getEvent(&a, &g, &temp);

    Serial.print("CAN bus 0: "); hexDump(8, inMsg.buf);
  
   display.drawRect(12, 25, 100, 8, SSD1306_BLACK);
    display.fillRect(12, 25, 100, 8, SSD1306_BLACK);

    display.setCursor(12, 25); 
    display.write("Aks-z:");
    display.setCursor(70, 25); 
    display.print(a.acceleration.z);
    display.display();

    msg.buf[0] = a.acceleration.x;
    msg.buf[1] = a.acceleration.y;
    msg.buf[2] = a.acceleration.z;
    Can0.write(msg);

    delay(1000); //rate på 1Hz
    
    //Serial.println(count);
    }
  }

 

}
