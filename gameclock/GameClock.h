
#ifndef __GameClock_h__
#define __GameClock_h__

#include "Clock.h"
#include "TimeControl.h"
#include "TimeTrackerImpl.h"
#include "Buzzer.h"

// Pins
const int LED1 = 6;
const int LED2 = 7;
const int BUZZER = 13;
const int BUTTON1 = 8;
const int BUTTON2 = 10;
const int MIDDLEBUTTON = 9;

extern Buzzer buzzer;

class GameClock {
  
  Clock *clock;
  TimeControl *timeControl;
  TimeTrackerImpl playerOne, playerTwo;
  
  TimeTrackerImpl *currentPlayer, *playerThatLoose;
  bool paused;
  bool overNotified;
  
public:

  GameClock() {
    clock = NULL;
    playerThatLoose = currentPlayer = NULL;
    paused = false;
    overNotified = false;
  }
  
  virtual ~GameClock() {
  }
  
  void setup(Clock *clock, TimeControl *timeControl) {
    this->clock = clock;
    this->timeControl = timeControl;
    
    this->timeControl->setup( &playerOne, &playerTwo );
  }
  
  void tick() {
    if( isOver() && overWasNotNotified() ) {
      onOver();
    }
    if( isPaused() || isOver() ) {
      return;
    }
    if( isPlayerOnePlaying() ) {
      checkIfPlayerOneTimeExpired();
    } else if( isPlayerTwoPlaying() ) {
      checkIfPlayerTwoTimeExpired();
    }
    dumpState();
  }
  
  void selectPlayerOne() {
    if( isOver() || isPaused() || isPlayerOnePlaying() ) {
      return;
    }
    if( isPlayerTwoPlaying() ) {
      onPlayerTwoPlayed();
    }
    onPlayerOneBeginToPlay();
  }
  
  void selectPlayerTwo() {
    if( isOver() || isPaused() || isPlayerTwoPlaying() ) {
      return;
    }
    if( isPlayerOnePlaying() ) {
      onPlayerOnePlayed();
    }
    onPlayerTwoBeginToPlay();
  }
  
  void pause() {
    if( currentPlayer == NULL || isPaused() || isOver() ) {
      return;
    }
    currentPlayer->beginPause( clock );
    paused = true;
    beep();
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
  
  void resume() {
    if( isOver() || !isPaused() ) {
      return;
    }
    currentPlayer->endPause( clock );
    paused = false;

    if( isPlayerOnePlaying() ) {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
    } else if( isPlayerTwoPlaying() ) {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
    }
    
    beep();
  }
  
  Clock *getClock() {
    return clock;
  }
  
  TimeControl *getTimeControl() {
    return timeControl;
  }
  
  bool isPaused() {
    return paused;
  }

  bool isOver() {
    return timeControl != NULL && timeControl->isOver();
  }

  bool isPlayerOnePlaying() {
    return currentPlayer == &playerOne;
  }
  
  bool isPlayerTwoPlaying() {
    return currentPlayer == &playerTwo;
  }
  
  bool playerOneWon() {
    return isOver() && playerThatLoose == &playerTwo;
  }
  
  bool playerTwoWon() {
    return isOver() && playerThatLoose == &playerOne;
  }

private:

  void onPlayerOneBeginToPlay() {
    currentPlayer = &playerOne;
    currentPlayer->mark( clock );
    timeControl->onPlayerOneBeganToPlay();
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    beep();
  }

  void onPlayerOnePlayed() {
    currentPlayer->consume( clock );
    timeControl->onPlayerOnePlayed();
  }
  
  void checkIfPlayerOneTimeExpired() {
    if( playerOne.getTime( clock ) > 0 ) {
      return;
    }
    if( playerOne.onlyOnceIsExpired() ) {
      playerOne.consume( clock );
      playerOne.mark( clock );
      timeControl->onPlayerOneTimeExpired();
    }
  }
  
  void onPlayerTwoPlayed() {
    currentPlayer->consume( clock );
    timeControl->onPlayerTwoPlayed();
  }

  void onPlayerTwoBeginToPlay() {
    currentPlayer = &playerTwo;
    currentPlayer->mark( clock );
    timeControl->onPlayerTwoBeganToPlay();
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    beep();
  }
  
  void checkIfPlayerTwoTimeExpired() {
    if( playerTwo.getTime( clock ) > 0 ) {
      return;
    }
    if( playerTwo.onlyOnceIsExpired() ) {
      playerTwo.consume( clock );
      playerTwo.mark( clock );
      timeControl->onPlayerTwoTimeExpired();
    }
  }
  
  bool overWasNotNotified() {
    return !overNotified;
  }

  void onOver() {
    buzzer.playTone( 3520, 2000 );
    overNotified = true;
    playerThatLoose = currentPlayer;
    currentPlayer = NULL;
  }
  
  void beep() {
    buzzer.playTone( 880, 250 );
  }
  
  void dumpState() {
    /*
    Serial.print( "Clock time: " );Serial.print( clock->currentTime() );Serial.print( " isOver: " );Serial.print( ( int )isOver() );
    Serial.print( " Player one isPlaying: " );Serial.print( ( int )isPlayerOnePlaying() );Serial.print( " time: " );Serial.print( playerOne.getTime( clock ) );
    Serial.print( " Player two isPlaying: " );Serial.print( ( int )isPlayerTwoPlaying() );Serial.print( " time: " );Serial.println( playerTwo.getTime( clock ) );
    //*/
  }

};

#endif

