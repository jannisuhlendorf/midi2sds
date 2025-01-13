#include <SPI.h>
#include <MIDI.h>

#define DAC1 8
#define DAC2 9
#define GATE_LENGTH 5
#define MIDI_CHANNEL 12
#define CHANNELS 4

MIDI_CREATE_DEFAULT_INSTANCE();

int channelDac[] = {DAC1, DAC1, DAC2, DAC2};
int channelAddress[] = {0, 1, 0, 1};
int channelNote[] = {0, 2, 4, 5}; // C D E F

unsigned long triggerTimes[CHANNELS];
bool triggerOn[CHANNELS];



void setup() {
  for (int i=0; i<CHANNELS; i++) {
    triggerTimes[i] = 0;
    triggerOn[i] = false;
  }

  MIDI.begin(MIDI_CHANNEL_OMNI);
  pinMode(DAC1, OUTPUT);
  pinMode(DAC2, OUTPUT);
  digitalWrite(DAC1,HIGH);
  digitalWrite(DAC2,HIGH);
  SPI.begin();
}

void loop() {
  int velocity, note, dac, channel;
  if (MIDI.read()) { 
    byte type = MIDI.getType();
    channel = MIDI.getChannel();
    if ((type == midi::NoteOn) && (channel == MIDI_CHANNEL)) {
        note = MIDI.getData1() % 12; // 0=C 2=D 4=
        velocity = MIDI.getData2();

        for (int i=0; i<CHANNELS; i++) {
          if ((note == channelNote[i]) && (!triggerOn[i])) {
            triggerTimes[i] = millis();
            triggerOn[i] = true;;
            setVoltage(channelDac[i], channelAddress[i], 1, velocity<<5);
          }
        }
    }
  }

  unsigned long now = millis();
  for (int i=0; i<CHANNELS; i++) {
    if ((triggerOn[i]) && (now > (triggerTimes[i] + GATE_LENGTH))) {
      triggerOn[i] = false;
      setVoltage(channelDac[i], channelAddress[i], 0, 0);
    }
  }
}

// setVoltage -- Set DAC voltage output
// dacpin: chip select pin for DAC.  Note and velocity on DAC1, pitch bend and CC on DAC2
// channel: 0 (A) or 1 (B).  Note and pitch bend on 0, velocity and CC on 2.
// gain: 0 = 1X, 1 = 2X.  
// mV: integer 0 to 4095.  If gain is 1X, mV is in units of half mV (i.e., 0 to 2048 mV).
// If gain is 2X, mV is in units of mV

void setVoltage(int dacpin, bool channel, bool gain, unsigned int mV)
{
  unsigned int command = channel ? 0x9000 : 0x1000;

  command |= gain ? 0x0000 : 0x2000;
  command |= (mV & 0x0FFF);
  
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(dacpin,LOW);
  SPI.transfer(command>>8);
  SPI.transfer(command&0xFF);
  digitalWrite(dacpin,HIGH);
  SPI.endTransaction();
}
