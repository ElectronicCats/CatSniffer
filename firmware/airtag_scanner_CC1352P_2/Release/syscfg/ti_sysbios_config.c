/*
 *  ======== ti_sysbios_config.c ========
 *  Configured BIOS module definitions
 *
 *  DO NOT EDIT - This file is generated
 *  by the SysConfig tool.
 */

#include <stddef.h>
#include <stdint.h>

#include "ti_sysbios_config.h"

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/cc26xx/Boot.h>
#include <ti/sysbios/family/arm/cc26xx/Timer.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/gates/GateMutexPri.h>
#include <ti/sysbios/heaps/HeapCallback.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Idle.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/runtime/Error.h>
#include <ti/sysbios/runtime/Memory.h>
#include <ti/sysbios/runtime/Startup.h>
#include <ti/sysbios/runtime/System.h>
#include <ti/sysbios/runtime/Timestamp.h>


/* Idle module definitions */

/* Idle functions */
extern void _pthread_cleanupFxn(void);
extern void Power_idleFunc(void);

/* Idle function list */
volatile const Idle_FuncPtr Idle_funcList[2] = {
    _pthread_cleanupFxn,
    Power_idleFunc,
};



/* Startup module definitions */

/* Startup reset functions */
void Startup_reset()
{
    Boot_trimDevice();
}

/*
 * Startup first functions
 * Module init functions
 * Startup last functions
 */
void Startup_exec()
{
    /* Startup first functions */
    Hwi_initNVIC();
    Hwi_initStack();
    BIOS_init();
    ti_sysbios_family_arm_cc26xx_TimestampProvider_init();

    /* Module init functions */
    System_init();
    Clock_init();
    Swi_init();
    Task_init();
    Idle_init();
    Hwi_init();
    HeapCallback_init();

    Startup_done = true;
}

/* BIOS module definitions */


HeapCallback_Object BIOS_heapCallbackObject;

/*
 *  ======== BIOS_init ========
 *  Perform any initialization needed before using any BIOS APIs
 */
void BIOS_init(void)
{
    if (BIOS_module->initDone) {
        return;
    }
    BIOS_module->initDone = true;

    HeapCallback_init();

    Memory_defaultHeapInstance = (IHeap_Handle)HeapCallback_construct(
        &BIOS_heapCallbackObject, NULL);
}

#include <ti/dpl/ClockP_tirtos.c>
#include <ti/dpl/DebugP_tirtos.c>
#include <ti/dpl/HwiP_tirtos.c>
#include <ti/dpl/MutexP_tirtos.c>
#include <ti/dpl/SemaphoreP_tirtos.c>
#include <ti/dpl/SwiP_tirtos.c>
#include <ti/dpl/SystemP_tirtos.c>
#include <ti/dpl/TaskP_tirtos.c>
#include <ti/dpl/PowerCC26X2_tirtos.c>
#include <ti/dpl/TimestampPCC26XX_tirtos.c>
#include <ti/posix/tirtos/clock.c>
#include <ti/posix/tirtos/mqueue.c>
#include <ti/posix/tirtos/pthread_barrier.c>
#include <ti/posix/tirtos/pthread.c>
#include <ti/posix/tirtos/pthread_cond.c>
#include <ti/posix/tirtos/pthread_key.c>
#include <ti/posix/tirtos/pthread_mutex.c>
#include <ti/posix/tirtos/pthread_rwlock.c>
#include <ti/posix/tirtos/pthread_util.c>
#include <ti/posix/tirtos/sched.c>
#include <ti/posix/tirtos/semaphore.c>
#include <ti/posix/tirtos/sleep.c>
#include <ti/posix/tirtos/timer.c>
#include <ti/sysbios/BIOS.c>
#include <ti/sysbios/family/arm/cc26xx/Boot.c>
#include <ti/sysbios/family/arm/cc26xx/ClockSupport.c>
#include <ti/sysbios/family/arm/cc26xx/Seconds.c>
#include <ti/sysbios/hal/time.c>
#include <ti/sysbios/family/arm/cc26xx/Timer.c>
#include <ti/sysbios/family/arm/cc26xx/TimestampProvider.c>
#include <ti/sysbios/family/arm/m3/Hwi.c>
#include <ti/sysbios/family/arm/m3/TaskSupport.c>
#include <ti/sysbios/gates/GateMutexPri.c>
#include <ti/sysbios/heaps/HeapCallback.c>
#include <ti/sysbios/knl/Clock.c>
#include <ti/sysbios/knl/Event.c>
#include <ti/sysbios/knl/Idle.c>
#include <ti/sysbios/knl/Mailbox.c>
#include <ti/sysbios/knl/Queue.c>
#include <ti/sysbios/knl/Semaphore.c>
#include <ti/sysbios/knl/Swi.c>
#include <ti/sysbios/knl/Task.c>
#include <ti/sysbios/rts/MemAlloc.c>
#include <ti/sysbios/runtime/Error.c>
#include <ti/sysbios/runtime/Memory.c>
#include <ti/sysbios/runtime/System.c>
#include <ti/sysbios/runtime/SysCallback.c>
#include <ti/sysbios/runtime/Timestamp.c>
#include <ti/sysbios/runtime/Startup.c>
