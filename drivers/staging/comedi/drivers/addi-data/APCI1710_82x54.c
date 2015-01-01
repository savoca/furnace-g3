/*
 *  Copyright (C) 2004,2005  ADDI-DATA GmbH for the source code of this module.
 *
 *	ADDI-DATA GmbH
 *	Dieselstrasse 3
 *	D-77833 Ottersweier
 *	Tel: +19(0)7223/9493-0
 *	Fax: +49(0)7223/9493-92
 *	http://www.addi-data.com
 *	info@addi-data.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */
/*
                                                                           
*/

#include "APCI1710_82x54.h"

/*
                                                                              
                                                                              
                                                                                       
                                                                                       
                                                                                       
                                                                                       
                                                                              
                                                                                       
                                                                                       
                                                                                       
                                                                      
                                                                                
                                             
 
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                      
                                                             
                                                         
                                          
                                           
                                                
                                             
                                          
                                               
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
*/

int i_APCI1710_InsnConfigInitTimer(struct comedi_device *dev, struct comedi_subdevice *s,
				   struct comedi_insn *insn, unsigned int *data)
{

	int i_ReturnValue = 0;
	unsigned char b_ModulNbr;
	unsigned char b_TimerNbr;
	unsigned char b_TimerMode;
	unsigned int ul_ReloadValue;
	unsigned char b_InputClockSelection;
	unsigned char b_InputClockLevel;
	unsigned char b_OutputLevel;
	unsigned char b_HardwareGateLevel;

	/*                                                                  */
	unsigned int dw_Test = 0;
	/*                                                                */

	i_ReturnValue = insn->n;
	b_ModulNbr = (unsigned char) CR_AREF(insn->chanspec);
	b_TimerNbr = (unsigned char) CR_CHAN(insn->chanspec);
	b_TimerMode = (unsigned char) data[0];
	ul_ReloadValue = (unsigned int) data[1];
	b_InputClockSelection = (unsigned char) data[2];
	b_InputClockLevel = (unsigned char) data[3];
	b_OutputLevel = (unsigned char) data[4];
	b_HardwareGateLevel = (unsigned char) data[5];

