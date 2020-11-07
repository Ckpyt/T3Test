#include "Tools.h"
#include "CoreMinimal.h"


int LStream::sync() {
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(str().c_str()));
	str("");
	return std::stringbuf::sync();
}
