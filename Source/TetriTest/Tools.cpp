#include "Tools.h"
#include "CoreMinimal.h"


EBlockSides OtherSide(EBlockSides side) {
	char intSide = static_cast<char>(side);
	return static_cast<EBlockSides>(-intSide);
}

EGunMode AlternativeMode(EGunMode mode) {
	switch (mode) {
	case EGunMode::destroy: return EGunMode::destroyFigure;
	case EGunMode::destroyFigure: return EGunMode::destroy;
	case EGunMode::pull: return EGunMode::push;
	case EGunMode::push: return EGunMode::pull;
	case EGunMode::rotate: return EGunMode::rotateCounter;
	case EGunMode::rotateCounter: return EGunMode::rotate;
	}
	return EGunMode::pull;
}

EGunMode IntToMode(int mode) {
	switch (mode) {
	case 0: return EGunMode::push;
	case 1: return EGunMode::rotate;
	case 2: return EGunMode::destroy;
	}
	return EGunMode::push;
}

