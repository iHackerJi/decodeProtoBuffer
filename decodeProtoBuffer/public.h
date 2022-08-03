#pragma once

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <windows.h>
#include <winternl.h>
#include <winnt.h>
#include <iostream>
#include <vector>
#include <queue>
#pragma comment(lib, "ntdll.lib")

#define RmHeapAlloc(Size) calloc(1,Size)
#define RmHeapFree(Buffer) (free(Buffer))

#include "def.h"
#include "import.h"
#include "Node.h"
#include "ProtoBufParse.h"
#include "ProtoBuffDeCoder.h"
