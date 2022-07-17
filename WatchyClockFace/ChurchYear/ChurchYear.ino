#include <Watchy.h>
#include "settings.h"
#include "Fonts/DSEG7_Classic_Regular_39.h"

class ChurchYearWatchFace : public Watchy{ // inherit and extend Watchy class
  public:
    MyFirstWatchFace(const settting& s) : Watchy(s) {}
    void drawWatchFace(){ // override this method to customise how the watch face looks
      display.setFont(&DSEG7_Classic_Regular_39);
      display.setCursor(25,100);
      if(currentTime.Hour < 10 { // use the currentTime struct to print latest time
        display.print("0");
      }
      display.print(currentTime.Hour);
      display.print(":");
      if(currentTime.Minute < 10){
        display.print("0");
      }
      display.println(currentTime.Minute);
    }
};

ChurchYearWatchFace m(settings); //instantiate your watchface

void setup() {
  m.init(); // call init in setup
}

void loop() {
  // this should never run, Watch deep sleeps after init();
}
