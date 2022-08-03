#pragma once
extern "C"
{
	NTSYSAPI NTSTATUS RtlIntegerToUnicodeString(
		LONG Value,
		ULONG Base,
		PUNICODE_STRING String
	);

	NTSYSAPI NTSTATUS RtlInt64ToUnicodeString(
		ULONGLONG Value,
		ULONG Base,
		PUNICODE_STRING String
	);

	//NTSYSAPI VOID RtlInitUnicodeString(
	//	PUNICODE_STRING         DestinationString,
	//	 __drv_aliasesMem PCWSTR SourceString
	//);
}
