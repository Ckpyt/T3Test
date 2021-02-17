#include "Tools.h"
#include "CoreMinimal.h"


blockSides OtherSide(blockSides side) {
	char intSide = static_cast<char>(side);
	return static_cast<blockSides>(-intSide);
}
