#include "public.h"


//BOOL PushMemNode(LIST_ENTRY* pListEntry, PVOID pAllocMem)
//{
//	struct MemoryQNode* qNode = (struct MemoryQNode*)RmHeapAlloc(sizeof(struct MemoryQNode));
//	if (qNode)
//	{
//		qNode->memoryAddress = pAllocMem;
//		InsertHeadList(pListEntry, &qNode->listEntry);
//	}
//}
//PVOID PopMemNode(LIST_ENTRY* pListEntry)
//{
//	PVOID retMemoryAddress = NULL;
//	PLIST_ENTRY listEntryPtr = NULL;
//	if (IsListEmpty(pListEntry) == FALSE)
//	{
//		listEntryPtr = RemoveTailList(pListEntry);
//		struct MemoryQNode* qNode = CONTAINING_RECORD(listEntryPtr, struct MemoryQNode, listEntry);
//		retMemoryAddress = qNode->memoryAddress;
//		RmHeapFree(qNode);
//	}
//	return retMemoryAddress;
//}
//BOOL PushProtoNode(LIST_ENTRY* pListEntry, struct ProtoQNode* pNod)
//{
//	struct ProtoQNode* qNode = (struct ProtoQNode*)RmHeapAlloc(sizeof(struct ProtoQNode));
//	if (qNode)
//	{
//		qNode->node = pNod;
//		InsertHeadList(pListEntry, &qNode->listEntry);
//		return TRUE;
//	}
//	return FALSE;
//}
//struct ProtoQNode* PopNode(LIST_ENTRY* pListEntry)
//{
//	struct ProtoQNode* retNode = NULL;
//	PLIST_ENTRY listEntryPtr = NULL;
//	if (IsListEmpty(pListEntry) == FALSE)
//	{
//		listEntryPtr = RemoveTailList(pListEntry);
//		struct ProtoQNode* qNode = CONTAINING_RECORD(listEntryPtr, struct ProtoQNode, listEntry);
//		retNode = qNode->node;
//		RmHeapFree(qNode);
//	}
//	return retNode;
//}