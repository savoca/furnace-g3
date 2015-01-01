/*
** =========================================================================
** File:
**     VibeOSKernelLinuxTime.c
**
** Description: 
**     Time helper functions for Linux.
**
** Portions Copyright (c) 2008-2011 Immersion Corporation. All Rights Reserved. 
**
** This file contains Original Code and/or Modifications of Original Code 
** as defined in and that are subject to the GNU Public License v2 - 
** (the 'License'). You may not use this file except in compliance with the 
** License. You should have received a copy of the GNU General Public License 
** along with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or contact 
** TouchSenseSales@immersion.com.
**
** The Original Code and all software distributed under the License are 
** distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
** EXPRESS OR IMPLIED, AND IMMERSION HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
** INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
** FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. Please see 
** the License for the specific language governing rights and limitations 
** under the License.
** =========================================================================
*/

#error "Please read the following statement"
/* 
                                                                         
                                                                              
                                    
*/
#error "End of statement"

#if (HZ != 1000)
#error The Kernel timer is not configured at 1ms. Please update TIMER_INCR to generate a proper 5ms timer.
#endif

#include <linux/mutex.h>

#define TIMER_INCR                      5       /*                                 */
#define WATCHDOG_TIMEOUT                10      /*                        */

/*                                      */
#ifndef DEFINE_SEMAPHORE
#define DEFINE_SEMAPHORE(name) \
    struct semaphore name = __SEMAPHORE_INITIALIZER(name, 1)
#endif

/*                  */
static bool g_bTimerStarted = false;
static struct timer_list g_timerList;
static int g_nWatchdogCounter = 0;

DEFINE_SEMAPHORE(g_hMutex);

/*                      */
static void VibeOSKernelLinuxStartTimer(void);
static void VibeOSKernelLinuxStopTimer(void);
static int VibeOSKernelProcessData(void* data);
#define VIBEOSKERNELPROCESSDATA

static inline int VibeSemIsLocked(struct semaphore *lock)
{
#if ((LINUX_VERSION_CODE & 0xFFFFFF) < KERNEL_VERSION(2,6,27))
    return atomic_read(&lock->count) != 1;
#else
    return (lock->count) != 1;
#endif
}

static void tsp_timer_interrupt(unsigned long param)
{
    /*                                          */
    mod_timer(&g_timerList, jiffies + TIMER_INCR);

    if(g_bTimerStarted)
    {
        if (VibeSemIsLocked(&g_hMutex)) up(&g_hMutex);
    }
}

