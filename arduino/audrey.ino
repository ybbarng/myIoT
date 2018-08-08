#include <string.h>
#include <LGAircon.h>

LGAircon aircon;

void setup()
{
  Serial.begin(115200);
}

void logError(String message) {
  Serial.print("Error: ");
  Serial.println(message);
}

char DELEMETER[] = ":";
char* getNextToken(char *command) {
  return strtok(command, DELEMETER);
}

void manageAirConditioner(String command) {
  char commandChar[command.length()];
  command.toCharArray(commandChar, command.length());
  String token = String(getNextToken(commandChar));
  if (token == NULL) {
    return;
  } else if (token.startsWith("ON")) {
    aircon.turnOn();
  } else if (token.startsWith("OFF")) {
    aircon.turnOff();
  } else if (token.startsWith("NORMAL")) {
    token = String(getNextToken(NULL));
    Mode mode = Mode::on;
    if (token.startsWith("COOL")) {
      mode = Mode::cooling;
    } else if (token.startsWith("DEHU")) {
      mode = Mode::dehumidifying;
    } else {
      logError("Wrong mode");
    }
    token = String(getNextToken(NULL));
    unsigned int temperature = token.toInt();
    if (temperature < 18) {
      logError("Wrong temperature");
    }
    token = String(getNextToken(NULL));
    FanSpeed fanSpeed = FanSpeed::low;
    if (token.startsWith("LOW")) {
      fanSpeed = FanSpeed::low;
    } else if (token.startsWith("HIGH")) {
      fanSpeed = FanSpeed::high;
    } else {
      logError("Wrong fanSpeed");
    }
    aircon.sendCommandNormal(mode, temperature, fanSpeed);
  } else if (command.startsWith("TOGGLE")) {
    token = String(getNextToken(NULL));
    Toggle toggle = Toggle::powerSaving;
    if (token.startsWith("SAVE")) {
      toggle = Toggle::powerSaving;
    } else if (token.startsWith("POWER")) {
      toggle = Toggle::powerCooling;
    } else if (token.startsWith("SWING")) {
      toggle = Toggle::swing;
    } else {
      logError("Wrong toggle");
    }
    token = String(getNextToken(NULL));
    boolean on = false;
    if (token.startsWith("ON")) {
      on = true;
    } else if (token.startsWith("OFF")) {
      on = false;
    } else {
      logError("Wrong toggle option");
    }
    aircon.sendCommandToggle(toggle, on);
  } else if (command.startsWith("TIMER")) {
    token = String(getNextToken(NULL));
    unsigned int minutes = token.toInt();
    aircon.sendCommandTimer(minutes);
  }
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readString();
    Serial.println("Input: " + input);
    manageAirConditioner(input);
  }
}
