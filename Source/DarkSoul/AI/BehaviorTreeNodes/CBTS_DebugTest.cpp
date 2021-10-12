// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTS_DebugTest.h"
#include "DarkSoul/_Utility/CLog.h"

void UCBTS_DebugTest::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	CLOG_FUNC;
}
