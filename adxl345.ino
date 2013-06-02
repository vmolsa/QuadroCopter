
#include <Wire.h>
#include <SPI.h>
#include <I2C.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <NewPing.h>

//  I2C

//  BOARD    PIN     Color       ( 4.7K pullup resistors! )
//  3,3v   = Vcc   = red 
//  GND    = Gnd   = black
//  20     = SDA   = blue
//  21     = SCL   = Yellow
//  GND    = SDO   = black
//  3,3v   = CS    = red

//  "CONFIG"

#define HAVE_SERIAL 1
#define HAVE_BATTERY 1
//#define HAVE_ADXL345 1
//#define HAVE_HMC6352 1
//#define HAVE_L3G4200D 1
//#define HAVE_BMP085 1
//#define HAVE_W5200 1

#define ADXL345_DEVICE (0x53)
#define HMC6352_DEVICE (0x21)
#define L3G4200D_DEVICE (0x68)
#define BMP085_DEVICE (0x77)

// GLOBAL

int i, i2;
byte buffer[24];
unsigned long time = 0;

void printDouble(double val, unsigned int precision) {
    Serial.print (int(val));
    Serial.print(".");

    unsigned int frac;
    
    if (val >= 0) {
        frac = (val - int(val)) * precision;
    }
    
    else {
        frac = (int(val)- val ) * precision;
    }
    
    Serial.print(frac,DEC) ;
}

//  ADXL345

char ADXL345_BUFFER_SIZE   = 6;
char ADXL345_POWER_CTL     = 0x2D;
char ADXL345_DATA_FMT      = 0x31;
char ADXL345_ADDR          = 0x32;

int ADXL345_x = 0;
int ADXL345_y = 0;
int ADXL345_z = 0;

void initADXL345() {
  I2c.write(ADXL345_DEVICE,ADXL345_DATA_FMT,0x01);
  I2c.write(ADXL345_DEVICE,ADXL345_POWER_CTL,0x08);
  
#ifdef HAVE_SERIAL
  Serial.println("initADXL345(READY)");
#endif  
}

void loopADXL345() {
  I2c.write(ADXL345_DEVICE, ADXL345_ADDR);
  I2c.read(ADXL345_DEVICE, ADXL345_BUFFER_SIZE);
  
  for (i = 0; i < ADXL345_BUFFER_SIZE; i++) {
      buffer[i] = I2c.receive();
  }
  
  if (i == ADXL345_BUFFER_SIZE) {
    ADXL345_x = (((int) buffer[1]) << 8) | buffer[0];    // TODO Check bitwise 
    ADXL345_y = (((int) buffer[3]) << 8) | buffer[2];    // TODO Check bitwise 
    ADXL345_z = (((int) buffer[5]) << 8) | buffer[4];    // TODO Check bitwise 
  }
}

void debugADXL345() {
  Serial.print("[DEBUG] ADXL345: X(");
  Serial.print(ADXL345_x);
  Serial.print(") Y(");
  Serial.print(ADXL345_y);
  Serial.print(") Z(");
  Serial.print(ADXL345_z);
  Serial.println(")"); 
}

//  HMC6352

int HMC6352_val = 0;
char HMC6352_BUFFER_SIZE = 2;

void initHMC6352() {
//  I2c.write(HMC6352_DEVICE, "O");
#ifdef HAVE_SERIAL
  Serial.println("initHMC6352(READY)");
#endif  
}

void loopHMC6352() {
  I2c.write(HMC6352_DEVICE, 'A');
  
  delay(10);  //  <- giving time to device :/

  I2c.read(HMC6352_DEVICE, HMC6352_BUFFER_SIZE);

  for (i = 0; i < HMC6352_BUFFER_SIZE; i++) {
    buffer[i] = I2c.receive();
  }
  
  if (i == HMC6352_BUFFER_SIZE) {
    HMC6352_val = buffer[0]*256 + buffer[1];    
  }
}

void debugHMC6352() {
  Serial.print("[DEBUG] HMC6352: value(");
  Serial.print((HMC6352_val & 0xffffff00));
  Serial.print(".");
  Serial.print((HMC6352_val & 0x000000ff));
  Serial.println(")");
}

//  L3G4200D

