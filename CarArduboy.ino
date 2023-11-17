#include <Arduboy2.h>
#include "Bitmaps.h"

Arduboy2 arduboy;

#define ANZ_CARS 4

char Cars[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char carSpeed = 1;

char bikeX = 64;
char bikeY = 32;
char bikeSpeed = 2;
long score = 0;

bool isGameover = false;

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(40);

  for (byte i = 1; i <= ANZ_CARS; i++)
  {
    setupCar(i);
  }
}

void loop()
{

  if (!(arduboy.nextFrame()))
    return;

  if (arduboy.everyXFrames(5))
  {
    score += carSpeed;
  }

  if (arduboy.pressed(RIGHT_BUTTON))
  {

    bikeX += bikeSpeed;
    if (bikeX > 121)
      bikeX = 121;
  }

  if (arduboy.pressed(LEFT_BUTTON))
  {
    bikeX -= bikeSpeed;
    if (bikeX < 0)
      bikeX = 0;
  }

  if (arduboy.pressed(UP_BUTTON))
  {
    bikeY -= bikeSpeed;
    if (bikeY < 0)
      bikeY = 0;
  }

  if (arduboy.pressed(DOWN_BUTTON))
  {
    bikeY += bikeSpeed;
    if (bikeY > 54)
      bikeY = 54;
  }

  if (arduboy.pressed(A_BUTTON))
  {
    if (!isGameover and arduboy.everyXFrames(5))
    {
      if (carSpeed == 2)
      {
        carSpeed = 1;
      }
      else
      {
        carSpeed = 2;
      }
    }
  }

  if (arduboy.pressed(B_BUTTON))
  {
    restartGame();
  }
  moveCars(carSpeed);
  isGameover = isBikeOverlapping();

  arduboy.clear();

  drawCars();
  arduboy.drawSlowXYBitmap(bikeX, bikeY, BIKE, 7, 16, WHITE);
  arduboy.drawLine(0, 0, 0, 63, WHITE);
  arduboy.drawLine(127, 0, 127, 63, WHITE);

  arduboy.setCursor(0, 0);
  arduboy.print(score);

  arduboy.setCursor(80, 0);
  arduboy.print("Speed ");
  arduboy.print((byte)carSpeed);
  // arduboy.print(arduboy.cpuLoad());
  // arduboy.print(isBikeOverlapping());

  if (isGameover)
    gameover();

  arduboy.display();
}

void drawCars()
{
  for (byte i = 0; i < ANZ_CARS * 2; i += 2)
  {
    arduboy.drawSlowXYBitmap(Cars[i], Cars[i + 1], CAR, 16, 32, WHITE);
  }
}

void moveCars(byte pPixel)
{
  for (byte i = 1; i < ANZ_CARS * 2; i += 2)
  {
    if (Cars[i] > 64)
    {
      // Cars[i] = -32;
      setupCar((i + 1) / 2);
    }
    else
    {
      Cars[i] += pPixel;
    }
  }
}

void setupCar(byte pID)
{
  byte x = (pID * 2) - 2;
  byte y = (pID * 2) - 1;

  do
  {
    Cars[x] = random(112);
    Cars[y] = -1 * 32 - random(40);
  } while (isCarOverlapping(pID));
}

bool isCarOverlapping(byte pID)
{
  char x = Cars[(pID * 2) - 2];
  char y = Cars[(pID * 2) - 1];
  bool overlapp = false;

  for (byte i = 0; i < ANZ_CARS * 2; i += 2)
  {
    if ((Cars[i] + 16 > x and x + 16 > Cars[i] and Cars[i + 1] + 32 > y and y + 32 > Cars[i + 1]) and !(x == Cars[i] and y == Cars[i + 1]))
    {
      overlapp = true;
    }
  }
  return overlapp;
}

bool isBikeOverlapping()
{
  bool overlapp = false;
  // bikeX, bikeY

  for (byte i = 0; i < ANZ_CARS * 2; i += 2)
  {
    if (Cars[i] + 16 > bikeX and bikeX + 7 > Cars[i] and Cars[i + 1] + 32 > bikeY and bikeY + 16 > Cars[i + 1])
    {
      overlapp = true;
    }
  }
  return overlapp;
}

void gameover()
{
  bikeSpeed = 0;
  carSpeed = 0;

  arduboy.setCursor(40, 29);
  arduboy.print("Gameover");
}

void restartGame()
{
  bikeX = 64;
  bikeY = 40;
  bikeSpeed = 2;
  carSpeed = 1;
  score = 0;

  for (byte i = 1; i <= ANZ_CARS; i++)
  {
    setupCar(i);
  }

  isGameover = false;
}