	/*                        */
	if (b_ModulNbr < 4) {
		/*                     */
		if ((devpriv->s_BoardInfos.dw_MolduleConfiguration[b_ModulNbr] & 0xFFFF0000UL) == APCI1710_82X54_TIMER) {
			/*                       */

			if (b_TimerNbr <= 2) {
				/*                     */
				if (b_TimerMode <= 5) {
					/*                                                                  */
					/*                               */
					/*
                                                                  
                                                                                              
      */

					if (((b_TimerNbr == 0) &&
					     (b_InputClockSelection == APCI1710_PCI_BUS_CLOCK)) ||
					    ((b_TimerNbr == 0) &&
					     (b_InputClockSelection == APCI1710_10MHZ)) ||
					    ((b_TimerNbr != 0) &&
					     ((b_InputClockSelection == APCI1710_PCI_BUS_CLOCK) ||
					      (b_InputClockSelection == APCI1710_FRONT_CONNECTOR_INPUT) ||
					      (b_InputClockSelection == APCI1710_10MHZ)))) {
						/*                                                                  */
						if (((b_InputClockSelection == APCI1710_10MHZ) &&
						     ((devpriv->s_BoardInfos.dw_MolduleConfiguration[b_ModulNbr] & 0x0000FFFFUL) >= 0x3131)) ||
						     (b_InputClockSelection != APCI1710_10MHZ)) {
							/*                                                                */
							/*                                      */

							if ((b_InputClockLevel == 0) ||
							    (b_InputClockLevel == 1)) {
								/*                                       */
								if ((b_OutputLevel == 0) || (b_OutputLevel == 1)) {
									/*                                        */
									if ((b_HardwareGateLevel == 0) || (b_HardwareGateLevel == 1)) {
										/*                                                                */
										/*                                                   */
										if ((b_InputClockSelection == APCI1710_10MHZ) && ((devpriv->s_BoardInfos.dw_MolduleConfiguration[b_ModulNbr] & 0x0000FFFFUL) > 0x3131)) {
											/*                               */
											dw_Test = inl(devpriv->s_BoardInfos.ui_Address + (16 + (b_TimerNbr * 4) + (64 * b_ModulNbr)));

											dw_Test = (dw_Test >> 16) & 1;
										} else {
											dw_Test = 1;
										}

										/*                      */
										if (dw_Test == 1) {
											/*                                                              */
											/*                   */
											devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_82X54Init = 1;

											/*                                */
											devpriv-> s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_InputClockSelection = b_InputClockSelection;

											/*                            */
											devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_InputClockLevel = ~b_InputClockLevel & 1;

											/*                       */
											devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_OutputLevel = ~b_OutputLevel & 1;

											/*                     */
											devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_HardwareGateLevel = b_HardwareGateLevel;

											/*                                                  */
											/*                                                                */
											/*
                                                 
                                
                                             
                                                                                                 
                                                          
                                                          
                                                           
            */
											/*                        */
											if (b_InputClockSelection == APCI1710_10MHZ) {
												b_InputClockSelection = 2;
											}

											devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord = (unsigned int)(((b_HardwareGateLevel << 0) & 0x1) | ((b_InputClockLevel << 1) & 0x2) | (((~b_OutputLevel & 1) << 2) & 0x4) | ((b_InputClockSelection << 4) & 0x30));
											/*                                                              */
											outl(devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord, devpriv->s_BoardInfos.ui_Address + 32 + (b_TimerNbr * 4) + (64 * b_ModulNbr));

											/*                            */
											outl((unsigned int) b_TimerMode, devpriv->s_BoardInfos.ui_Address + 16 + (b_TimerNbr * 4) + (64 * b_ModulNbr));

											/*                        */
											outl(ul_ReloadValue, devpriv->s_BoardInfos.ui_Address + 0 + (b_TimerNbr * 4) + (64 * b_ModulNbr));
											/*                                                                */
										}	/*                    */
										else {
											/*                                      */
											i_ReturnValue = -6;
										}	/*                    */
										/*                                                              */
									}	/*                                                                */
									else {
										/*                                             */
										DPRINTK("Selection from hardware gate level is wrong\n");
										i_ReturnValue = -9;
									}	/*                                                                */
								}	/*                                                    */
								else {
									/*                                            */
									DPRINTK("Selection from output clock level is wrong\n");
									i_ReturnValue = -8;
								}	/*                                                    */
							}	/*                                                            */
							else {
								/*                                           */
								DPRINTK("Selection from input clock level is wrong\n");
								i_ReturnValue = -7;
							}	/*                                                            */
						} else {
							/*                                      */
							DPRINTK("Input timer clock selection is wrong\n");
							i_ReturnValue = -6;
						}
					} else {
						/*                                      */
						DPRINTK("Input timer clock selection is wrong\n");
						i_ReturnValue = -6;
					}
				}	/*                                                */
				else {
					/*                               */
					DPRINTK("Timer mode selection is wrong\n");
					i_ReturnValue = -5;
				}	/*                                                */
			}	/*                                              */
			else {
				/*                       */
				DPRINTK("Timer selection wrong\n");
				i_ReturnValue = -3;
			}	/*                                              */
		} else {
			/*                                  */
			DPRINTK("The module is not a TIMER module\n");
			i_ReturnValue = -4;
		}
	} else {
		/*                     */
		DPRINTK("Module number error\n");
		i_ReturnValue = -2;
	}

	return i_ReturnValue;
}

/*
                                                                              
                                                                              
                                                                                       
                                                                                       
                                                                                       
                                                                  
                                                                                                
                                                              
                                                                              
                                                                                         
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                      
                                                             
                                                         
                                                                 */
+----------------------------------------------------------------------------+
| Output Parameters : -                                                      |
+----------------------------------------------------------------------------+
| Return Value      : 0: No error                                            |
|                    -1: The handle parameter of the board is wrong          |
|                    -2: Module selection wrong                              |
|                    -3: Timer selection wrong                               |
|                    -4: The module is not a TIMER module                    |
|                    -5: Timer not initialised see function                  |
|                        "i_APCI1710_InitTimer"                              |
|                    -6: Interrupt parameter is wrong                        |
|                    -7: Interrupt function not initialised.                 |
|                        See function "i_APCI1710_SetBoardIntRoutineX"       |
+----------------------------------------------------------------------------+
*/

