#include "Systems/RNGService.h"

void URNGService::InitializeRun(int32 BaseSeed)
{
	RunBaseSeed = BaseSeed;
	NameToStream.Empty();
}

FRandomStream& URNGService::GetStream(FName StreamName)
{
	if (FRandomStream* Found = NameToStream.Find(StreamName))
	{
		return *Found;
	}
	// Derive a stream seed from base seed and stream name hash
	const uint32 NameHash = GetTypeHash(StreamName);
	const int32 DerivedSeed = (int32)((uint32)RunBaseSeed ^ NameHash * 2654435761u);
	FRandomStream& NewStream = NameToStream.Add(StreamName);
	NewStream.Initialize(DerivedSeed);
	return NameToStream[StreamName];
}

int32 URNGService::RandRange(FName StreamName, int32 Min, int32 Max)
{
	return GetStream(StreamName).RandRange(Min, Max);
}
