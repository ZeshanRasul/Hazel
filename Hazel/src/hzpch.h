#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <optional>

#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <bitset>

#include <stdint.h>

#include "Hazel/Log.h"

#ifdef HZ_PLATFORM_WINDOWS
	// TODO check this doesn't cause any issues
	#pragma warning(disable:4005)
	#define _WIN32_WINNT 0x0601
	#include <sdkddkver.h>
	// The following #defines disable a bunch of unused windows stuff. If you 
	// get weird errors when trying to do some windows stuff, try removing some
	// (or all) of these defines (it will increase build time though).
	#define WIN32_LEAN_AND_MEAN
	#define NOGDICAPMASKS
	#define NOSYSMETRICS
	#define NOMENUS
	#define NOICONS
	#define NOSYSCOMMANDS
	#define NORASTEROPS
	#define OEMRESOURCE
	#define NOATOM
	#define NOCLIPBOARD
	#define NOCOLOR
	#define NOCTLMGR
	#define NODRAWTEXT
	#define NOKERNEL
	#define NONLS
	#define NOMEMMGR
	#define NOMETAFILE
	#define NOMINMAX
	#define NOOPENFILE
	#define NOSCROLL
	#define NOSERVICE
	#define NOSOUND
	#define NOTEXTMETRIC
	#define NOWH
	#define NOCOMM
	#define NOKANJI
	#define NOHELP
	#define NOPROFILER
	#define NODEFERWINDOWPOS
	#define NOMCX
	#define NORPC
	#define NOPROXYSTUB
	#define NOIMAGE
	#define NOTAPE

	#define STRICT
	#include <Windows.h>
#endif
