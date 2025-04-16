//===============================
//  PRESCALER
//===============================
#if defined(__AVR__)
// Maximum sampling frequency    // Resolution
enum Prescaler {
  Prescaler_2 = B00000000, // 16 MHz / 2 = 8 MHz            //
  Prescaler_4 = B00000010, // 16 MHz / 4 = 4 MHz            // ~5.9
  Prescaler_8 = B00000011, // 16 MHz / 8 = 2 MHz            // ~7.4
  Prescaler_16 = B00000100, // 16 MHz / 16 = 1 MHz           // ~8.6
  Prescaler_32 = B00000101, // 16 MHz / 32 = 500 kHz         // ~8.9
  Prescaler_64 = B00000110, // 16 MHz / 64 = 250 kHz         // ~9.0
  Prescaler_128 = B00000111, // 16 MHz / 128 = 125 kHz        // ~9.1
};

inline void setPrescaler(int prescaler) {
  ADCSRA &= B11111000;
  ADCSRA |= prescaler;
}
#endif

//===============================

//==============================
//    SOFTRESET
//==============================
void softReset() {
  #if defined(__AVR__)
  asm volatile (" jmp 0");
  #endif 
//wdt_enable(WDTO_30MS);
}


//==============================
//    FREERAM
//==============================
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

//==============================
//    BLINK
//==============================
void blink()
{
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
}