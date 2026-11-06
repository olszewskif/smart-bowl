#include "../include/App.hpp"

App app{};

void setup() { 
  app.Init();
}

void loop() {
  app.Update();
  delay(10);
}
