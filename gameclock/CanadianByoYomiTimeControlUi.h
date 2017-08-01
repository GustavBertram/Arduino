
#ifndef __CanadianByoYomiTimeControlUi_h__
#define __CanadianByoYomiTimeControlUi_h__

#include <stdlib.h>

#include "TimeControlUi.h"
#include "ByoYomiTimeControl.h"

const char canadianByoYomiName[] PROGMEM = "Canadian ByoYomi";

const char canadianByoYomiOption1[] PROGMEM = "IGS Panda       1m +25mov/15m   ";
const char canadianByoYomiOption2[] PROGMEM = "IGS Panda       1m +25mov/10m   ";
const char canadianByoYomiOption3[] PROGMEM = "IGS Panda       1m +25mov/7m    ";
const char canadianByoYomiOption4[] PROGMEM = "IGS Panda       1m +25mov/5m    ";
const char canadianByoYomiOption5[] PROGMEM = "SAGA            45m +25mov/10m  ";

const char * const canadianByoYomiOptions[] PROGMEM = {
  canadianByoYomiOption1, canadianByoYomiOption2, canadianByoYomiOption3, canadianByoYomiOption4, canadianByoYomiOption5
};

const char canadianByoYomiUiNormalTime[] PROGMEM = "normal";
const char canadianByoYomiUiFormat[] PROGMEM = "%d";

class CanadianByoYomiTimeControlUi : public TimeControlUi {
  
public:

  virtual const char *getName() {
    return canadianByoYomiName;
  }
  
  virtual int16_t getNumberOfOptions() { 
    return 5;
  }
  
  virtual const char *getOption(int16_t option) {
    switch( option ) {
      case 0: return canadianByoYomiOptions[ option ];
      case 1: return canadianByoYomiOptions[ option ];
      case 2: return canadianByoYomiOptions[ option ];
      case 3: return canadianByoYomiOptions[ option ];
      case 4: return canadianByoYomiOptions[ option ];
    }
    return canadianByoYomiOptions[ 0 ];
  }
  
  virtual TimeControl *create(int16_t option) {
    uint32_t time = 1000L * 60L * 1L;
    uint16_t numberOfPlays = 25;
    uint16_t numberOfPeriods = 1;
    ByoYomiSetup byoYomiSetup;  
    
    switch(option) {
      case 0: 
        byoYomiSetup.time = time;
        byoYomiSetup.numberOfPeriods = numberOfPeriods;
        byoYomiSetup.periods[ 0 ].numberOfPlays = numberOfPlays;
        byoYomiSetup.periods[ 0 ].time = 1000L * 60L * 15L;
        break;
      case 1: 
        byoYomiSetup.time = time;
        byoYomiSetup.numberOfPeriods = numberOfPeriods;
        byoYomiSetup.periods[ 0 ].numberOfPlays = numberOfPlays;
        byoYomiSetup.periods[ 0 ].time = 1000L * 60L * 10L;
        break;
      case 2: 
        byoYomiSetup.time = time;
        byoYomiSetup.numberOfPeriods = numberOfPeriods;
        byoYomiSetup.periods[ 0 ].numberOfPlays = numberOfPlays;
        byoYomiSetup.periods[ 0 ].time = 1000L * 60L * 7L;
        break;
      case 3: 
        byoYomiSetup.time = time;
        byoYomiSetup.numberOfPeriods = numberOfPeriods;
        byoYomiSetup.periods[ 0 ].numberOfPlays = numberOfPlays;
        byoYomiSetup.periods[ 0 ].time = 1000L * 60L * 5L;
        break;
      case 4: 
        byoYomiSetup.time = 1000L * 60L * 45L;
        byoYomiSetup.numberOfPeriods = numberOfPeriods;
        byoYomiSetup.periods[ 0 ].numberOfPlays = numberOfPlays;
        byoYomiSetup.periods[ 0 ].time = 1000L * 60L * 10L;
        break;
    }

    return new ByoYomiTimeControl( byoYomiSetup );
  }
  
  virtual void renderGame(GameClock *gameClock, GameClockLcd *lcd) {
    TimeControlUi::renderGame( gameClock, lcd );
    
    ByoYomiTimeControl *canadian = ( ByoYomiTimeControl* )gameClock->getTimeControl();    
    if( canadian->isPlayerOneInNormalTime() ) {
      lcd->printBottomLeft( canadianByoYomiUiNormalTime );
    }else {
      lcd->sPrintBottomLeft( canadianByoYomiUiFormat, canadian->getPlayerOneRemainingNumberOfPlays() );
    }
    
    if( canadian->isPlayerTwoInNormalTime() ) {
      lcd->printBottomRight( canadianByoYomiUiNormalTime );
    }else {
      lcd->sPrintBottomRight( canadianByoYomiUiFormat, canadian->getPlayerTwoRemainingNumberOfPlays() );
    }
  }
  
};

#endif

