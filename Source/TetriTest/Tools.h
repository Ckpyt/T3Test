#pragma once

#include <sstream>
#include <vector>

class LStream : public std::stringbuf {
protected:
	int sync();
};

UENUM()
enum struct GunMode {
	push UMETA(DisplayName = "push"),
	rotate UMETA(DisplayName = "rotate"),
	destroy UMETA(DisplayName = "destroy")
};