int i_APCI1710_InsnWriteEnableDisableTimer(struct comedi_device *dev,
					   struct comedi_subdevice *s,
					   struct comedi_insn *insn, unsigned int *data)
{
	int i_ReturnValue = 0;
	unsigned int dw_DummyRead;
	unsigned char b_ModulNbr;
	unsigned char b_TimerNbr;
	unsigned char b_ActionType;
	unsigned char b_InterruptEnable;

	i_ReturnValue = insn->n;
	b_ModulNbr = (unsigned char) CR_AREF(insn->chanspec);
	b_TimerNbr = (unsigned char) CR_CHAN(insn->chanspec);
	b_ActionType = (unsigned char) data[0];	/*                 */

	/*                        */
	if (b_ModulNbr < 4) {
		/*                     */
		if ((devpriv->s_BoardInfos.dw_MolduleConfiguration[b_ModulNbr] & 0xFFFF0000UL) == APCI1710_82X54_TIMER) {
			/*                       */
			if (b_TimerNbr <= 2) {
				/*                           */
				if (devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_82X54Init == 1) {

					switch (b_ActionType) {
					case APCI1710_ENABLE:
						b_InterruptEnable = (unsigned char) data[1];
						/*                              */
						if ((b_InterruptEnable == APCI1710_ENABLE) ||
						    (b_InterruptEnable == APCI1710_DISABLE)) {
							if (b_InterruptEnable == APCI1710_ENABLE) {

								dw_DummyRead = inl(devpriv->s_BoardInfos.ui_Address + 12 + (b_TimerNbr * 4) + (64 * b_ModulNbr));

								/*                      */
								devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord = devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord | 0x8;

								outl(devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord, devpriv->s_BoardInfos.ui_Address + 32 + (b_TimerNbr * 4) + (64 * b_ModulNbr));
								devpriv->tsk_Current = current;	/*                                          */

							}	/*                                            */
							else {
								/*                       */
								devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord = devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord & 0xF7;

								outl(devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord, devpriv->s_BoardInfos.ui_Address + 32 + (b_TimerNbr * 4) + (64 * b_ModulNbr));

								/*                         */
								devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.b_InterruptMask = devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.b_InterruptMask & (0xFF - (1 << b_TimerNbr));
							}	/*                                            */

							/*                     */
							if (i_ReturnValue >= 0) {
								/*                         */
								devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.b_InterruptMask = devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.b_InterruptMask | ((1 & b_InterruptEnable) << b_TimerNbr);

								/*                  */
								outl(1, devpriv->s_BoardInfos.ui_Address + 44 + (b_TimerNbr * 4) + (64 * b_ModulNbr));
							}
						} else {
							/*                              */
							DPRINTK("\n");
							i_ReturnValue = -6;
						}
						break;
					case APCI1710_DISABLE:
						/*                         */
						if (((devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.b_InterruptMask >> b_TimerNbr) & 1) == 1) {
							/*                       */

							devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr]. dw_ConfigurationWord = devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord & 0xF7;

							outl(devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].dw_ConfigurationWord, devpriv->s_BoardInfos.ui_Address + 32 + (b_TimerNbr * 4) + (64 * b_ModulNbr));

							/*                         */
							devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.b_InterruptMask = devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.b_InterruptMask & (0xFF - (1 << b_TimerNbr));
						}

						/*                   */
						outl(0, devpriv->s_BoardInfos.ui_Address + 44 + (b_TimerNbr * 4) + (64 * b_ModulNbr));
						break;
					}	/*             */
				} else {
					/*                                    */
					DPRINTK ("Timer not initialised see function\n");
					i_ReturnValue = -5;
				}
			} else {
				/*                       */
				DPRINTK("Timer selection wrong\n");
				i_ReturnValue = -3;
			}	/*                                              */
		} else {
			/*                                  */
			DPRINTK("The module is not a TIMER module\n");
			i_ReturnValue = -4;
		}
	} else {
		/*                     */
		DPRINTK("Module number error\n");
		i_ReturnValue = -2;
	}

	return i_ReturnValue;
}

