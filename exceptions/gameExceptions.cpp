#include "gameExceptions.h"

NoAbilityException::NoAbilityException() : GameException("No ability available to use") {}

ShipPlacementException::ShipPlacementException() : GameException("Invalid ship placement") {}

OutOfFieldException::OutOfFieldException() : GameException("Coordinates are out of field bounds") {}