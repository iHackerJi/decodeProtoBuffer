#include "public.h"

int varint_read_u(const void* data, size_t len, uint64_t* x) {
	const char* str = (const char*)data;
	uint64_t b;
	*x = 0;
	if (len == 0) return 0; b = str[0]; *x |= (b & 0x7f) << (7 * 0); if (b < 0x80) return 0 + 1;
	if (len == 1) return 0; b = str[1]; *x |= (b & 0x7f) << (7 * 1); if (b < 0x80) return 1 + 1;
	if (len == 2) return 0; b = str[2]; *x |= (b & 0x7f) << (7 * 2); if (b < 0x80) return 2 + 1;
	if (len == 3) return 0; b = str[3]; *x |= (b & 0x7f) << (7 * 3); if (b < 0x80) return 3 + 1;
	if (len == 4) return 0; b = str[4]; *x |= (b & 0x7f) << (7 * 4); if (b < 0x80) return 4 + 1;
	if (len == 5) return 0; b = str[5]; *x |= (b & 0x7f) << (7 * 5); if (b < 0x80) return 5 + 1;
	if (len == 6) return 0; b = str[6]; *x |= (b & 0x7f) << (7 * 6); if (b < 0x80) return 6 + 1;
	if (len == 7) return 0; b = str[7]; *x |= (b & 0x7f) << (7 * 7); if (b < 0x80) return 7 + 1;
	if (len == 8) return 0; b = str[8]; *x |= (b & 0x7f) << (7 * 8); if (b < 0x80) return 8 + 1;
	if (len == 9) return 0; b = str[9]; *x |= (b & 0x7f) << (7 * 9); if (b < 0x80) return 9 + 1;
	return -1;
}

int varint_read_i(const void* data, size_t len, int64_t* x) {
	uint64_t ux;
	int n = varint_read_u(data, len, &ux);
	*x = (int64_t)(ux >> 1);
	if ((ux & 1) != 0) {
		*x = ~*x;
	}
	return n;
}

bool is_str_utf8(const char* str)
{
	unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节  
	unsigned char chr = *str;
	bool bAllAscii = true;

	for (unsigned int i = 0; str[i] != '\0'; ++i) {
		chr = *(str + i);
		//判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx 
		if (nBytes == 0 && (chr & 0x80) != 0) {
			bAllAscii = false;
		}

		if (nBytes == 0) {
			//如果不是ASCII码,应该是多字节符,计算字节数  
			if (chr >= 0x80) {

				if (chr >= 0xFC && chr <= 0xFD) {
					nBytes = 6;
				}
				else if (chr >= 0xF8) {
					nBytes = 5;
				}
				else if (chr >= 0xF0) {
					nBytes = 4;
				}
				else if (chr >= 0xE0) {
					nBytes = 3;
				}
				else if (chr >= 0xC0) {
					nBytes = 2;
				}
				else {
					return false;
				}

				nBytes--;
			}
		}
		else {
			//多字节符的非首字节,应为 10xxxxxx 
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			//减到为零为止
			nBytes--;
		}
	}

	//违返UTF8编码规则 
	if (nBytes != 0) {
		return false;
	}

	if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
		return true;
	}

	return true;
}


void decodeProtoBuff(const char * pBuffer,const uint64_t pSize)
{
	struct ErgodicNode ergodicNodeRoot = { 0 };
	ergodicNodeRoot.Buffer = (char*)pBuffer;
	ergodicNodeRoot.Size = pSize;
	vErgodic.push(ergodicNodeRoot);
	

	vErgodicP.push(&ProtoNodeTree.vNodeRoot);

	while (false == vErgodic.empty())
	{
		std::vector<struct ProtoNode>* vNodeEntry = vErgodicP.front();
		vErgodicP.pop();

		struct ErgodicNode ergodicNodeEntry = vErgodic.front();
		vErgodic.pop();

		char* bufferEntryPtr = (char*)ergodicNodeEntry.Buffer;
		uint64_t bufferEntrySize = ergodicNodeEntry.Size;
		const char* bufferMaxPtr = bufferEntryPtr + bufferEntrySize;
		
		while (bufferEntryPtr < bufferMaxPtr || bufferEntrySize < 0)
		{
		 
			uint64_t ul64IndexType = 0;
			int varintReadSize = 0;
			ProtoNode protoNode = { 0 };

			varintReadSize = varint_read_u(bufferEntryPtr, bufferEntrySize, &ul64IndexType);
			bufferEntryPtr += varintReadSize;
			bufferEntrySize -= varintReadSize;

			enum protoType type = (enum protoType)(ul64IndexType & 7);
			uint64_t ul64fieldNumber = ul64IndexType >> 3;

			protoNode.nodetype = type;
			switch (type)
			{
			case m_VARINT:
			{
				uint64_t ul64Value = 0;
				NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
				varintReadSize = varint_read_u(bufferEntryPtr, bufferEntrySize, &ul64Value);
				protoNode.u.u64Buffer = ul64Value;
				bufferEntryPtr += varintReadSize;
				bufferEntrySize -= varintReadSize;
				vNodeEntry->push_back(protoNode);
			}
			break;
			case m_STRING:
			{
				uint64_t ul64Length = 0;
				char* bufferStringPtr = NULL;

				varintReadSize = varint_read_u(bufferEntryPtr, bufferEntrySize, &ul64Length);
				bufferEntryPtr += varintReadSize;
				bufferEntrySize -= varintReadSize;
				bufferStringPtr = (char*)RmHeapAlloc(ul64Length + 1);
				if (bufferStringPtr)
				{
					memcpy(bufferStringPtr, bufferEntryPtr, ul64Length);
					vMemoryFree.push_back(bufferStringPtr);
				}

				if (false == is_str_utf8(bufferStringPtr))
				{
					struct ErgodicNode ergodicNode = {0};
					ergodicNode.Buffer = bufferStringPtr;
					ergodicNode.Size = ul64Length;
					vErgodic.push(ergodicNode);

					int endIndex = vNodeEntry->size();
					protoNode.nodetype = m_STRUCT;
					vNodeEntry->push_back(protoNode);
					vNodeEntry = &vNodeEntry[0][endIndex].vSubNode;
					vErgodicP.push(vNodeEntry);

				}
				else
				{
					RtlInitAnsiString(&protoNode.u.unistrBuffer, bufferStringPtr);
					vNodeEntry->push_back(protoNode);
				}

				bufferEntryPtr = bufferEntryPtr + ul64Length;
				bufferEntrySize = bufferEntrySize - ul64Length;
			}
			break;
			case m_FIXED32:
			{
				uint32_t u32Value = 0;
				u32Value = *(uint32_t*)bufferEntryPtr;
				protoNode.u.u64Buffer = u32Value;
				bufferEntryPtr = bufferEntryPtr + 4;
				bufferEntrySize = bufferEntrySize - 4;
				vNodeEntry->push_back(protoNode);
			}
			break;
			case m_FIXED64:
			{
				uint64_t u64Value = 0;
				u64Value = *(uint64_t*)bufferEntryPtr;
				protoNode.u.u64Buffer = u64Value;
				bufferEntryPtr = bufferEntryPtr + 8;
				bufferEntrySize = bufferEntrySize - 8;
				vNodeEntry->push_back(protoNode);
			}
			break;
			default:
				break;
			}

		}
	}
	__debugbreak();
	
}