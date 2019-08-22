#ifndef GLOBALDEFINES_H
#define GLOBALDEFINES_H

const unsigned PlayerSpeedLimit = 970; // максимально допустимая скорость игрока
const unsigned PlayerIncreaseSpeedMax = 660; // максимально допустимая скорость ускорения игрока
const unsigned PlayerDecreaseSpeedMax = 820; // максимально допустимая скорость ускорения игрока
const double HitAngleKoeffFromHead = 0.5;
const double HitAngleKoeffFromChest = 0.2;
const double HitAngleMax = 2*3.14159265359;

const double RotationAngleMax = 6*3.14159265359;

const unsigned MaxHeightForFoot = 100;
const unsigned MaxHeightForChest = 150;
const unsigned MaxHeightForHead = 220;
const unsigned MaxHeightForBallSituation = 270;

const double Gravitation = 980;
const double Windage = 4;
const double GroundFriction = 14;

#endif // GLOBALDEFINES_H
