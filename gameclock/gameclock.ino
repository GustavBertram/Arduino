
#include <LiquidCrystal.h>

#include "base.h"

#include "GameClock.h"
#include "PushButton.h"
#include "Buzzer.h"
#include "GameClockLcd.h"
#include "ArduinoClock.h"

#include "SelectTimeControlUiHandler.h"
#include "SelectTimeControlOptionUiHandler.h"
#include "GameUiHandler.h"

#include "ByoYomiTimeControlUi.h"
#include "CanadianByoYomiTimeControlUi.h"
#include "FischerDelayTimeControlUi.h"
#include "BronsteinDelayTimeControlUi.h"
#include "HourGlassTimeControlUi.h"
#include "SuddenDeathTimeControlUi.h"

//Hardware:
GameClockLcd lcd2(12, 11, 5, 4, 3, 2);
PushButton playerOneButton( BUTTON1 ), playerTwoButton( BUTTON2 ), menuButton( MIDDLEBUTTON );
Buzzer buzzer( BUZZER );

//GameClock...
ArduinoClock clock;
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
UiHandler *currentUiHandler;
SelectTimeControlUiHandler selectTimeControlUiHandler;
SelectTimeControlOptionUiHandler selectTimeControlOptionUiHandler;
GameUiHandler gameUiHandler;
GameButtonGestures buttonGestures;

void setup() {
  //Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  lcd2.init();
  playerOneButton.init();
  playerTwoButton.init();
  menuButton.init();
  
  selectTimeControlOptionUiHandler.wire( &selectTimeControlUiHandler );
  currentUiHandler = &selectTimeControlUiHandler;
}

void loop() {
  currentUiHandler->tick( &clock );
  buzzer.tick();
  currentUiHandler->render( &clock );
}