/*
                                                                              
                                                                              
                                                                                       
                                                                                       
                                                                     
                                                                                          
                                                      
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
*/

int i_APCI1710_InsnReadAllTimerValue(struct comedi_device *dev, struct comedi_subdevice *s,
				     struct comedi_insn *insn, unsigned int *data)
{
	int i_ReturnValue = 0;
	unsigned char b_ModulNbr, b_ReadType;
	unsigned int *pul_TimerValueArray;

	b_ModulNbr = CR_AREF(insn->chanspec);
	b_ReadType = CR_CHAN(insn->chanspec);
	pul_TimerValueArray = (unsigned int *) data;
	i_ReturnValue = insn->n;

	switch (b_ReadType) {
	case APCI1710_TIMER_READINTERRUPT:

		data[0] = devpriv->s_InterruptParameters.s_FIFOInterruptParameters[devpriv->s_InterruptParameters.ui_Read].b_OldModuleMask;
		data[1] = devpriv->s_InterruptParameters.s_FIFOInterruptParameters[devpriv->s_InterruptParameters.ui_Read].ul_OldInterruptMask;
		data[2] = devpriv->s_InterruptParameters.s_FIFOInterruptParameters[devpriv->s_InterruptParameters.ui_Read].ul_OldCounterLatchValue;

		/*                         */
		devpriv->s_InterruptParameters.ui_Read = (devpriv->s_InterruptParameters.ui_Read + 1) % APCI1710_SAVE_INTERRUPT;

		break;

	case APCI1710_TIMER_READALLTIMER:
		/*                        */
		if (b_ModulNbr < 4) {
			/*                     */
			if ((devpriv->s_BoardInfos.dw_MolduleConfiguration[b_ModulNbr] & 0xFFFF0000UL) == APCI1710_82X54_TIMER) {
				/*                              */
				if (devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[0].b_82X54Init == 1) {
					/*                              */
					if (devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[1].b_82X54Init == 1) {
						/*                              */
						if (devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[2].b_82X54Init == 1) {
							/*                   */
							outl(0x17, devpriv->s_BoardInfos.ui_Address + 12 + (64 * b_ModulNbr));

							/*                        */
							pul_TimerValueArray[0] = inl(devpriv->s_BoardInfos.ui_Address + 0 + (64 * b_ModulNbr));

							/*                        */
							pul_TimerValueArray[1] = inl(devpriv->s_BoardInfos.ui_Address + 4 + (64 * b_ModulNbr));

							/*                        */
							pul_TimerValueArray[2] = inl(devpriv->s_BoardInfos.ui_Address + 8 + (64 * b_ModulNbr));
						} else {
							/*                                      */
							DPRINTK("Timer 2 not initialised see function\n");
							i_ReturnValue = -6;
						}
					} else {
						/*                                      */
						DPRINTK("Timer 1 not initialised see function\n");
						i_ReturnValue = -5;
					}
				} else {
					/*                                      */
					DPRINTK("Timer 0 not initialised see function\n");
					i_ReturnValue = -4;
				}
			} else {
				/*                                  */
				DPRINTK("The module is not a TIMER module\n");
				i_ReturnValue = -3;
			}
		} else {
			/*                     */
			DPRINTK("Module number error\n");
			i_ReturnValue = -2;
		}

	}			/*                */
	return i_ReturnValue;
}

/*
                                                                              
                                                                            
                                                                                           
                                                                              
                                                                                               
                                                                              
                     
                                                                              
                                                                              
                                                                              
                     
                                                                              
*/

int i_APCI1710_InsnBitsTimer(struct comedi_device *dev, struct comedi_subdevice *s,
			     struct comedi_insn *insn, unsigned int *data)
{
	unsigned char b_BitsType;
	int i_ReturnValue = 0;
	b_BitsType = data[0];

	printk("\n82X54");

