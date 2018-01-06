#include <TimeLib.h>

#define EIGHT_AM 28800
#define FOUR_PM 57600
#define SEVEN_PM 68400
#define SECONDS_IN_DAY 86400

// IR DEFINITIONS
int IRledPin = 13;           // Pin for IR LED

uint16_t IRsignalOff[] = 
{
  // ON, OFF 
  9400, 4640,
  640, 520,
  640, 540,
  520, 640,
  540, 620,
  560, 600,
  560, 620,
  580, 580,
  640, 540,
  520, 1800,
  600, 1740,
  600, 1740,
  540, 1800,
  640, 520,
  540, 1800,
  580, 1760,
  620, 1700,
  560, 620,
  560, 1780,
  540, 640,
  540, 600,
  600, 580,
  540, 620,
  600, 560,
  620, 560,
  540, 1780,
  600, 580,
  540, 1780,
  600, 1760,
  580, 1740,
  560, 1780,
  580, 1760,
  580, 1760,
  540, 42720,
  9360, 2320,
  600, 0
};

uint16_t IRsignalOn[] = 
{
  9360, 4700,
  600, 580,
  540, 640,
  580, 560,
  620, 560,
  540, 640,
  540, 600,
  600, 580,
  540, 640,
  540, 1780,
  540, 1800,
  600, 1740,
  580, 1760,
  540, 640,
  580, 1740,
  540, 1780,
  600, 1760,
  580, 1760,
  540, 1780,
  600, 580,
  540, 640,
  540, 600,
  620, 560,
  540, 640,
  540, 600,
  620, 560,
  540, 640,
  540, 1780,
  540, 1800,
  600, 1760,
  580, 1740,
  540, 1800,
  580, 1760,
  580, 42660,
  9420, 2320,
  540, 34440,
  9400, 2360,
  540, 0
};

uint16_t IRsignal10[] = 
{
  9460, 4620,
  660, 520,
  540, 640,
  540, 600,
  640, 540,
  620, 560,
  600, 540,
  580, 600,
  540, 640,
  600, 1720,
  560, 1780,
  620, 1720,
  560, 1780,
  540, 640,
  600, 1720,
  540, 1800,
  620, 1720,
  560, 1780,
  540, 640,
  600, 1720,
  540, 640,
  540, 1780,
  540, 640,
  620, 540,
  580, 580,
  560, 620,
  620, 1720,
  540, 640,
  600, 1720,
  540, 640,
  600, 1720,
  540, 1800,
  620, 1720,
  560, 42720,
  9420, 2300,
  540, 0
};
uint16_t IRsignal100[] = 
{
  9400, 4640,
  620, 560,
  560, 620,
  600, 560,
  600, 580,
  540, 640,
  560, 580,
  620, 560,
  540, 640,
  580, 1740,
  560, 1780,
  600, 1760,
  640, 1680,
  540, 640,
  580, 1740,
  560, 1780,
  600, 1760,
  620, 540,
  640, 540,
  520, 1800,
  600, 580,
  540, 640,
  620, 520,
  600, 580,
  540, 640,
  620, 1720,
  540, 1780,
  600, 580,
  560, 1780,
  600, 1760,
  620, 1700,
  540, 1800,
  640, 1700,
  620, 42640,
  944, 228,
  540, 0
};

int signalSizes[] =
{
  sizeof(IRsignalOff) / 2,
  sizeof(IRsignalOn) / 2,
  sizeof(IRsignal10) / 2,
  sizeof(IRsignal100) / 2,
};

// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs)
{
  // we'll count down from the number of microseconds we are told to wait

  cli();  // this turns off any background interrupts

  while (microsecs > 0)
  {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
    digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
    delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
    digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
    delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working

    // so 26 microseconds altogether
    microsecs -= 26;
  }

  sei();  // this turns them back on
}

uint16_t* getSignal(int mode)
{
  if (mode == 0)
  {
    return IRsignalOff;
  } else if (mode == 1)
  {
    return IRsignalOn;
  } else if (mode == 2)
  {
    return IRsignal10;
  } else {
    return IRsignal100;
  }
};

uint32_t getDaySeconds()
{
  return ((uint32_t) ((uint32_t) hour() * 3600) + ((uint32_t) minute() * 60) + (uint32_t) second());
}

void SendIRCode(int mode)
{
  uint16_t* IRsignal = getSignal(mode);
  int arraySize = signalSizes[mode];
  for (int k = 0; k < 3; k++)
  {
    for (int i = 0; i < arraySize; i += 2)
    {
      pulseIR(IRsignal[i]);
      delayMicroseconds(IRsignal[i + 1]);
    }
  }
}

void setup(void)
{
  pinMode(IRledPin, OUTPUT);      

  Serial.begin(9600);                           // Begin serial data transfer
  Serial.println("Lighting cycles begun");
  setTime(17, 0, 30, 6, 1, 2018);
}

void loop(void)
{
  int mode;
  uint32_t daySeconds = getDaySeconds();
  uint32_t nextInterval;
  if (daySeconds <= EIGHT_AM)
  {
    mode = 2;
    nextInterval = EIGHT_AM - daySeconds;
  } else if (daySeconds <= FOUR_PM)
  {
    mode = 0;
    nextInterval = FOUR_PM - daySeconds;
  } else if (daySeconds <= SEVEN_PM) {
    mode = 2;
    nextInterval = SEVEN_PM - daySeconds;
  }
  else {
    mode = 3;
    nextInterval = SECONDS_IN_DAY - daySeconds;
  }
  if (mode != 0)
  {
    SendIRCode(1);
  }
  SendIRCode(mode);
  Serial.print("Mode: ");
  Serial.println(mode);
  Serial.print("Day Seconds: ");
  Serial.println(daySeconds);
  Serial.print("Next Interval: ");
  Serial.println(nextInterval);
  delay(nextInterval * 1000);
}
