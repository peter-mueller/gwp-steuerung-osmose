
enum Event {
  NONE, SETUP_FILL, FILL, RINSE, ERR
};

const int emptyContainerPin = 2;
const int fullContainerPin = 3;
const int pressureButtonPin = 4;
const int startButtonPin = 5;

const int pumpPin = 8;
const int mValvePin = 9;
const int rinseValvePin = 10;

const unsigned long SETUP_TIME = 120000;
const unsigned long RINSE_TIME = 180000;

Event event = NONE;
unsigned long startTime = 0;

void setup() {
  Serial.begin(9600);


  pinMode(emptyContainerPin, INPUT_PULLUP); // LOW means is emtpy
  pinMode(fullContainerPin, INPUT_PULLUP); // HIGH means is full
  pinMode(pressureButtonPin, INPUT_PULLUP); // LOW means pressure is sensed
  pinMode(startButtonPin, INPUT_PULLUP); // LOW means should start

  pinMode(pumpPin, OUTPUT);
  pinMode(mValvePin, OUTPUT);
  pinMode(rinseValvePin, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(13, LOW);
  digitalWrite(pumpPin, HIGH);
  digitalWrite(mValvePin, HIGH);
  digitalWrite(rinseValvePin, HIGH);
}



void loop() {
  /*
     Find out what to do.
  */
  if (digitalRead(emptyContainerPin) == LOW && digitalRead(fullContainerPin) == HIGH) {  // can't be empty and full
    Serial.println("WARN: Tank leer und voll!");
    delay(500);
    if (digitalRead(emptyContainerPin) == LOW && digitalRead(fullContainerPin) == HIGH) {
      event = ERR;
      Serial.println("ERR: Tank leer und voll!");
    }
  }

  if (event == FILL || event == SETUP_FILL || event == RINSE) {
    if (digitalRead(pressureButtonPin)) {
      Serial.println("Achtung, druckschalter nicht da.");
      delay(10000);
      if (digitalRead(pressureButtonPin)) {
        event = ERR;
        Serial.println("ERR: Druckschalter länger als 10 Sekunden aus!");
      }
    }
  }

  if (event == NONE) {
    if (!digitalRead(startButtonPin) && !digitalRead(fullContainerPin)) {
      event = SETUP_FILL;
      Serial.println("SETUP_FILL: Start Knopf gedrückt und Tank nicht voll!");
      startTime = 0;
    } else if (!digitalRead(emptyContainerPin)) {
      Serial.println("*possible*:  SETUP_FILL: Tank leer!");
      delay(500);
      if (!digitalRead(emptyContainerPin)) {
        event = SETUP_FILL;
        startTime = 0;
        Serial.println("SETUP_FILL: Tank leer!");
      }
    }
  }



  /*
     Init the wished Event.
  */
  if (event == SETUP_FILL) {

    if (startTime == 0) {
      startTime = millis();
      digitalWrite(pumpPin, LOW);
      digitalWrite(mValvePin, LOW);
      digitalWrite(rinseValvePin, LOW);
    } else if (millis() - startTime > SETUP_TIME) {
      event = FILL;
      Serial.println("FILL: Vorbereiten (2 Minuten) erfolgreich!");
      startTime = 0;
    }
  }

  if (event == FILL) {
    if (startTime == 0) {
      startTime = millis();
      digitalWrite(pumpPin, LOW);
      digitalWrite(mValvePin, LOW);
      digitalWrite(rinseValvePin, HIGH);
    } else if (digitalRead(fullContainerPin)) {
      event = RINSE;
      Serial.println("RINSE: Tank voll!");
      startTime = 0;
    }
  }

  if (event == RINSE) {
    if (startTime == 0) {
      startTime = millis();
      digitalWrite(pumpPin, LOW);
      digitalWrite(mValvePin, LOW);
      digitalWrite(rinseValvePin, LOW);
    } else if (millis() - startTime > RINSE_TIME) {
      event = NONE;
      Serial.println("NONE: Nachbereiten fertig!");
      startTime = 0;
    }
  }

  if (event == NONE) {
    if (startTime == 0) {
      startTime = millis();
      digitalWrite(pumpPin, HIGH);
      digitalWrite(mValvePin, HIGH);
      digitalWrite(rinseValvePin, HIGH);
    }
  }

  if (event == ERR) {
    Serial.println("ERR!");
    digitalWrite(pumpPin, HIGH);
    digitalWrite(mValvePin, HIGH);
    digitalWrite(rinseValvePin, HIGH);

    digitalWrite(13, HIGH);

    while (true);
  }
}