	switch (b_BitsType) {
	case APCI1710_TIMER_READVALUE:
		i_ReturnValue = i_APCI1710_ReadTimerValue(dev,
							  (unsigned char)CR_AREF(insn->chanspec),
							  (unsigned char)CR_CHAN(insn->chanspec),
							  (unsigned int *) &data[0]);
		break;

	case APCI1710_TIMER_GETOUTPUTLEVEL:
		i_ReturnValue = i_APCI1710_GetTimerOutputLevel(dev,
							       (unsigned char)CR_AREF(insn->chanspec),
							       (unsigned char)CR_CHAN(insn->chanspec),
							       (unsigned char *) &data[0]);
		break;

	case APCI1710_TIMER_GETPROGRESSSTATUS:
		i_ReturnValue = i_APCI1710_GetTimerProgressStatus(dev,
								  (unsigned char)CR_AREF(insn->chanspec),
								  (unsigned char)CR_CHAN(insn->chanspec),
								  (unsigned char *)&data[0]);
		break;

	case APCI1710_TIMER_WRITEVALUE:
		i_ReturnValue = i_APCI1710_WriteTimerValue(dev,
							   (unsigned char)CR_AREF(insn->chanspec),
							   (unsigned char)CR_CHAN(insn->chanspec),
							   (unsigned int)data[1]);

		break;

	default:
		printk("Bits Config Parameter Wrong\n");
		i_ReturnValue = -1;
	}

	if (i_ReturnValue >= 0)
		i_ReturnValue = insn->n;
	return i_ReturnValue;
}

/*
                                                                              
                                                                              
                                                                                       
                                                                                       
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
*/

int i_APCI1710_ReadTimerValue(struct comedi_device *dev,
			      unsigned char b_ModulNbr, unsigned char b_TimerNbr,
			      unsigned int *pul_TimerValue)
{
	int i_ReturnValue = 0;

	/*                        */
	if (b_ModulNbr < 4) {
		/*                     */
		if ((devpriv->s_BoardInfos.
		     dw_MolduleConfiguration[b_ModulNbr] &
		     0xFFFF0000UL) == APCI1710_82X54_TIMER) {
			/*                       */
			if (b_TimerNbr <= 2) {
				/*                           */
				if (devpriv->
				    s_ModuleInfo[b_ModulNbr].
				    s_82X54ModuleInfo.
				    s_82X54TimerInfo[b_TimerNbr].
				    b_82X54Init == 1) {
					/*                       */
					outl((2 << b_TimerNbr) | 0xD0,
					     devpriv->s_BoardInfos.
					     ui_Address + 12 +
					     (64 * b_ModulNbr));

					/*                        */
					*pul_TimerValue =
					    inl(devpriv->s_BoardInfos.
						ui_Address + (b_TimerNbr * 4) +
						(64 * b_ModulNbr));
				} else {
					/*                                    */
					DPRINTK("Timer not initialised see function\n");
					i_ReturnValue = -5;
				}
			} else {
				/*                       */
				DPRINTK("Timer selection wrong\n");
				i_ReturnValue = -3;
			}	/*                                              */
		} else {
			/*                                  */
			DPRINTK("The module is not a TIMER module\n");
			i_ReturnValue = -4;
		}
	} else {
		/*                     */
		DPRINTK("Module number error\n");
		i_ReturnValue = -2;
	}

	return i_ReturnValue;
}

	/*
                                                                                  
                                                                                  
                                                                                           
                                                                                           
                                                                                           
                                                                                            
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                           
                                                                                  
                                                                                           
                                                                                  
                                                                                           
                                                                                  
                                                                                  
                                                                                            
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
  */

