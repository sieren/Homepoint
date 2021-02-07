#include <M5Core2.h>
#include "Goals.h"

extern Button A;
extern Button B;

Goal::Goal() { name = ""; success = false; }
bool  Goal::passed() { return success; }
const char* Goal::getName(){ return name.c_str(); }
bool  Goal::test() {
  M5.Lcd.fillRect(0, TEXT_TOP, 320, TEXT_HEIGHT, NAVY);
  M5.Lcd.drawCentreString(name, TEXT_CENTER, TEXT_TOP, TEXT_FONT);
  start_time = millis();
  while(start_time + TEST_DURRATION > millis()) {
    M5.update();
    delay(1);
    if(success) {
      return true;
    }
  }
  return false;
}


//  Tap the A button
//
TapAGoal::TapAGoal() { name = "Tap the A Button"; }
// Set success to true if all the conditions of the goal are met
void TapAGoal::event_handler(Event& e) {
  if((E_TAP == e) && (0 == strcmp("A", e.button->getName()))) success = true;
}


//  Tap the B button
//
TapBGoal::TapBGoal() { name = "Tap the B Button"; }
void TapBGoal::event_handler(Event& e) {
  if((E_TAP == e) && (0 == strcmp("B", e.button->getName()))) success = true;
}

// Long Press (LONG_PRESS_TIME mS) on the A Button
//
LongPressAGoal::LongPressAGoal() { name = "Long Press the A Button"; }
void LongPressAGoal::event_handler(Event& e) {
  if((E_LONGPRESSED == e) && (0 == strcmp("A", e.button->getName()))) success = true;
}


// Long Press (LONG_PRESS_TIME mS) on the B Button
//
LongPressBGoal::LongPressBGoal() { name = "Long Press the B Button"; }
void LongPressBGoal::event_handler(Event& e) {
  if((E_LONGPRESSED == e) && (0 == strcmp("B", e.button->getName()))) success = true;
}


// Long Press (LONG_PRESS_TIME mS) on the Background
//
LongPressBackgroundGoal::LongPressBackgroundGoal() { name = "Long Press the Background"; }
void LongPressBackgroundGoal::event_handler(Event& e) {
  if((E_LONGPRESSED == e) && (0 == strcmp("background", e.button->getName()))) success = true;
}


//  Double Tap the A button
//
DoubleTapAGoal::DoubleTapAGoal() { name = "Double Tap the A Button"; }
// Set success to true if all the conditions of the goal are met
void DoubleTapAGoal::event_handler(Event& e) {
  if((E_DBLTAP == e) && (0 == strcmp("A", e.button->getName()))) success = true;
}


//  Double Tap the B button
//
DoubleTapBGoal::DoubleTapBGoal() { name = "Double Tap the B Button"; }
void DoubleTapBGoal::event_handler(Event& e) {
  if((E_DBLTAP == e) && (0 == strcmp("B", e.button->getName()))) success = true;
}


//  Tap the Background
//
TapBackgroundGoal::TapBackgroundGoal() { name = "Tap the Background"; }
// Set success to true if all the conditions of the goal are met
void TapBackgroundGoal::event_handler(Event& e) {
  if((E_TAP == e) && (0 == strcmp("background", e.button->getName()))) success = true;
}


//  Double Tap the Background
//
DoubleTapBackgroundGoal::DoubleTapBackgroundGoal() { name = "Double Tap the Background"; }
void DoubleTapBackgroundGoal::event_handler(Event& e) {
  if((E_DBLTAP == e) && (0 == strcmp("background", e.button->getName()))) success = true;
}


// Drag from A to B
//
DragFromAtoBGoal::DragFromAtoBGoal() { name = "Drag From A to B"; }
// The series of events I see is: E_TOUCH(A), E_MOVE(A)..., E_PRESSING(a), E_MOVE(A)..., E_RELEASE(A), E_DRAGGED(A)
// Button never reflects another object; get position and test location.
void DragFromAtoBGoal::event_handler(Event& e) {
  if(E_DRAGGED == e) {
    if(A.contains(e.from) && B.contains(e.to)) success = true;
  }
}


// Drag from B to A
//
DragFromBtoAGoal::DragFromBtoAGoal() { name = "Drag From B to A"; }
void DragFromBtoAGoal::event_handler(Event& e) {
  if(E_DRAGGED == e) {
    if(B.contains(e.from) && A.contains(e.to)) success = true;
  }
}


// Drag from A to Background
//
DragFromAtoBackgroundGoal::DragFromAtoBackgroundGoal() { name = "Drag From A to Background"; }
void DragFromAtoBackgroundGoal::event_handler(Event& e) {
  if(E_DRAGGED == e) {
    if(A.contains(e.from) && M5.background.contains(e.to) && !A.contains(e.to) && !B.contains(e.to)) success = true;
  }
}


// Drag from B to Background
//
DragFromBtoBackgroundGoal::DragFromBtoBackgroundGoal() { name = "Drag From B to Background"; }
void DragFromBtoBackgroundGoal::event_handler(Event& e) {
  if(E_DRAGGED == e) {
    if(B.contains(e.from) && M5.background.contains(e.to) && !A.contains(e.to) && !B.contains(e.to)) success = true;
  }
}


// Drag from Background to A
//
DragFromBackgroundtoAGoal::DragFromBackgroundtoAGoal() {
  name = "Drag From Background to A";
  can_succeed = true;
}
// You don't get an E_DRAGGED event if you start in the background, so return an error if one comes in.
void DragFromBackgroundtoAGoal::event_handler(Event& e) {
  if(E_DRAGGED == e) can_succeed = false;
  if(E_RELEASE == e) {
    if(M5.background.contains(e.from) && !A.contains(e.from) && !B.contains(e.from) && A.contains(e.to)) success = can_succeed;
  }
}


// Drag from Background to B
//
DragFromBackgroundtoBGoal::DragFromBackgroundtoBGoal() {
  name = "Drag From Background to B";
  can_succeed = true;
}
// You don't get an E_DRAGGED event if you start in the background, so return an error if one comes in.
void DragFromBackgroundtoBGoal::event_handler(Event& e) {
  if(E_DRAGGED == e) can_succeed = false;
  if(E_RELEASE == e) {
    if(M5.background.contains(e.from) && !A.contains(e.from) && !B.contains(e.from) && B.contains(e.to)) success = can_succeed;
  }
}


// Swipe up detection
//
SwipeUpGoal::SwipeUpGoal() { name = "Swipe Up"; }
void SwipeUpGoal::event_handler(Event& e) {
  if((E_GESTURE == e) && (0 == strcmp("swipe up", e.gesture->getName()))) success = true;
}

// Swipe down detection
//
SwipeDownGoal::SwipeDownGoal() { name = "Swipe Down"; }
void SwipeDownGoal::event_handler(Event& e) {
  if((E_GESTURE == e) && (0 == strcmp("swipe down", e.gesture->getName()))) success = true;
}

// Swipe left detection
//
SwipeLeftGoal::SwipeLeftGoal() { name = "Swipe Left"; }
void SwipeLeftGoal::event_handler(Event& e) {
  if((E_GESTURE == e) && (0 == strcmp("swipe left", e.gesture->getName()))) success = true;
}

// Swipe right detection
//
SwipeRightGoal::SwipeRightGoal() { name = "Swipe Right"; }
void SwipeRightGoal::event_handler(Event& e) {
  if((E_GESTURE == e) && (0 == strcmp("swipe right", e.gesture->getName()))) success = true;
}
