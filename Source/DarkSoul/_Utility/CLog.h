#pragma once
#include "CoreMinimal.h"
#include "DarkSoul/DarkSoul.h"

/** 사용자에게 필요한 정보를 출력한다. */
#define CLOG_FUNC					UE_LOG(LogDarkSoul, Warning, TEXT("Called : %s(%d)"), TEXT(__FUNCTION__), __LINE__);
#define CLOG_FUNC_TEXT(s)			UE_LOG(LogDarkSoul, Warning, TEXT("Called : %s(%d) : %s"), TEXT(__FUNCTION__), __LINE__, *FString(s));
#define CLOG_ERROR_FUNC				UE_LOG(LogDarkSoul, Error, TEXT("Called : %s(%d)"), TEXT(__FUNCTION__), __LINE__);
#define CLOG_ERROR_FUNC_TEXT(s)		UE_LOG(LogDarkSoul, Error, TEXT("Called : %s(%d) : %s"), TEXT(__FUNCTION__), __LINE__, *FString(s));

/** 치명적인 오류 검사 및 출력한다. */
#define CLOG_ERROR_CHECK(p)\
{\
	if (p == nullptr)\
	{\
		CLOG_ERROR_FUNC_TEXT("NULL")\
	}\
}
#define CLOG_ERROR_CHECK_TEXT(p, s)\
{\
	if (p == nullptr)\
	{\
		CLOG_ERROR_FUNC_TEXT(s)\
	}\
}

/** 위험한 nullptr 검사 및 출력한다. */
#define CLOG_CHECK(p)\
{\
	if(p == nullptr)\
	{\
		CLOG_FUNC_TEXT("NULL");\
	}\
}
#define CLOG_CHECK_TEXT(p, s)\
{\
	if(p == nullptr)\
	{\
		CLOG_FUNC_TEXT(s);\
	}\
}

/**
 * 위험한 nullptr 검사 및 출력, 마지막으로 함수를 종료 시킨다.
 * 사용되는 함수에서 반환 자료형이 존재하면, 사용할 수 없다.
 * 반환형이 void인 함수에서만 사용할 수 있다.
 */
#define CLOG_CHECK_RETURN(p)\
{\
	if(p == nullptr)\
	{\
		CLOG_FUNC_TEXT("NULL");\
		return;\
	}\
}
#define CLOG_CHECK_RETURN_TEXT(p, s)\
{\
	if(p == nullptr)\
	{\
		CLOG_FUNC_TEXT(s);\
		return;\
	}\
}
#define CLOG_ERROR_CHECK_RETURN(p)\
{\
	if(p == nullptr)\
	{\
		CLOG_ERROR_FUNC_TEXT("NULL");\
		return;\
	}\
}
#define CLOG_ERROR_CHECK_RETURN_TEXT(p, s)\
{\
	if(p == nullptr)\
	{\
		CLOG_ERROR_FUNC_TEXT(s);\
		return;\
	}\
}

/** Log 출력과 함께 void return을 수행한다. */
#define CLOG_RETURN_TEXT(s)\
{\
	CLOG_FUNC_TEXT(s);\
	return;\
}\

#define CLOG_ERROR_RETURN_TEXT(s)\
{\
	CLOG_ERROR_FUNC_TEXT(s);\
	return;\
}\

class DARKSOUL_API CLog
{
public:
	CLog();
	~CLog();

	static void Log(int val);
	static void Log(float val);
	static void Log(UINT val);
	static void Log(FVector2D val);
	static void Log(const FString& val);
	static void Log(const TCHAR* val);
	static void Log(const FName& val);
	static void LogNull(UObject* object);

	static void Print(bool val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(int val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(float val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(UINT val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(FVector val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(FVector2D val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(FRotator val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(FQuat val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const FString& val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const TCHAR* val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void Print(const FName& val, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
	static void PrintNull(UObject* object, float time = 10.0f, FColor color = FColor::Blue, int32 key = -1);
};