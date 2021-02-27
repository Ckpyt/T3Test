#pragma once

#include <sstream>
#include <vector>

UENUM(BlueprintType)
	enum class EGunMode : uint8 {
	default = 0		UMETA(DisplayName = "default"),
	push			UMETA(DisplayName = "push"),
	rotate			UMETA(DisplayName = "rotate"),
	destroy			UMETA(DisplayName = "destroy"),
	pull			UMETA(DisplayName = "pull"),
	rotateCounter	UMETA(DisplayName = "rotateCounter"),
	destroyFigure	UMETA(DisplayName = "destroyFigure")
};

UENUM(BlueprintType)
	enum class EBlockSides : uint8 {
	default = 0		UMETA(DisplayName = "default"),
	plusX = 1		UMETA(DisplayName = "plusX"),
	minusX = 255	UMETA(DisplayName = "minusX"),
	plusY = 2		UMETA(DisplayName = "plusY"),
	minusY = 254	UMETA(DisplayName = "minusY"),
	plusZ = 3		UMETA(DisplayName = "plusZ"),
	minusZ = 253	UMETA(DisplayName = "minusZ")
};

EBlockSides OtherSide(EBlockSides side);
EGunMode AlternativeMode(EGunMode mode);
EGunMode IntToMode(int mode);

