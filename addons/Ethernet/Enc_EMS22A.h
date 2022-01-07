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
        pinMode(_clkPin,OUTPUT);
        digitalWrite(_clkPin,LOW);
        pinMode(_csPin,OUTPUT);
        digitalWrite(_csPin,LOW);
        pinMode(_dataPin,INPUT);  # was INPUT_PULLUP
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
            _offset = v - (int32_t)_position;
          }
        }
      }
      void setZero() {
        if (_axis == 1) nv.writeLong(EE_ENC_A1_ZERO,_offset);
        if (_axis == 2) nv.writeLong(EE_ENC_A2_ZERO,_offset);
        nv.commit();
      }
    private:
      uint32_t _position=0;
      int32_t _offset=0;
      int16_t _clkPin;
      int16_t _csPin;
      int16_t _dataPin;
      int16_t _axis;
      
      bool readEnc(uint32_t &encPos) {
        byte stream[16] = {0};

        // rate in microseconds, ie 2+2 = 4 = 250KHz
        int rate=2;
        // prepare for a reading
        digitalWrite(_csPin, HIGH);
        delayMicroseconds(rate);
        digitalWrite(_csPin, LOW);
        delayMicroseconds(rate);

        for (int i = 0; i < 16; i++) {
          digitalWrite(_clkPin, LOW);
          delayMicroseconds(rate);
          digitalWrite(_clkPin, HIGH);
          delayMicroseconds(rate);
   
          stream[i] = digitalRead(_dataPin);
        }

        digitalWrite(PIN_CLOCK, LOW);
        delayMicroseconds(rate);
        digitalWrite(PIN_CLOCK, HIGH);
        delayMicroseconds(rate);

        //extract 10 bit position from data stream use testStream
        uint32_t pos = 0;
        for (int i = 0; i < 10; i++) {
          pos = pos << 1;
          pos += stream[i];
        }

        encPos = pos;
        V("ENC: Current position axis "); V(_axis); V(" : "); VL(encPos);
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