int i_APCI1710_GetTimerOutputLevel(struct comedi_device *dev,
				   unsigned char b_ModulNbr, unsigned char b_TimerNbr,
				   unsigned char *pb_OutputLevel)
{
	int i_ReturnValue = 0;
	unsigned int dw_TimerStatus;

	/*                        */
	if (b_ModulNbr < 4) {
		/*                     */
		if ((devpriv->s_BoardInfos.dw_MolduleConfiguration[b_ModulNbr] & 0xFFFF0000UL) == APCI1710_82X54_TIMER) {
			/*                       */
			if (b_TimerNbr <= 2) {
				/*                           */
				if (devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_82X54Init == 1) {
					/*                       */
					outl((2 << b_TimerNbr) | 0xE0, devpriv->s_BoardInfos.ui_Address + 12 + (64 * b_ModulNbr));

					/*                       */
					dw_TimerStatus = inl(devpriv->s_BoardInfos.ui_Address + 16 + (b_TimerNbr * 4) + (64 * b_ModulNbr));

					*pb_OutputLevel = (unsigned char) (((dw_TimerStatus >> 7) & 1) ^ devpriv-> s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_OutputLevel);
				} else {
					/*                                    */
					DPRINTK("Timer not initialised see function\n");
					i_ReturnValue = -5;
				}
			} else {
				/*                       */
				DPRINTK("Timer selection wrong\n");
				i_ReturnValue = -3;
			}	/*                                              */
		} else {
			/*                                  */
			DPRINTK("The module is not a TIMER module\n");
			i_ReturnValue = -4;
		}
	} else {
		/*                     */
		DPRINTK("Module number error\n");
		i_ReturnValue = -2;
	}

	return i_ReturnValue;
}

/*
                                                                              
                                                                              
                                                                                       
                                                                                       
                                                                                       
                                                                                        
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                                        
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
*/

int i_APCI1710_GetTimerProgressStatus(struct comedi_device *dev,
				      unsigned char b_ModulNbr, unsigned char b_TimerNbr,
				      unsigned char *pb_TimerStatus)
{
	int i_ReturnValue = 0;
	unsigned int dw_TimerStatus;

	/*                        */
	if (b_ModulNbr < 4) {
		/*                     */

		if ((devpriv->s_BoardInfos.dw_MolduleConfiguration[b_ModulNbr] & 0xFFFF0000UL) == APCI1710_82X54_TIMER) {
			/*                       */
			if (b_TimerNbr <= 2) {
				/*                           */
				if (devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_82X54Init == 1) {
					/*                       */
					outl((2 << b_TimerNbr) | 0xE0, devpriv->s_BoardInfos.ui_Address + 12 + (64 * b_ModulNbr));

					/*                       */
					dw_TimerStatus = inl(devpriv->s_BoardInfos.ui_Address + 16 + (b_TimerNbr * 4) + (64 * b_ModulNbr));

					*pb_TimerStatus = (unsigned char) ((dw_TimerStatus) >> 8) & 1;
					printk("ProgressStatus : %d", *pb_TimerStatus);
				} else {
					/*                                    */
					i_ReturnValue = -5;
				}
			} else {
				/*                       */
				i_ReturnValue = -3;
			}	/*                                              */
		} else {
			/*                                  */

			i_ReturnValue = -4;
		}
	} else {
		/*                     */

		i_ReturnValue = -2;
	}

	return i_ReturnValue;
}

/*
                                                                              
                                                                              
                                                                                       
                                                                                       
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                                       
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
                                                                              
*/

int i_APCI1710_WriteTimerValue(struct comedi_device *dev,
			       unsigned char b_ModulNbr, unsigned char b_TimerNbr,
			       unsigned int ul_WriteValue)
{
	int i_ReturnValue = 0;

	/*                        */
	if (b_ModulNbr < 4) {
		/*                     */
		if ((devpriv->s_BoardInfos.dw_MolduleConfiguration[b_ModulNbr] & 0xFFFF0000UL) == APCI1710_82X54_TIMER) {
			/*                       */
			if (b_TimerNbr <= 2) {
				/*                           */
				if (devpriv->s_ModuleInfo[b_ModulNbr].s_82X54ModuleInfo.s_82X54TimerInfo[b_TimerNbr].b_82X54Init == 1) {
					/*                 */
					outl(ul_WriteValue, devpriv->s_BoardInfos.ui_Address + (b_TimerNbr * 4) + (64 * b_ModulNbr));
				} else {
					/*                                    */
					DPRINTK("Timer not initialised see function\n");
					i_ReturnValue = -5;
				}
			} else {
				/*                       */
				DPRINTK("Timer selection wrong\n");
				i_ReturnValue = -3;
			}	/*                                              */
		} else {
			/*                                  */
			DPRINTK("The module is not a TIMER module\n");
			i_ReturnValue = -4;
		}
	} else {
		/*                     */
		DPRINTK("Module number error\n");
		i_ReturnValue = -2;
	}

	return i_ReturnValue;
}
