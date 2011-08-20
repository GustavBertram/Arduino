

#include <LiquidCrystal.h>

#include "base.h"
#include "ManualClock.h"
#include "ArduinoClock.h"
#include "GameClock.h"
#include "SuddenDeathTimeControl.h"
#include "HourGlassTimeControl.h"
#include "FischerDelayTimeControl.h"
#include "ByoYomiTimeControl.h"
#include "BronsteinDelayTimeControl.h"
#include "SimpleDelayTimeControl.h"
#include "PushButton.h"
#include "GameUiHandler.h"

GameClock gameClock;
Clock *clock;
TimeControl *timeControl;
PushButton playerOneButton( 9 );
PushButton playerTwoButton( 8 );
UiHandler *currentUiHandler;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void format(int32_t left, int32_t right, uint8_t *buffer) {
  buffer[ 0 ] = ( uint8_t )( '0' + ( left / 10 ) );
  buffer[ 1 ] = ( uint8_t )( '0' + ( left % 10 ) );
  buffer[ 2 ] = ':';
  buffer[ 3 ] = ( uint8_t )( '0' + ( right / 10 ) );
  buffer[ 4 ] = ( uint8_t )( '0' + ( right % 10 ) );
}

void format(int32_t time, uint8_t *buffer) {
  if( time <=0 ) {
    format( 0, 0, buffer );
    return;
  }
  int32_t hours = time / ( 1000L * 60L * 60L );
  int32_t minutes = ( time % ( 1000L * 60L * 60L ) ) / ( 1000L * 60L );
  int32_t seconds = ( ( time % ( 1000L * 60L * 60L ) ) % ( 1000L * 60L ) ) / 1000L;
  //Serial.print( "Time: " );Serial.print( time );Serial.print( " hours: " );Serial.print( hours );Serial.print( " minutes: " );Serial.print( minutes );Serial.print( " seconds: " );Serial.println( seconds );
  if( hours > 0 ) {
    format( hours, minutes, buffer );
  }else {
    format( minutes, seconds, buffer );
  }
}

void printTime() {
  uint8_t buffer[ 16 ];
  for(int i = 0; i < 16; ++i) {
    buffer[ i ] = ' ';
  }
  
  format( timeControl->getPlayerOneTime( clock ), buffer );
  format( timeControl->getPlayerTwoTime( clock ), &buffer[11] );
  //lcd.clear();
  lcd.setCursor( 0, 0 );
  lcd.print( (const char*)buffer );
  lcd.setCursor( 0, 1 );
  lcd.print( "Bronstein Delay" );
}

TimeControl *createByoYomiTimeControl() {
  ByoYomiSetup byoYomiSetup;
  byoYomiSetup.time = 30L * 1000L;
  byoYomiSetup.numberOfPeriods = 3;
  byoYomiSetup.periods[ 0 ].numberOfPlays = 3;
  byoYomiSetup.periods[ 0 ].time = 25L * 1000L;
  byoYomiSetup.periods[ 1 ].numberOfPlays = 2;
  byoYomiSetup.periods[ 1 ].time = 20L * 1000L;
  byoYomiSetup.periods[ 2 ].numberOfPlays = 1;
  byoYomiSetup.periods[ 2 ].time = 15L * 1000L;
  
  return new ByoYomiTimeControl( byoYomiSetup );
}

TimeControl * createBronsteinDelayTimeControl() {
  return new BronsteinDelayTimeControl( 20L * 1000L, 5L * 1000L );
}

TimeControl * createFischerDelayTimeControl() {
  return new FischerDelayTimeControl( 20L * 1000L, 5L * 1000L );
}

TimeControl * createSimpleDelayTimeControl() {
  return new SimpleDelayTimeControl( 20L * 1000L, 5L * 1000L );
}

void setup() {
  Serial.begin(9600);
  Serial.println( "-= BEGIN =-" );
  Serial.println( sizeof(GameClock) );
  Serial.println( sizeof(ByoYomiTimeControl) );
  Serial.println( sizeof(PushButton) );
  
  lcd.begin( 16, 2 );
  
  clock = new ArduinoClock();
  timeControl = createBronsteinDelayTimeControl();
  gameClock.setup( clock, timeControl );
  //gameClock.selectPlayerOne();
  
  currentUiHandler = new GameUiHandler();

  
  pinMode( 8, INPUT );
  pinMode( 9, INPUT );
}

void loop() {
  currentUiHandler->tick( clock );
  printTime();
  
  delay( 50 );
}

