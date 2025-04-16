 
//==============================================================================
//=>                              megaDRUM                                    <= ENABLE_CHANNEL
//=>                                                                          <=
//=>                     Bateria Eletrônica Caseira                           <=
//=>                  Código Adaptado para Arduíno Mega                       <=
//=>                             DYMON'S                                      <=
//=>                                                                          <=
//=>  Créditos: Massimo Bernava                                               <=
//=>  Projeto original: microDrum -> https://github.com/massimobernava        <=
//=>                                                                          <=
//==============================================================================

//========CONFIGURE=============

#define FASTADC           1     // FASTADC = Prescaler_16, VERYFASTADC = Prescaler_8, VERYVERYFASTADC = Prescaler_4
#define SERIALSPEED       1     // 1 = 115200, 0 = 31250 (MIDI) , picoMIDI use 115200 therefore with nanoDrum and v0.8 use 1
#define ENABLE_CHANNEL    1     // Enable MIDI Channel (Sempre usa o canal padrão de bateria, ou seja, Canal 10. Eu prefiro alterar o canal via Software se precisar enviar peças em canais diferentes no VST)
//==============================
//#include <FastADC.h>

#if defined(__arm__) 
  //TODO: https://github.com/sebnil/DueFlashStorage
#elif defined(__AVR__) 
  #include <EEPROM.h>
#endif

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define TIMEFUNCTION millis() //NOT micros() (thresold error)
#define ANALOGREAD(s,p) analogRead(s)

#if defined(__arm__) 
  #define fastWrite(_pin_, _state_) digitalWrite(_pin_, _state_);
#elif defined(__AVR__) 
//#define fastWrite(_pin_, _state_) ( _pin_ < 8 ? (_state_ ?  PORTD |= 1 << _pin_ : PORTD &= ~(1 << _pin_ )) : (_state_ ?  PORTB |= 1 << (_pin_ -8) : PORTB &= ~(1 << (_pin_ -8)  )))
  #define fastWrite(_pin_, _state_) (_state_ ?  PORTD |= 1 << _pin_ : PORTD &= ~(1 << _pin_ ))
#endif

#if ENABLE_CHANNEL
  #define fastNoteOn(_channel,_note,_velocity) { Serial.write(0x90 | _channel);Serial.write(_note);Serial.write(_velocity); }
  #define fastMidiCC(_channel,_number,_value) { Serial.write((0xB0 | _channel)); Serial.write(_number); Serial.write(_value); }
#else
  #define fastNoteOn(_channel,_note,_velocity) { Serial.write(0x90 | 0x09); Serial.write(_note); Serial.write(_velocity);}
  #define fastMidiCC(_channel,_number,_value) { Serial.write((0xB0 | 0x09)); Serial.write(_number); Serial.write(_value); }
#endif

//===========MODE============
enum mode:byte
{
  Off     = 0,
  Standby = 1,
  MIDI    = 2,
  Tool    = 3
};
//============================

//===GLOBAL========================
mode Mode=MIDI;
unsigned long GlobalTime;
//=================================

//===SETTING================
//===General================

const int delayTime=10;
byte GeneralXtalk=0;

//Define o número de pinos utilizados, no caso 16 para MEGA
const byte NPin=15;


byte NSensor=2;
//===========================

//===HiHat==================
byte HHNoteSensor[]     = {22,24,25,26};
byte HHThresoldSensor[] = {40,30,20,10};
byte HHFootNoteSensor[] = {21,23};
byte HHFootThresoldSensor[] = {127,127};
//===========================

//===Xtalk===================
const byte NXtalkGroup=4;//Max number XTalk groups
int MaxXtalkGroup[NXtalkGroup] = {-1};
int MaxMultiplexerXtalk[8]={-1};
//===========================

 // Define as notas do chocke //
 //para funcionar em todos os pratos
 #define choke1 10
 #define choke2 11  
 #define choke3 12
 #define choke4 13
 #define chokeRide 14
 
  // ############# Define os Pinos Digitais dos Chokes ###################

//    Ao definir todos os chokes no mesmo pino digital (abaixo)
//    consigo parar o ataque de todos os pratos definidos (crash 1, splash, crash 2 e china)
//    apertando o único prato que possui realmente o choke (que no caso é o crash 1, à minha esquerda). 
//    Exemplo: prato 1 tem choke, eu toco o prato 2, 3, 4 e aperto o choke do prato 1, pronto, qualquer prato irá parar seu som...

 #define Choke1_Pin 50 
 #define Choke2_Pin 50 
 #define Choke3_Pin 50 
 #define Choke4_Pin 50 
 #define ChokeRide_Pin 50 
 

//Define o estado dos pinos digitais
 int Choke1_State = LOW;
 int Choke2_State = LOW; 
 int Choke3_State = LOW;
 int Choke4_State = LOW;
 int ChokeRide_State = LOW;
  
 
 int currentSwitchState = LOW;