int L3G4200D_BUFFER_SIZE = 6;
int L3G4200D_x = 0;
int L3G4200D_y = 0;
int L3G4200D_z = 0;

char L3G4200D_CONFIG_SIZE = 10;
char L3G4200D_CONFIG_FLAGS[] = { 0x20, 0x0F, 0x21, 0x00, 0x22, 0xA0, 0x23, 0x30, 0x24, 0x00 };

//char L3G4200D_GET_FLAGS[] = { 0x29, 0x28, 0x2B, 0x2A, 0x2D, 0x2C };

void debugL3G4200D() {
  Serial.print("[DEBUG] L3G4200D: X(");
  Serial.print(L3G4200D_x);
  Serial.print(") Y(");
  Serial.print(L3G4200D_y);
  Serial.print(") Z(");
  Serial.print(L3G4200D_z);
  Serial.println(")"); 
}

void initL3G4200D() {
  for (i = 0; i < L3G4200D_CONFIG_SIZE; i += 2) {
    I2c.write(L3G4200D_DEVICE, L3G4200D_CONFIG_FLAGS[i], L3G4200D_CONFIG_FLAGS[(i + 1)]);
  }
  
#ifdef HAVE_SERIAL
  Serial.println("initL3G4200D(READY)");
#endif  
}

void loopL3G4200D() {
  I2c.read(L3G4200D_DEVICE, 0xA8, 6);
  
  for (i = 0; i < L3G4200D_BUFFER_SIZE; i++) {
      buffer[i] = I2c.receive();
  }  
   
  L3G4200D_x = (((int) buffer[1]) << 8) | buffer[0];
  L3G4200D_y = (((int) buffer[3]) << 8) | buffer[2];
  L3G4200D_z = (((int) buffer[5]) << 8) | buffer[4];
}

//  BMP085

int BMP085_altitude = 0;
short BMP085_temperature = 0;
long BMP085_pressure = 0;
int BMP085_zeroMpressure = 101325;

const unsigned char BMP085_OSS = 0;

char BMP085_CONFIG_SIZE = 11;
char BMP085_CONFIG_FLAGS[] = { 0xAA, 0xAC, 0xAE, 0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE };

int BMP085_ac1;
int BMP085_ac2; 
int BMP085_ac3; 
unsigned int BMP085_ac4;
unsigned int BMP085_ac5;
unsigned int BMP085_ac6;
int BMP085_b1; 
int BMP085_b2;
int BMP085_mb;
int BMP085_mc;
int BMP085_md;
long BMP085_b5;

unsigned long BMP085_tmp_pressure = 0;
unsigned int BMP085_tmp_temperature = 0;

void debugBMP085() {
  Serial.print("[DEBUG] BMP085: Altidude(");
  Serial.print(BMP085_altitude);
  Serial.print(") Pressure(");
  Serial.print(BMP085_pressure);
  Serial.print(") Temperature(");
  Serial.print(BMP085_temperature);
  Serial.println(")");
}

void initBMP085() {  
  for (i = 0, i2 = 0; i < BMP085_CONFIG_SIZE; i++, i2 += 2) {
    I2c.read(BMP085_DEVICE, BMP085_CONFIG_FLAGS[i], 2);
 
    buffer[i2] = I2c.receive();
    buffer[(i2 + 1)] = I2c.receive();
  }
  
  BMP085_ac1 = (((int) buffer[0]) << 8) | buffer[1];
  BMP085_ac2 = (((int) buffer[2]) << 8) | buffer[3];
  BMP085_ac3 = (((int) buffer[4]) << 8) | buffer[5];
  BMP085_ac4 = (((int) buffer[6]) << 8) | buffer[7];
  BMP085_ac5 = (((int) buffer[8]) << 8) | buffer[9];
  BMP085_ac6 = (((int) buffer[10]) << 8) | buffer[11];
  BMP085_b1 = (((int) buffer[12]) << 8) | buffer[13];
  BMP085_b2 = (((int) buffer[14]) << 8) | buffer[15];
  BMP085_mb = (((int) buffer[16]) << 8) | buffer[17];
  BMP085_mc = (((int) buffer[18]) << 8) | buffer[19];
  BMP085_md = (((int) buffer[20]) << 8) | buffer[21];

#ifdef HAVE_SERIAL
  Serial.println("initBMP085(READY)");
#endif  
}

