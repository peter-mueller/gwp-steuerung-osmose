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

#### Pumpe

Wasser wird von der Hochdruckpumpe angesaugt und durch das Membranmodul (Filter) gedrückt.

#### Magnetventil

Mit öffnen des Magnetventils wird der Wasserzulauf geöffnet. 

#### Spülventil

Die Filter werden gespült.

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

#### Tank ist leer

Wenn der Vorratstank für Osmosewasser leer ist **schließt** dieser Schalter. 

#### Tank ist voll

Wenn der Vorratstank für Osmosewasser voll ist **öffnet** dieser Schalter.

#### Druckschalter

Der Druckschalter schließt, wenn der Wassereingangsdruck in Ordnung ist.

#### Startknopf

Beim schließen dieses Schalters wird der Füllvorgang gestartet, sofern der Tank nicht voll ist.

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

#### Unbeschäftigt

Weder die Pumpe wird noch gestartet, noch ein Ventil wird geöffnet.

#### Vorbereitung Füllen

Über einen Zeitraum von zwei Minuten wird der Füllvorgang vorbereitet. Dazu wird neben der Pumpe und dem Magnetventil auch das Spülventil aktiviert.

#### Auffüllen des Tanks

Die Pumpe läuft und das Magnetventil ist geöffnet.

#### Nachbereiten des Füllvorgans

Über einen Zeitraum von drei Minuten wird der Füllvorgang nachbereitet. Dazu wird neben der Pumpe und dem Magnetventil auch das Spülventil aktiviert.

#### Fehlerzustand

Falls die Schalter `Tank leer` und `Tank voll` gleichzeitig melden oder der Druckschalter nicht beim Pumpen aktiviert ist wird in einen blockierenden Fehlerzustand gewechselt.

Dabei sind die Pumpe sowie alle Ventile deaktiviert.
