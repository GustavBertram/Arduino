

#include <LiquidCrystal.h>
#include <MemoryFree.h>

#include "base.h"

#include "ManualClock.h"
#include "ArduinoClock.h"

#include "GameClock.h"
#include "PushButton.h"

#include "SelectTimeControlUiHandler.h"
#include "SelectTimeControlOptionUiHandler.h"
#include "GameUiHandler.h"

#include "ByoYomiTimeControlUi.h"
#include "CanadianByoYomiTimeControlUi.h"
#include "FischerDelayTimeControlUi.h"
#include "BronsteinDelayTimeControlUi.h"
#include "HourGlassTimeControlUi.h"
#include "SuddenDeathTimeControlUi.h"
//#include "NormalDelayTimeControlUi.h"


//Hardware:
PushButton playerOneButton( 9 ), playerTwoButton( 8 ), selectButton( 7 ), backButton( 6 );
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//GameClock...
Clock *clock;
GameClock gameClock;

//TimeControls:
TimeControlUi *timeControls[] = {
  new SuddenDeathTimeControlUi(),
  new HourGlassTimeControlUi(),
  new ByoYomiTimeControlUi(),
  new CanadianByoYomiTimeControlUi(),
  new FischerDelayTimeControlUi(),
  new BronsteinDelayTimeControlUi(),
  NULL
};

//User Interface ...
SelectTimeControlUiHandler selectTimeControlUiHandler;
SelectTimeControlOptionUiHandler selectTimeControlOptionUiHandler;
GameUiHandler gameUiHandler;
UiHandler *currentUiHandler;

void bug() {
  //char buffer[33];
  //buffer[32] = '\0';
  //const prog_char *option = byoYomiOptions[ 0 ];//byoYomiOption1;//timeControls[ 1 ]->getOption( 0 );
  //const prog_char *option = timeControls[ 2 ]->getOption( 2 );
  //memcpy_P( buffer, option, 32 );
  //Serial.println( buffer  );
  //for(int i = 0; i < 32; ++i) {
  //  Serial.print( ( char )buffer[i] );
  //}
}

void setup() {
  Serial.begin(9600);
  Serial.println( "-= :) =-" );
  Serial.println( sizeof(GameClock) );
  Serial.println( sizeof(ByoYomiTimeControl) );
  Serial.println( sizeof(PushButton) );
  
  lcd.begin( 16, 2 );
    
  clock = new ArduinoClock();
  
  //gameUiHandler.setTimeControlUi( timeControls[ 1 ] );
  //currentUiHandler = &gameUiHandler;
  currentUiHandler = &selectTimeControlUiHandler;

  selectTimeControlOptionUiHandler.wire( &selectTimeControlUiHandler );

  //gameClock.setup( clock, timeControls[ 1 ]->create( 0 ) );

  pinMode( 6, INPUT );
  pinMode( 7, INPUT );
  pinMode( 8, INPUT );
  pinMode( 9, INPUT );
  Serial.print( "freeMem=" );Serial.println(freeMemory());  
}

void loop() {
  currentUiHandler->tick( clock );
  currentUiHandler->render( clock );
  
  delay( 50 );
}

