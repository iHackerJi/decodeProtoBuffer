#pragma once

struct ProtoNode
{
	union
	{
		ANSI_STRING unistrBuffer;
		uint64_t u64Buffer;
		void* structBuffrePtr;
	}u;
	enum protoType nodetype;
	std::vector<struct ProtoNode> vSubNode;
};

struct ErgodicNode
{
	char* Buffer;
	uint64_t Size;
};

extern std::vector<void*> vMemoryFree;
extern std::queue<struct ErgodicNode> vErgodic;
extern std::queue<std::vector<struct ProtoNode>*> vErgodicP;
extern struct ProtoTree ProtoNodeTree;



struct ProtoTree
{
	std::vector<struct ProtoNode> vNodeRoot;
};

