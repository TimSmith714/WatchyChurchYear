#include "Watchy_ChurchYear.h"
#include "settings.h"

 WatchyChurchYear watchy(settings);

void setup() {
  watchy.init(); // call init in setup
}

void loop() {
  // this should never run, Watch deep sleeps after init();
}
