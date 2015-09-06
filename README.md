% gwp-steuerung-osmose
% Steuerung für Osmose-Anlage des Günztal Waschparks
% 06.09.2015

# Ausgänge

Die Platine steuert folgende Einheiten an den entsprechenden PINs:

```c
/** Pumpe */
const int pumpPin = 8;
/** Magnetventil */
const int mValvePin = 9;
/** Spülventil */
const int rinseValvePin = 10;
```

### Pumpe

### Magnetventil

### Spülventil

# Eingänge

Vier Schalter werden verwendet um die Ausgänge zu steuern.

```c
/** Tank ist leer */
const int emptyContainerPin = 2;
/** Tank ist voll */
const int fullContainerPin = 3;
/** Druckschalter */
const int pressureButtonPin = 4;
/** Startknopf */
const int startButtonPin = 5;
```

### Tank ist leer

### Tank ist voll

### Druckschalter

### Startknopf

# Zustände

Die Steuerung kann sich in folgenden Zuständen befinden: 

```c
enum Event {
  /** Unbeschäftigt */
  NONE, 
  /** Vorbereiten des Füllvorgangs (Spülen) */
  SETUP_FILL, 
  /** Auffüllen des Tanks */
  FILL, 
  /** Nachbereiten des Füllvorgangs (Spülen) */
  RINSE, 
  /** Fehlerzustand */
  ERR
};
```

### Unbeschäftigt

### Vorbereitung Füllen

### Auffüllen des Tanks

### Nachbereiten des Füllvorgans

### Fehlerzustand