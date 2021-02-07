#pragma once

#define TEXT_TOP        25
#define TEXT_CENTER     160
#define TEXT_HEIGHT     32
#define TEXT_FONT       4

#define TEST_DURRATION  8000

class Goal {
  public:
    Goal();
    bool          test();
    bool          passed();
    const char*   getName();
    virtual void  event_handler(Event& evt) = 0;
  protected:
    String        name;
    uint32_t      start_time;
    bool          success;
};

class TapAGoal                  : public Goal { public: TapAGoal();                  void event_handler(Event& e); };
class TapBGoal                  : public Goal { public: TapBGoal();                  void event_handler(Event& e); };
class LongPressAGoal            : public Goal { public: LongPressAGoal();            void event_handler(Event& e); };
class LongPressBGoal            : public Goal { public: LongPressBGoal();            void event_handler(Event& e); };
class LongPressBackgroundGoal   : public Goal { public: LongPressBackgroundGoal();   void event_handler(Event& e); };
class DoubleTapAGoal            : public Goal { public: DoubleTapAGoal();            void event_handler(Event& e); };
class DoubleTapBGoal            : public Goal { public: DoubleTapBGoal();            void event_handler(Event& e); };
class TapBackgroundGoal         : public Goal { public: TapBackgroundGoal();         void event_handler(Event& e); };
class DoubleTapBackgroundGoal   : public Goal { public: DoubleTapBackgroundGoal();   void event_handler(Event& e); };
class DragFromAtoBGoal          : public Goal { public: DragFromAtoBGoal();          void event_handler(Event& e); };
class DragFromBtoAGoal          : public Goal { public: DragFromBtoAGoal();          void event_handler(Event& e); };
class DragFromAtoBackgroundGoal : public Goal { public: DragFromAtoBackgroundGoal(); void event_handler(Event& e); };
class DragFromBtoBackgroundGoal : public Goal { public: DragFromBtoBackgroundGoal(); void event_handler(Event& e); };
class DragFromBackgroundtoAGoal : public Goal { public: DragFromBackgroundtoAGoal(); void event_handler(Event& e); private: bool can_succeed; };
class DragFromBackgroundtoBGoal : public Goal { public: DragFromBackgroundtoBGoal(); void event_handler(Event& e); private: bool can_succeed; };
class SwipeUpGoal               : public Goal { public: SwipeUpGoal();               void event_handler(Event& e); };
class SwipeDownGoal             : public Goal { public: SwipeDownGoal();             void event_handler(Event& e); };
class SwipeLeftGoal             : public Goal { public: SwipeLeftGoal();             void event_handler(Event& e); };
class SwipeRightGoal            : public Goal { public:  SwipeRightGoal();           void event_handler(Event& e); };
