/// Copyright (c) 2023 namarium
/// Licensed under the MIT License.
/// https://opensource.org/licenses/MIT

#ifndef GLOBAL_H
#define GLOBAL_H

// Windows
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
#include <windows.h>

// CLR
#include <msclr/gcroot.h>
#include <msclr/auto_gcroot.h>
#include <msclr/auto_handle.h>
#include <msclr/marshal_cppstd.h>

// STL
#include <cstdlib>		// system
#include <algorithm>	// transform
#include <array>		// array
#include <atomic>		// atomic
#include <deque>		// deque
#include <fstream>		// fstream
#include <iostream>		// cout, cerr
#include <limits>		// numeric_limits
#include <memory>		// unique_ptr
#include <mutex>		// mutex
#include <regex>		// regex
#include <sstream>		// istringstream
#include <stdexcept>	// exception
#include <string>		// string
#include <thread>		// thread
#include <tuple>		// tuple
#include <type_traits>	// underlying_type
#include <vector>		// vector

#endif //GLOBAL_H
