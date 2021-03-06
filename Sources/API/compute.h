/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

/// \brief <p>ClanLib OpenCL library.</p>
//! Global=Compute

#pragma once

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "Compute/setup_compute.h"
#include "Compute/compute_buffer.h"
#include "Compute/compute_command_queue.h"
#include "Compute/compute_context.h"
#include "Compute/compute_event.h"
#include "Compute/compute_kernel.h"
#include "Compute/compute_memory_map.h"
#include "Compute/compute_program.h"
#include "Compute/compute_sampler.h"
#include "Compute/compute_transfer_buffer.h"
#include "Compute/compute_wait_list.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(DLL)
			#pragma comment(lib, "clanCompute-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanCompute-static-mtdll.lib")
		#else
			#pragma comment(lib, "clanCompute-static-mt.lib")
		#endif
	#else
		#if defined(DLL)
			#pragma comment(lib, "clanCompute-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanCompute-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "clanCompute-static-mt-debug.lib")
		#endif
	#endif
#endif
