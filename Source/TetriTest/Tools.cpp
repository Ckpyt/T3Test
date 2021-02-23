#include "Tools.h"
#include "CoreMinimal.h"


blockSides OtherSide(blockSides side) {
	char intSide = static_cast<char>(side);
	return static_cast<blockSides>(-intSide);
}

GunMode AlternativeMode(GunMode mode) {
	switch (mode) {
	case GunMode::destroy: return GunMode::destroyFigure;
	case GunMode::destroyFigure: return GunMode::destroy;
	case GunMode::pull: return GunMode::push;
	case GunMode::push: return GunMode::pull;
	case GunMode::rotate: return GunMode::rotateCounter;
	case GunMode::rotateCounter: return GunMode::rotate;
	}
	return GunMode::pull;
}

GunMode IntToMode(int mode) {
	switch (mode){
	case 0: return GunMode::push;
	case 1: return GunMode::rotate;
	case 2: return GunMode::destroy;
	}
	return GunMode::push;
}