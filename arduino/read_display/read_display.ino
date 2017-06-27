#include <Wire.h>

int address = 56; // or 0x38 in hex is the i2c address of the ft5206

// some helper functions for communicating via i2c with the chip
int write_i2c(int addr, byte reg, int val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission();
}

int read_i2c(int addr, byte reg, byte data[], int len) {
  int stat;
  Wire.beginTransmission(addr);
  Wire.write(byte(reg));
  stat = Wire.endTransmission();
  Wire.requestFrom(addr, len);
  if (len <= Wire.available()) {
    for (int i = 0; i < len; i++) {
      data[i] = Wire.read();
    }
  }
  return stat;
}


void setup() {
  // set the i2c pins LOW in order to disable internal pullup resistors (from the AVR datasheet)
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);
  
  Wire.begin();
  Serial.begin(115200);

  // set 'factory mode', i.e. device mode 100b --> 0100 0000 = 0x40
  write_i2c(address, byte(0x00), byte(0x40));
  delay(100);

  // set 'gain' to 200. This value seems to give a good dynamic range without saturating.
  write_i2c(address, byte(0x07), byte(200));
  
}

void loop() {  
  // set high bit of register 0x00 (ST_SCAN bit) to start scan. Keep
  // the rest of the register as is, i.e. in factory mode --> 1100 0000 = 0xc0
  write_i2c(address, byte(0x00), byte(0xc0));
  delay(1);

  // scan is complete when ST_SCAN bit is set to zero, so wait until
  // it's done
  Wire.beginTransmission(address);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  int reading = 0;
  while(1) {
    Wire.requestFrom(address, 1);
    if (Wire.available()) {
      reading = Wire.read();
      int high_bit = (reading & (1 << 7));
      if (high_bit == 0) {
        break;
      }
    }
  }

  byte result[20]; // to store the readings of one row at a time
  
  // print the time in milliseconds as the first column in order to
  // be able to keep track of sampling intervals
  Serial.print(millis());
  Serial.print(",");

  // loop through sensor rows
  // here, only 5 rows are used so that
  // we can sample faster (reading all rows takes longer)
  for (int row_idx = 5; row_idx < 10; row_idx++) {
    
    // write the address of the row to be read and wait at least 100 us
    write_i2c(address, byte(0x01), byte(row_idx));
    delayMicroseconds(200);

    // read data from all columns at once. There are 10 sensors per row, and
    // each have a high byte and a low byte for a 16-bit integer value. The address
    // of the first column is 0x10 (16 in decimal).
    read_i2c(address, byte(16), result, 20);
    for (int col_idx = 3; col_idx < 7; col_idx++) {
      
      // reconstruct the integer from the high and low bytes
      int output = result[2*col_idx];
      output = output << 8;
      output |= result[2*col_idx + 1];
      
      Serial.print(output);
      Serial.print(",");
    }
  }
  Serial.print("\n");
}
