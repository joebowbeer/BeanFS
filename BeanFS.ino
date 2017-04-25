// LightBlue Bean or Bean+

#define CHANNEL CHANNEL10
#define RNG_CONTROLLER GENERALPURPOSECONTROLLER6
#define TIP_CONTROLLER GENERALPURPOSECONTROLLER5

// TRS jack breakout connected to pins D0..D5
#define TIP_PIN 0
#define RNG_PIN 2
#define SLV_PIN 4

bool connected;

int rngInitState;
int rngLastState;
int tipInitState;
int tipLastState;

void setup() {
  // Configure tip and ring pins as digital input with pull-up resistor
  pinMode(RNG_PIN, INPUT_PULLUP);
  pinMode(TIP_PIN, INPUT_PULLUP);

  // Configure sleave pin as ground
  pinMode(SLV_PIN, OUTPUT);
  digitalWrite(SLV_PIN, LOW);

  connected = false;

  // LED is blue until we're connected
  Bean.setLedBlue(255);

  BeanMidi.enable();
}

void loop() {
  // connection
  if (Bean.getConnectionState() != connected) {
    connected = !connected;
    if (connected) {
      // Read initial pin states
      rngInitState = digitalRead(RNG_PIN);
      rngLastState = rngInitState;
      tipInitState = digitalRead(TIP_PIN);
      tipLastState = tipInitState;
      Bean.setLedBlue(0);
    } else {
      Bean.setLedBlue(255);
    }
  }
  if (!connected) {
    return;
  }
  // ring
  int rngState = digitalRead(RNG_PIN);
  if (rngState != rngLastState) {
    rngLastState = rngState;
    if (rngState != rngInitState) {
      Bean.setLedRed(255);
      BeanMidi.sendMessage(CONTROLCHANGE + CHANNEL, RNG_CONTROLLER, 127);
    } else {
      Bean.setLedRed(0);
      BeanMidi.sendMessage(CONTROLCHANGE + CHANNEL, RNG_CONTROLLER, 0);
    }
  }
  // tip
  int tipState = digitalRead(TIP_PIN);
  if (tipState != tipLastState) {
    tipLastState = tipState;
    if (tipState != tipInitState) {
      Bean.setLedGreen(255);
      BeanMidi.sendMessage(CONTROLCHANGE + CHANNEL, TIP_CONTROLLER, 127);
    } else {
      Bean.setLedGreen(0);
      BeanMidi.sendMessage(CONTROLCHANGE + CHANNEL, TIP_CONTROLLER, 0);
    }
  }
}
