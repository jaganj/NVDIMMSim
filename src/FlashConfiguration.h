/*********************************************************************************
*  Copyright (c) 2011-2012, Paul Tschirhart
*                             Peter Enns
*                             Jim Stevens
*                             Ishwar Bhati
*                             Mu-Tien Chang
*                             Bruce Jacob
*                             University of Maryland 
*                             pkt3c [at] umd [dot] edu
*  All rights reserved.
*  
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*  
*     * Redistributions of source code must retain the above copyright notice,
*        this list of conditions and the following disclaimer.
*  
*     * Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
*  
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*********************************************************************************/

#ifndef NVFLASHCONF_H
#define NVFLASHCONF_H
//SysemConfiguration.h
//Configuration values, headers, and macros for the whole system
//

#include <iostream>
#include <cstdlib>
#include <string>

#include <vector>
#include <unordered_map>
#include <queue>
#include <list>
#include <stdint.h>
#include <limits.h>


//sloppily reusing #defines from dramsim
#ifndef ERROR
#define ERROR(str) std::cerr<<"[ERROR ("<<__FILE__<<":"<<__LINE__<<")]: "<<str<<std::endl;
#endif

#ifndef WARNING
#define WARNING(str) std::cout<<"[WARNING ("<<__FILE__<<":"<<__LINE__<<")]: "<<str<<std::endl;
#endif

#ifdef DEBUG_BUILD
	#ifndef DEBUG
	#define DEBUG(str) std::cout<< str <<std::endl;
	#endif
	#ifndef DEBUGN
	#define DEBUGN(str) std::cout<< str;
	#endif
#else
	#ifndef DEBUG
	#define DEBUG(str) ;
	#endif
	#ifndef DEBUGN
	#define DEBUGN(str) ;
	#endif
#endif

#ifndef NO_OUTPUT
	#ifndef PRINT
	#define PRINT(str)  if(OUTPUT) { std::cerr <<str<<std::endl; }
	#endif
	#ifndef PRINTN
	#define PRINTN(str) if(OUTPUT) { std::cerr <<str; }
	#endif
#else
	#undef DEBUG
	#undef DEBUGN
	#define DEBUG(str) ;
	#define DEBUGN(str) ;
	#ifndef PRINT
	#define PRINT(str) ;
	#endif
	#ifndef PRINTN
	#define PRINTN(str) ;
	#endif
#endif

// Power Callback Options
#define Power_Callback 1
#define Verbose_Power_Callback 0

namespace NVDSim{

// Scheduling Options
extern bool SCHEDULE;
extern bool WRITE_ON_QUEUE_SIZE;
extern uint WRITE_QUEUE_LIMIT;
extern bool IDLE_WRITE;
extern bool CTRL_SCHEDULE;
extern bool CTRL_WRITE_ON_QUEUE_SIZE;
extern uint CTRL_WRITE_QUEUE_LIMIT;
extern bool CTRL_IDLE_WRITE;

// Buffering Options
extern bool BUFFERED;
extern uint64_t IN_BUFFER_SIZE;
extern uint64_t OUT_BUFFER_SIZE;

// Critical Cache Line First Options 
extern bool CRIT_LINE_FIRST;

// Logging Options
extern bool LOGGING;
extern std::string LOG_DIR;
extern bool WEAR_LEVEL_LOG;
extern bool RUNTIME_WRITE;
extern bool PER_PACKAGE;
extern bool QUEUE_EVENT_LOG;
extern bool PLANE_STATE_LOG;

// Save and Restore Options
extern bool ENABLE_NV_SAVE;
extern std::string NV_SAVE_FILE;
extern bool ENABLE_NV_RESTORE;
extern std::string NV_RESTORE_FILE;

extern std::string DEVICE_TYPE;
extern uint64_t NUM_PACKAGES;
extern uint64_t DIES_PER_PACKAGE;
extern uint64_t PLANES_PER_DIE;
extern uint64_t BLOCKS_PER_PLANE;
extern uint64_t VIRTUAL_BLOCKS_PER_PLANE;
extern uint64_t PAGES_PER_BLOCK;
extern uint64_t NV_PAGE_SIZE;
extern float DEVICE_CYCLE;
extern float CHANNEL_CYCLE;
extern uint64_t DEVICE_WIDTH;
extern uint64_t CHANNEL_WIDTH;
// does the device use garbage collection 
extern bool GARBAGE_COLLECT;

#define GC GARBAGE_COLLECT

extern float IDLE_GC_THRESHOLD;
extern float FORCE_GC_THRESHOLD;
extern float PBLOCKS_PER_VBLOCK;

#define BLOCK_SIZE (NV_PAGE_SIZE * PAGES_PER_BLOCK)
#define PLANE_SIZE (NV_PAGE_SIZE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define DIE_SIZE (NV_PAGE_SIZE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define PACKAGE_SIZE (NV_PAGE_SIZE * DIES_PER_PACKAGE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define TOTAL_SIZE (NV_PAGE_SIZE * NUM_PACKAGES * DIES_PER_PACKAGE * PLANES_PER_DIE * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)

#define VIRTUAL_PLANE_SIZE (NV_PAGE_SIZE * VIRTUAL_BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define VIRTUAL_DIE_SIZE (NV_PAGE_SIZE * PLANES_PER_DIE * VIRTUAL_BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define VIRTUAL_PACKAGE_SIZE (NV_PAGE_SIZE * DIES_PER_PACKAGE * PLANES_PER_DIE * VIRTUAL_BLOCKS_PER_PLANE * PAGES_PER_BLOCK)
#define VIRTUAL_TOTAL_SIZE (NV_PAGE_SIZE * NUM_PACKAGES * DIES_PER_PACKAGE * PLANES_PER_DIE * VIRTUAL_BLOCKS_PER_PLANE * PAGES_PER_BLOCK)

extern uint READ_TIME;
extern uint WRITE_TIME;
extern uint ERASE_TIME;
extern uint COMMAND_LENGTH; //in bits, including address
extern uint LOOKUP_TIME;
extern uint QUEUE_ACCESS_TIME; //time it takes to read data out of the write queue
// in nanoseconds
extern float CYCLE_TIME;

extern uint EPOCH_TIME;
#define USE_EPOCHS (EPOCH_TIME > 0)
extern uint FTL_READ_QUEUE_LENGTH;
extern uint FTL_WRITE_QUEUE_LENGTH;
extern uint CTRL_READ_QUEUE_LENGTH;
extern uint CTRL_WRITE_QUEUE_LENGTH;

// Power stuff
extern double READ_I;
extern double WRITE_I;
extern double ERASE_I;
extern double STANDBY_I;
extern double IN_LEAK_I;
extern double OUT_LEAK_I;
extern double VCC;

// PCM specific power stuff
extern double ASYNC_READ_I;
extern double VPP_STANDBY_I;
extern double VPP_READ_I;
extern double VPP_WRITE_I;
extern double VPP_ERASE_I;
extern double VPP;

extern uint OUTPUT;

//namespace NVDSim{
	typedef void (*returnCallBack_t)(uint id, uint64_t addr, uint64_t clockcycle);
}
#endif