static int VibeOSKernelProcessData(void* data)
{
    int i;
    int nActuatorNotPlaying = 0;

    for (i = 0; i < NUM_ACTUATORS; i++) 
    {
        actuator_samples_buffer *pCurrentActuatorSample = &(g_SamplesBuffer[i]);

        if (-1 == pCurrentActuatorSample->nIndexPlayingBuffer)
        {
            nActuatorNotPlaying++;
            if ((NUM_ACTUATORS == nActuatorNotPlaying) && ((++g_nWatchdogCounter) > WATCHDOG_TIMEOUT))
            {
                VibeInt8 cZero[1] = {0};

                /*                                                                                                   */
                ImmVibeSPI_ForceOut_SetSamples(i, 8, 1, cZero);
                ImmVibeSPI_ForceOut_AmpDisable(i);
                VibeOSKernelLinuxStopTimer();

                /*                        */
                g_nWatchdogCounter = 0;
            }
        }
        else
        {
            /*                         */
            if (VIBE_E_FAIL == ImmVibeSPI_ForceOut_SetSamples(
                pCurrentActuatorSample->actuatorSamples[(int)pCurrentActuatorSample->nIndexPlayingBuffer].nActuatorIndex, 
                pCurrentActuatorSample->actuatorSamples[(int)pCurrentActuatorSample->nIndexPlayingBuffer].nBitDepth, 
                pCurrentActuatorSample->actuatorSamples[(int)pCurrentActuatorSample->nIndexPlayingBuffer].nBufferSize,
                pCurrentActuatorSample->actuatorSamples[(int)pCurrentActuatorSample->nIndexPlayingBuffer].dataBuffer))
            {
                /*                                                                                 */
                mod_timer(&g_timerList, jiffies + TIMER_INCR);
            }

            pCurrentActuatorSample->nIndexOutputValue += pCurrentActuatorSample->actuatorSamples[(int)pCurrentActuatorSample->nIndexPlayingBuffer].nBufferSize;

            if (pCurrentActuatorSample->nIndexOutputValue >= pCurrentActuatorSample->actuatorSamples[(int)pCurrentActuatorSample->nIndexPlayingBuffer].nBufferSize)
            {
                /*                                     */
                pCurrentActuatorSample->actuatorSamples[(int)pCurrentActuatorSample->nIndexPlayingBuffer].nBufferSize = 0;

                /*               */
                (pCurrentActuatorSample->nIndexPlayingBuffer) ^= 1;
                pCurrentActuatorSample->nIndexOutputValue = 0;

                /*                                                */
                if (g_bStopRequested)
                {
                    pCurrentActuatorSample->nIndexPlayingBuffer = -1; 

                    ImmVibeSPI_ForceOut_AmpDisable(i);
                }
            }
        }
    }

    /*                                 */
    if (g_bStopRequested)
    {
        VibeOSKernelLinuxStopTimer();

        /*                        */
        g_nWatchdogCounter = 0;

        if (VibeSemIsLocked(&g_hMutex)) up(&g_hMutex);
        return 1;   /*                                            */
    }

    return 0;
}

static void VibeOSKernelLinuxInitTimer(void)
{
    /*                                                                     */
    init_timer(&g_timerList);
    g_timerList.function = tsp_timer_interrupt;
}

static void VibeOSKernelLinuxStartTimer(void)
{
    int i;
    int res;

    /*                        */
    g_nWatchdogCounter = 0;

    if (!g_bTimerStarted)
    {
        if (!VibeSemIsLocked(&g_hMutex)) res = down_interruptible(&g_hMutex); /*              */

        g_bTimerStarted = true;

        /*                 */
        g_timerList.expires = jiffies + TIMER_INCR;
        add_timer(&g_timerList);

        /*                                                                                                                  */
        for (i = 0; i < NUM_ACTUATORS; i++)
        {
            if ((g_SamplesBuffer[i].actuatorSamples[0].nBufferSize) || (g_SamplesBuffer[i].actuatorSamples[1].nBufferSize))
            {
                g_SamplesBuffer[i].nIndexOutputValue = 0;
                return;
            }
        }
    }

    if (0 != VibeOSKernelProcessData(NULL)) return;

    /* 
                                                    
                                                                             
    */
    res = down_interruptible(&g_hMutex);  /*                                             */
    if (res != 0)
    {
        DbgOut((KERN_INFO "VibeOSKernelLinuxStartTimer: down_interruptible interrupted by a signal.\n"));
    }
}

static void VibeOSKernelLinuxStopTimer(void)
{
    int i;

    if (g_bTimerStarted)
    {
        g_bTimerStarted = false;

        /* 
                          
                                            
                                                                                  
                                                                               
        */
        del_timer(&g_timerList);
    }

    /*                       */
    for (i = 0; i < NUM_ACTUATORS; i++)
    {
        g_SamplesBuffer[i].nIndexPlayingBuffer = -1;
        g_SamplesBuffer[i].actuatorSamples[0].nBufferSize = 0;
        g_SamplesBuffer[i].actuatorSamples[1].nBufferSize = 0;
    }
    g_bStopRequested = false;
    g_bIsPlaying = false;
} 

static void VibeOSKernelLinuxTerminateTimer(void)
{
    VibeOSKernelLinuxStopTimer();
    if (VibeSemIsLocked(&g_hMutex)) up(&g_hMutex);
}
