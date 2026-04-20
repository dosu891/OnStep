// ----------------------------------------------------------------------------------------------------------------
// Read BOURNS EMS22A type encoders
//@DS

#if AXIS1_ENC == EMS22A || AXIS2_ENC == EMS22A
  #ifdef EEPROM_DISABLED
    #error "Absolute encoders require NV/EEPROM which isn't supported on this platform"
  #endif

  #include <EEPROM.h>
  #include "EEProm.h"
  #define ENC_HAS_ABSOLUTE
  
  class EMS22A_Encoder {
    public:
      EMS22A_Encoder(int16_t csPin, int16_t clkPin, int16_t dataPin, int16_t axis) {
        _clkPin=clkPin;
        _csPin=csPin;
        _dataPin=dataPin;
        _axis=axis;
        _ports_initiated=false;  
        // HW not yet ready to initiate ports
        if (_axis == 1) _offset=nv.readLong(EE_ENC_A1_ZERO);
        if (_axis == 2) _offset=nv.readLong(EE_ENC_A2_ZERO);
      }
      int32_t read() {
        if (readEnc(_position)) {
          return (int32_t)_position + _offset;
        } else return INT32_MAX;
      }
      void write(int32_t v) {
        if (_position != INT32_MAX) {
          if (readEnc(_position)) {
            VL("Write!");
            _offset = v - (int32_t)_position;
          }
        }
      }
      void setZero() {
        if (_axis == 1) nv.writeLong(EE_ENC_A1_ZERO,_offset);
        if (_axis == 2) nv.writeLong(EE_ENC_A2_ZERO,_offset);
        nv.commit();
        V("ENC: Current offset writen to EEPROM for Axis"); V(_axis); V(" - Offset:"); VL(_offset);
      }
      void getZero() {    //@DS
        if (_axis == 1) _offset=nv.readLong(EE_ENC_A1_ZERO);
        if (_axis == 2) _offset=nv.readLong(EE_ENC_A2_ZERO);      
        V("ENC: Current offset loaded from EEPROM for Axis"); V(_axis); V(" - Offset:"); VL(_offset);
      }
    private:
      uint32_t _position=0;
      int32_t _offset=0;
      int16_t _clkPin;
      int16_t _csPin;
      int16_t _dataPin;
      int16_t _axis;
      bool _ports_initiated;

      void initiatePorts() {
          VF("EMS22A: Set pins axis ");VL(_axis);
          pinMode(_clkPin,OUTPUT);
          digitalWrite(_clkPin,HIGH);
          pinMode(_csPin,OUTPUT);
          digitalWrite(_csPin,HIGH);
          pinMode(_dataPin,INPUT);
          _ports_initiated = true;
      }
      
      bool readEnc(uint32_t &encPos) {
        // rate in microseconds, ie 2+2 = 4 = 250KHz
        int rate=1;
        int pos = 0;

        // initiate ports if not done yet
        if(!_ports_initiated)  initiatePorts();
        
        digitalWrite(_clkPin, HIGH);
        digitalWrite(_csPin, HIGH); // do not select the encoder (yet)
        delayMicroseconds(rate);
        digitalWrite(_csPin, LOW);  // select encoder to read
        delayMicroseconds(rate);
      
        byte stream[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        
        for (int i = 0; i < 16; i++) {
          digitalWrite(_clkPin, LOW);
          delayMicroseconds(rate);
          digitalWrite(_clkPin, HIGH);
          delayMicroseconds(rate);
      
          stream[i] = digitalRead(_dataPin);
          delayMicroseconds(rate);
        }
      
        digitalWrite(_clkPin, LOW);
        delayMicroseconds(rate);
        digitalWrite(_clkPin, HIGH);
        delayMicroseconds(rate);
      
        digitalWrite(_csPin, HIGH);  //encoder not active
        delayMicroseconds(rate);

        //extract 10 bit position from data stream use testStream
        pos = 0; //clear previous data
        for (int i = 0; i < 10; i++) {
          pos = pos << 1;
          pos += stream[i];
        }
      
        //calculate parity from the whole stream (which includes the 
        //parity bit, the result should always be 0
        int parity = 0;
        for (int i = 0; i < 16; i++) {
          if( stream[i] == 1 ){
            parity = 1 - parity;  //swap parity from 0 to 1 or for 1 to 0
          }
        } 

        encPos = parity == 0 ? pos : INT32_MAX;
        V("ENC: Current position axis "); V(_axis); V("_");V(_csPin);V(" : ");V("_");VL(encPos);
        return true;
      }
      
  };

#if AXIS1_ENC == EMS22A
  #define ENC_HAS_ABSOLUTE_AXIS1
  EMS22A_Encoder axis1Pos(AXIS1_ENC_CS_PIN,AXIS_ENC_CLK_PIN,AXIS_ENC_DATA_PIN,1);
#endif
#if AXIS2_ENC == EMS22A
  #define ENC_HAS_ABSOLUTE_AXIS2
  EMS22A_Encoder axis2Pos(AXIS2_ENC_CS_PIN,AXIS_ENC_CLK_PIN,AXIS_ENC_DATA_PIN,2);
#endif

#endif
