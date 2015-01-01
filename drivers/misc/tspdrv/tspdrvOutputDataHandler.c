/*
** =========================================================================
** File:
**     tspdrvOutputDataHandler.c
**
** Description: 
**     Output data handler module for TSP 5000 edition. 
**     This module supports multiple actuators and multiple samples
**     per update. It sends all samples from the buffer every time
**     SendOutputData() is called.
**
** Portions Copyright (c) 2011-2012 Immersion Corporation. All Rights Reserved. 
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

typedef struct
{
    VibeUInt8 nActuatorIndex;  /*                            */
    VibeUInt8 nBitDepth;       /*                       */
    VibeUInt8 nBufferSize;     /*                       */
    VibeUInt8 dataBuffer[VIBE_OUTPUT_SAMPLE_SIZE];
} actuator_samples_buffer;

/*                                  */
#define SPI_BUFFER_SIZE (NUM_ACTUATORS * (VIBE_OUTPUT_SAMPLE_SIZE + SPI_HEADER_SIZE))
static actuator_samples_buffer g_SamplesBuffer[NUM_ACTUATORS] = {{0}}; 

int SaveOutputData(const char *outputDataBuffer, int count)
{
    int i = 0;

    /*                                                         */
#if (SPI_HEADER_SIZE != 3)
#error "SPI_HEADER_SIZE expected to be 3"
#endif

    /*                   */
    if ((count < SPI_HEADER_SIZE) || (count > SPI_BUFFER_SIZE))
    {
        DbgOut((DBL_ERROR, "SaveOutputData: invalid write buffer size.\n"));
        return 0;
    }

    /*                */
    if (0 == outputDataBuffer)
    {
        DbgOut((DBL_ERROR, "SaveOutputData: outputDataBuffer invalid.\n"));
        return 0;
    }

    while (i < count)
    {
        actuator_samples_buffer* pInputBuffer = (actuator_samples_buffer*)(&outputDataBuffer[i]);

        if ((i + SPI_HEADER_SIZE) > count)
        {
            /*
                                                           
                                     
            */
            DbgOut((DBL_FATAL,"SaveOutputData: invalid buffer index.\n"));
        }

        /*                 */
        if (8 != pInputBuffer->nBitDepth)
        {
            DbgOut((DBL_WARNING, "SaveOutputData: invalid bit depth. Use default value (8).\n"));
        }

        /*                   */
        if ((i + SPI_HEADER_SIZE + pInputBuffer->nBufferSize) > count)
        {
            /*
                                                           
                                     
            */
            DbgOut((DBL_FATAL, "SaveOutputData: invalid data size.\n"));
        }
        
        /*                      */
        if (NUM_ACTUATORS <= pInputBuffer->nActuatorIndex)
        {
            DbgOut((DBL_ERROR, "SaveOutputData: invalid actuator index.\n"));
            i += (SPI_HEADER_SIZE + pInputBuffer->nBufferSize);
            continue;
        }

        if ((count == SPI_HEADER_SIZE) && (0 == pInputBuffer->nBufferSize))
        {
            /* 
                                                                                        
                                                                               
                                                    
            */
            break;
        }

        /*                                                         */
        memcpy(&(g_SamplesBuffer[pInputBuffer->nActuatorIndex]), &outputDataBuffer[i], (SPI_HEADER_SIZE + pInputBuffer->nBufferSize));

        /*                        */
        i += (SPI_HEADER_SIZE + pInputBuffer->nBufferSize);
    }

    return 1;
}

bool SendOutputData(void)
{
    int i;
    bool ret = 0;

    for (i = 0; i < NUM_ACTUATORS; i++) 
    {
        actuator_samples_buffer *pCurrentActuatorSample = &(g_SamplesBuffer[i]);

        if (pCurrentActuatorSample->nBufferSize)
        {

            /*                                                                                             
                                               
                                                    
                                                        */

            /*                         */
            ImmVibeSPI_ForceOut_SetSamples(
                i, 
                pCurrentActuatorSample->nBitDepth, 
                pCurrentActuatorSample->nBufferSize,
                pCurrentActuatorSample->dataBuffer);

            ret = 1;

            /*                               */
            pCurrentActuatorSample->nBufferSize = 0;
        }
    }

    return ret;
}

void ResetOutputData(void)
{
    int i;

    for (i = 0; i < NUM_ACTUATORS; i++)
    {
        g_SamplesBuffer[i].nBufferSize = 0;
    }
}

bool IsOutputDataBufferEmpty(void)
{
    int i;

    for (i = 0; i < NUM_ACTUATORS; i++)
    {
        if (g_SamplesBuffer[i].nBufferSize)
        {
            return 0;
        }
    }

    return 1;
}