void loopBMP085() {
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  I2c.write(BMP085_DEVICE, 0xF4, 0x24);
  
  delay(5);    //  <- giving time to device :/

  I2c.read(BMP085_DEVICE, 0xF6, 2);
  buffer[0] = I2c.receive();
  buffer[1] = I2c.receive();

  I2c.write(BMP085_DEVICE, 0xF4, (0x34 + (BMP085_OSS << 6))); 
  
  delay(2 + (3 << BMP085_OSS));
  
  I2c.read(BMP085_DEVICE, 0xF6, 3);
  buffer[2] = I2c.receive();
  buffer[3] = I2c.receive();
  buffer[4] = I2c.receive();  
  
  BMP085_tmp_temperature = ((int) (buffer[0] << 8) | buffer[1]);
  BMP085_tmp_pressure = (((unsigned long) buffer[2] << 16) | ((unsigned long) buffer[3] << 8) | (unsigned long) buffer[4]) >> (8 - BMP085_OSS); 
  
  b6 = BMP085_b5 - 4000;
  x1 = (BMP085_b2 * (b6 * b6) >> 12) >> 11;
  x2 = (BMP085_ac2 * b6) >> 11;
  x3 = x1 + x2;

  b3 = (((((long) BMP085_ac1) * 4 + x3) << BMP085_OSS) + 2) >> 2;
  
  x1 = (BMP085_ac3 * b6) >> 13;
  x2 = (BMP085_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;

  b4 = (BMP085_ac4 * (unsigned long) (x3 + 32768)) >> 15;
  
  b7 = ((unsigned long) (BMP085_tmp_pressure - b3) * (50000 >> BMP085_OSS));

  if (b7 < 0x80000000) {
    p = (b7 << 1) / b4;
  }
  
  else {
    p = (b7 / b4) << 1;
  }
  
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  p += (x1 + x2 + 3791) >> 4;  
  
  BMP085_pressure = p;
  
  x1 = (((long) BMP085_tmp_temperature - (long) BMP085_ac6) * (long) BMP085_ac5) >> 15;
  x2 = ((long) BMP085_mc << 11) / (x1 + BMP085_md);
  BMP085_b5 = x1 + x2;
  
  BMP085_temperature = ((BMP085_b5 + 8) >> 4);
  BMP085_altitude = (float) 44330 * (1 - pow(((float) BMP085_pressure / BMP085_zeroMpressure), 0.190295)); 
}

//  UDP

unsigned long UDP_time;

byte UDP_mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
EthernetUDP UDP_sock;
EthernetUDP UDP_multicastSock;
IPAddress UDP_multicastAddr(224, 0, 0, 1);
IPAddress UDP_src(192, 168, 1, 50);
IPAddress UDP_dst(192, 168, 1, 1);
int UDP_multicastPort = 9001;
int UDP_srcPort = 9000;
int UDP_dstPort = 1234;

char UDP_request[UDP_TX_PACKET_MAX_SIZE];
char UDP_response[UDP_TX_PACKET_MAX_SIZE];

int UDP_in = 0;
int UDP_out = 0;

void debugUDP() {
  Serial.print("[DEBUG] ");
  
  for (int i =0; i < 4; i++) {
    Serial.print(UDP_dst[i], DEC);
    if (i < 3) { Serial.print("."); }
  }
  
  Serial.print(":");
  Serial.print(UDP_dstPort);
  Serial.print("(");
  Serial.print(UDP_in);
  Serial.println(" bytes"); 
}

void handleResponse() {
    UDP_sock.beginPacket(UDP_dst, UDP_dstPort);
    UDP_sock.write((const unsigned char *) UDP_response, UDP_out);
    UDP_sock.endPacket();  
}

void handleRequest() {
    for (i = 0; i < UDP_in; i++) {
        // TODO 
        switch (UDP_request[i]) {
           default:
             break;  
        }
    }
}

void initUDP() {
  
  while (Ethernet.begin(UDP_mac) == 0) {
#ifdef HAVE_SERIAL
    Serial.println("Waiting IP Address..");
#endif
    delay(1000);
  }
   
//  Ethernet.begin(UDP_mac, UDP_src);

  UDP_src = Ethernet.localIP();
  UDP_sock.begin(UDP_srcPort);

//  UDP_multicastSock.beginMultiCast(UDP_multicastAddr, UDP_multicastPort);
  
  UDP_time = millis();

#ifdef HAVE_SERIAL
  Serial.print("[DEBUG] Ip Address: ");
  Serial.println(UDP_src);
#endif
}

void loopUDP() {
  if (time < UDP_time || (time - UDP_time) >= 100) {
    UDP_time = millis(); 
    UDP_in = UDP_sock.parsePacket();

    if (UDP_in) {
      UDP_out = 0;
      UDP_dst = UDP_sock.remoteIP();
      UDP_dstPort = UDP_sock.remotePort();
      UDP_sock.read(UDP_request, UDP_TX_PACKET_MAX_SIZE);
      handleRequest();
    }
  }
}

//  Battery

int battery_value = 0;
double battery_voltage = 0.0;
double battery_cell = 0.0;
double battery_div = 212.658228;

void debugBattery() {
    Serial.print("[DEBUG] Battery: (");
    Serial.print(battery_value);
    Serial.print(") Cell (");
    printDouble(battery_cell, 100);
    Serial.print(") Voltage (");
    printDouble(battery_voltage, 100);
    Serial.println(")");
}

void initBattery() {
      
}

void loopBattery() {
  battery_value = analogRead(A0);
  battery_cell = (battery_value / battery_div);
  battery_voltage = (battery_cell * 6);
}

// Looper

unsigned long LOOPER_time = 0;
unsigned int LOOPER_res = 0;
unsigned int LOOPER_counter = 0;

void loopLooper() {
  LOOPER_res = ((int) time - LOOPER_time);
  LOOPER_counter += 1;

  if (LOOPER_res >= 1001) {
    Serial.print("[DEBUG] ");
    Serial.print(LOOPER_counter);
    Serial.println(" Loops / Second");
    
    LOOPER_time = millis();
    LOOPER_counter = 0; 
  }
}

void initLooper() {
  LOOPER_time = millis();
}

//    !!!MAIN!!!

void setup() {

#ifdef HAVE_SERIAL  
  Serial.begin(115200);
  Serial.print("READY!");  
#endif
  
  Wire.begin();
  I2c.begin();

#ifdef HAVE_ADXL345   
  initADXL345();
#endif
  
#ifdef HAVE_HMC6352  
  initHMC6352();
#endif

#ifdef HAVE_L3G4200D  
  initL3G4200D();
#endif

#ifdef HAVE_BMP085
  initBMP085();
#endif

#ifdef HAVE_W5200 
  initUDP();
#endif

}

void loop() {
  time = millis();

#ifdef HAVE_ADXL345   
  loopADXL345();
#endif
  
#ifdef HAVE_HMC6352  
  loopHMC6352();
#endif
  
#ifdef HAVE_L3G4200D  
  loopL3G4200D();
#endif

#ifdef HAVE_BMP085
  loopBMP085();
#endif

#ifdef HAVE_W5200 
  loopUDP();
#endif

#ifdef HAVE_BATTERY
  loopBattery();
#endif

loopLooper();

#ifdef HAVE_SERIAL
//  if (loopLooper() == 1) {
    #ifdef HAVE_ADXL345 
      debugADXL345();
    #endif
  
    #ifdef HAVE_HMC6352
      debugHMC6352();
    #endif
  
    #ifdef HAVE_L3G4200D
      debugL3G4200D();
    #endif
  
    #ifdef HAVE_BMP085
      debugBMP085();
    #endif
    
    #ifdef HAVE_BATTERY
      debugBattery();
    #endif
//  }
#endif

  delay(500);

//  UDP_sock.beginPacket(UDP_dst, UDP_dstPort);
//  UDP_sock.write((const uint8_t*) "Ehlo!\n", 6);
//  UDP_sock.endPacket(); 
  
//  UDP_multicastSock.beginPacket(UDP_multicastAddr, UDP_multicastPort);
//  UDP_multicastSock.write((const uint8_t*) "Ehlo!\n", 6);
//  UDP_multicastSock.endPacket(); 

}

