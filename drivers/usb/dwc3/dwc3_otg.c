/**
 * dwc3_otg.c - DesignWare USB3 DRD Controller OTG
 *
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/usb/msm_hsusb.h>

#include "core.h"
#include "dwc3_otg.h"
#include "io.h"
#include "xhci.h"

#ifdef CONFIG_LGE_PM
#include <mach/board_lge.h>
#include <linux/power_supply.h>
#if defined (CONFIG_SLIMPORT_ANX7816) || defined(CONFIG_SLIMPORT_ANX7808)
#include <linux/slimport.h>
#endif
#endif
#ifdef CONFIG_MACH_LGE
#define PARAMETER_OVERRIDE_X_REG (0xF8814)
#define DEFAULT_HSPHY_INIT (0x00D195A4) /*                     */
#endif
#define VBUS_REG_CHECK_DELAY	(msecs_to_jiffies(1000))
#define MAX_INVALID_CHRGR_RETRY 3
static int max_chgr_retry_count = MAX_INVALID_CHRGR_RETRY;
module_param(max_chgr_retry_count, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(max_chgr_retry_count, "Max invalid charger retry count");
static void dwc3_otg_reset(struct dwc3_otg *dotg);

static void dwc3_otg_notify_host_mode(struct usb_otg *otg, int host_mode);
static void dwc3_otg_reset(struct dwc3_otg *dotg);
#if defined (CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI4)
void update_status(int code, int value);
#endif
#ifdef CONFIG_QPNP_CHARGER
extern struct pseudo_batt_info_type pseudo_batt_info;
#endif

/* 
                                                                       
  
                                                                      
                                                                        
  
                                     
 */
static void dwc3_otg_set_host_regs(struct dwc3_otg *dotg)
{
	u32 reg;
	struct dwc3 *dwc = dotg->dwc;
	struct dwc3_ext_xceiv *ext_xceiv = dotg->ext_xceiv;

#ifdef CONFIG_MACH_LGE
	/*                                                     */
	reg = dwc3_readl(dwc->regs, PARAMETER_OVERRIDE_X_REG);
	reg &= ~(0x03FFFFFF);
	reg |= (DEFAULT_HSPHY_INIT & 0x03FFFFFF);
	dwc3_writel(dwc->regs, PARAMETER_OVERRIDE_X_REG, reg);
#endif

	if (ext_xceiv && !ext_xceiv->otg_capability) {
		/*                                   */
		reg = dwc3_readl(dotg->regs, DWC3_OCTL);
		reg &= ~DWC3_OTG_OCTL_PERIMODE;
		dwc3_writel(dotg->regs, DWC3_OCTL, reg);
	} else {
		reg = dwc3_readl(dwc->regs, DWC3_GCTL);
		reg &= ~(DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_OTG));
		reg |= DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_HOST);
		/*
                                                             
                                                            
                                              
   */
		reg |= DWC3_GCTL_SOFITPSYNC;
		/*
                                                                 
                                                        
   */
		reg |= DWC3_GCTL_U2RSTECN;
		reg &= ~(DWC3_GCTL_PWRDNSCALEMASK);
		reg |= DWC3_GCTL_PWRDNSCALE(2);
		dwc3_writel(dwc->regs, DWC3_GCTL, reg);
	}
}

static int dwc3_otg_set_suspend(struct usb_phy *phy, int suspend)
{
	struct usb_otg *otg = phy->otg;
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);

	if (dotg->host_bus_suspend == suspend)
		return 0;

	dotg->host_bus_suspend = suspend;
	if (suspend) {
		pm_runtime_put_sync(phy->dev);
	} else {
		pm_runtime_get_noresume(phy->dev);
		pm_runtime_resume(phy->dev);
	}

	return 0;
}

static void dwc3_otg_set_hsphy_auto_suspend(struct dwc3_otg *dotg, bool susp)
{
	struct dwc3 *dwc = dotg->dwc;
	u32 reg;

	reg = dwc3_readl(dwc->regs, DWC3_GUSB2PHYCFG(0));
	if (susp)
		reg |= DWC3_GUSB2PHYCFG_SUSPHY;
	else
		reg &= ~(DWC3_GUSB2PHYCFG_SUSPHY);
	dwc3_writel(dwc->regs, DWC3_GUSB2PHYCFG(0), reg);
}

/* 
                                                                         
  
                                                                            
                                                                         
                            
  
                                     
 */
void dwc3_otg_set_host_power(struct dwc3_otg *dotg)
{
	u32 osts;

	osts = dwc3_readl(dotg->regs, DWC3_OSTS);
	if (!(osts & 0x8))
		dev_err(dotg->dwc->dev, "%s: xHCIPrtPower not set\n", __func__);

	dwc3_writel(dotg->regs, DWC3_OCTL, DWC3_OTG_OCTL_PRTPWRCTL);
}

/* 
                                                                                   
  
                                                                            
                                                                        
  
                                         
 */
static void dwc3_otg_set_peripheral_regs(struct dwc3_otg *dotg)
{
	u32 reg;
	struct dwc3 *dwc = dotg->dwc;
	struct dwc3_ext_xceiv *ext_xceiv = dotg->ext_xceiv;

	if (ext_xceiv && !ext_xceiv->otg_capability) {
		/*                                         */
		reg = dwc3_readl(dotg->regs, DWC3_OCTL);
		reg |= DWC3_OTG_OCTL_PERIMODE;
		dwc3_writel(dotg->regs, DWC3_OCTL, reg);
		/*
                                                                  
                                                         
   */
	} else {
		reg = dwc3_readl(dwc->regs, DWC3_GCTL);
		reg &= ~(DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_OTG));
		reg |= DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_DEVICE);
		/*
                                                                 
                                                        
  */
		reg |= DWC3_GCTL_U2RSTECN;
		reg &= ~(DWC3_GCTL_PWRDNSCALEMASK);
		reg |= DWC3_GCTL_PWRDNSCALE(2);
		reg &= ~(DWC3_GCTL_SOFITPSYNC);
		dwc3_writel(dwc->regs, DWC3_GCTL, reg);
	}
}

/* 
                                                                                          
  
                                                  
                                                
  
                                                 
 */
static int dwc3_otg_start_host(struct usb_otg *otg, int on)
{
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);
	struct dwc3_ext_xceiv *ext_xceiv = dotg->ext_xceiv;
	struct dwc3 *dwc = dotg->dwc;
	int ret = 0;

	if (!dwc->xhci)
		return -EINVAL;

	if (!dotg->vbus_otg) {
		dotg->vbus_otg = devm_regulator_get(dwc->dev->parent,
							"vbus_dwc3");
		if (IS_ERR(dotg->vbus_otg)) {
			dev_err(dwc->dev, "Failed to get vbus regulator\n");
			ret = PTR_ERR(dotg->vbus_otg);
			dotg->vbus_otg = 0;
			return ret;
		}
	}

	if (on) {
		dev_dbg(otg->phy->dev, "%s: turn on host\n", __func__);

		/*
                                                            
                                                         
                                                       
                                         
                                                            
                                                         
                                                            
             
   */
		dwc3_otg_set_hsphy_auto_suspend(dotg, true);
		dwc3_otg_set_host_regs(dotg);
		/*
                                                                  
                                                              
                                                                
                                                                  
                                                                  
   */
		pm_runtime_init(&dwc->xhci->dev);
		ret = platform_device_add(dwc->xhci);
		if (ret) {
			dev_err(otg->phy->dev,
				"%s: failed to add XHCI pdev ret=%d\n",
				__func__, ret);
			return ret;
		}

		dwc3_otg_notify_host_mode(otg, on);
		ret = regulator_enable(dotg->vbus_otg);
		if (ret) {
			dev_err(otg->phy->dev, "unable to enable vbus_otg\n");
			platform_device_del(dwc->xhci);
			return ret;
		}

		/*                                                    */
		if (ext_xceiv && !ext_xceiv->otg_capability)
			dwc3_otg_reset(dotg);
	} else {
		dev_dbg(otg->phy->dev, "%s: turn off host\n", __func__);

		ret = regulator_disable(dotg->vbus_otg);
		if (ret) {
			dev_err(otg->phy->dev, "unable to disable vbus_otg\n");
			return ret;
		}
		dwc3_otg_notify_host_mode(otg, on);

		platform_device_del(dwc->xhci);
		/*
                                                               
                                                              
                             
   */
		if (ext_xceiv && ext_xceiv->otg_capability &&
						ext_xceiv->ext_block_reset)
			ext_xceiv->ext_block_reset(ext_xceiv, true);

		dwc3_otg_set_hsphy_auto_suspend(dotg, false);
		dwc3_otg_set_peripheral_regs(dotg);

		/*                                                            */
		dwc3_post_host_reset_core_init(dwc);
		if (ext_xceiv && !ext_xceiv->otg_capability)
			dwc3_otg_reset(dotg);
	}

	return 0;
}

/* 
                                                               
  
                                                  
                                           
  
                                                 
 */
static int dwc3_otg_set_host(struct usb_otg *otg, struct usb_bus *host)
{
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);

	if (host) {
		dev_dbg(otg->phy->dev, "%s: set host %s, portpower\n",
					__func__, host->bus_name);
		otg->host = host;
		/*
                                                             
                                                               
                          
   */
		msleep(300);
		dwc3_otg_set_host_power(dotg);
	} else {
		otg->host = NULL;
	}

	return 0;
}

/* 
                                                                      
  
                                                  
                                                
  
                                                 
 */
static int dwc3_otg_start_peripheral(struct usb_otg *otg, int on)
{
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);
	struct dwc3_ext_xceiv *ext_xceiv = dotg->ext_xceiv;

	if (!otg->gadget)
		return -EINVAL;

	if (on) {
		dev_dbg(otg->phy->dev, "%s: turn on gadget %s\n",
					__func__, otg->gadget->name);

		/*                                                         
                                                              */
		if (ext_xceiv && ext_xceiv->otg_capability &&
						ext_xceiv->ext_block_reset)
			ext_xceiv->ext_block_reset(ext_xceiv, false);

		dwc3_otg_set_hsphy_auto_suspend(dotg, true);
		dwc3_otg_set_peripheral_regs(dotg);
		usb_gadget_vbus_connect(otg->gadget);
	} else {
		dev_dbg(otg->phy->dev, "%s: turn off gadget %s\n",
					__func__, otg->gadget->name);
		usb_gadget_vbus_disconnect(otg->gadget);
		dwc3_otg_set_hsphy_auto_suspend(dotg, false);
	}

	return 0;
}

/* 
                                                                           
  
                                                  
                                                
  
                                                 
 */
static int dwc3_otg_set_peripheral(struct usb_otg *otg,
				struct usb_gadget *gadget)
{
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);

	if (gadget) {
		dev_dbg(otg->phy->dev, "%s: set gadget %s\n",
					__func__, gadget->name);
		otg->gadget = gadget;
		queue_delayed_work(system_nrt_wq, &dotg->sm_work, 0);
	} else {
		if (otg->phy->state == OTG_STATE_B_PERIPHERAL) {
			dwc3_otg_start_peripheral(otg, 0);
			otg->gadget = NULL;
			otg->phy->state = OTG_STATE_UNDEFINED;
			queue_delayed_work(system_nrt_wq, &dotg->sm_work, 0);
		} else {
			otg->gadget = NULL;
		}
	}

	return 0;
}

/* 
                                                                          
                                                 
                                                      
  
                       
 */
static void dwc3_ext_chg_det_done(struct usb_otg *otg, struct dwc3_charger *chg)
{
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);

	/*
                                                                       
                                                                       
  */
	if (test_bit(B_SESS_VLD, &dotg->inputs))
		queue_delayed_work(system_nrt_wq, &dotg->sm_work, 0);
}

/* 
                                                         
                                                 
                                                      
  
                       
 */
int dwc3_set_charger(struct usb_otg *otg, struct dwc3_charger *charger)
{
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);

	dotg->charger = charger;
	if (charger)
		charger->notify_detection_complete = dwc3_ext_chg_det_done;

	return 0;
}

/* 
                                                                              
                                                 
                                        
  
                       
 */
static void dwc3_ext_event_notify(struct usb_otg *otg,
					enum dwc3_ext_events event)
{
	static bool init;
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);
	struct dwc3_ext_xceiv *ext_xceiv = dotg->ext_xceiv;
	struct usb_phy *phy = dotg->otg.phy;
	int ret = 0;

	/*                                                              */
	if (init)
		flush_delayed_work(&dotg->sm_work);

	if (event == DWC3_EVENT_PHY_RESUME) {
		if (!pm_runtime_status_suspended(phy->dev)) {
			dev_warn(phy->dev, "PHY_RESUME event out of LPM!!!!\n");
		} else {
			dev_dbg(phy->dev, "ext PHY_RESUME event received\n");
			/*                                                    */
			ret = pm_runtime_get(phy->dev);
			if ((phy->state == OTG_STATE_A_HOST) &&
							dotg->host_bus_suspend)
				dotg->host_bus_suspend = 0;
			if (ret == -EACCES) {
				/*                                      
                                 */
				pm_runtime_disable(phy->dev);
				pm_runtime_set_active(phy->dev);
				pm_runtime_enable(phy->dev);
			} else if (ret < 0) {
				dev_warn(phy->dev, "pm_runtime_get failed!\n");
			}
		}
	} else if (event == DWC3_EVENT_XCEIV_STATE) {
		if (pm_runtime_status_suspended(phy->dev)) {
			dev_warn(phy->dev, "PHY_STATE event in LPM!!!!\n");
			ret = pm_runtime_get(phy->dev);
			if (ret < 0)
				dev_warn(phy->dev, "pm_runtime_get failed!!\n");
		}
		if (ext_xceiv->id == DWC3_ID_FLOAT) {
			dev_dbg(phy->dev, "XCVR: ID set\n");
			set_bit(ID, &dotg->inputs);
		} else {
			dev_dbg(phy->dev, "XCVR: ID clear\n");
			clear_bit(ID, &dotg->inputs);
		}

		if (ext_xceiv->bsv) {
			dev_dbg(phy->dev, "XCVR: BSV set\n");
			set_bit(B_SESS_VLD, &dotg->inputs);
		} else {
			dev_dbg(phy->dev, "XCVR: BSV clear\n");
			clear_bit(B_SESS_VLD, &dotg->inputs);
		}

		if (!init) {
			init = true;
			if (!work_busy(&dotg->sm_work.work))
				queue_delayed_work(system_nrt_wq,
							&dotg->sm_work, 0);

			complete(&dotg->dwc3_xcvr_vbus_init);
			dev_dbg(phy->dev, "XCVR: BSV init complete\n");
			return;
		}

		queue_delayed_work(system_nrt_wq, &dotg->sm_work, 0);
	}
}

/* 
                                                               
                                                 
                                                             
  
                       
 */
int dwc3_set_ext_xceiv(struct usb_otg *otg, struct dwc3_ext_xceiv *ext_xceiv)
{
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);

	dotg->ext_xceiv = ext_xceiv;
	if (ext_xceiv)
		ext_xceiv->notify_ext_events = dwc3_ext_event_notify;

	return 0;
}

static void dwc3_otg_notify_host_mode(struct usb_otg *otg, int host_mode)
{
	struct dwc3_otg *dotg = container_of(otg, struct dwc3_otg, otg);

	if (!dotg->psy) {
		dev_err(otg->phy->dev, "no usb power supply registered\n");
		return;
	}

	if (host_mode)
		power_supply_set_scope(dotg->psy, POWER_SUPPLY_SCOPE_SYSTEM);
	else
		power_supply_set_scope(dotg->psy, POWER_SUPPLY_SCOPE_DEVICE);
}

/*                                                                                           */
#if defined(CONFIG_LGE_PM) && !defined(CONFIG_USB_DWC3_LGE_SINGLE_PSY)
static int dwc3_otg_get_psy(struct usb_phy *phy)
{
	struct dwc3_otg *dotg = container_of(phy->otg, struct dwc3_otg, otg);

	if (dotg->charger->chg_type == DWC3_DCP_CHARGER) {
		pr_info("msm_otg_notify_power_supply: "
				"power_supply_get_by_name(ac)\n");
		dotg->psy = power_supply_get_by_name("ac");
	} else {
		pr_info("msm_otg_notify_power_supply: "
				"power_supply_get_by_name(usb)\n");
		dotg->psy = power_supply_get_by_name("usb");
	}
	if (!dotg->psy) {
		goto psy_error;
	}

	return 0;

psy_error:
	dev_dbg(phy->dev, "power supply error when setting property\n");
	return -ENXIO;
}
#endif

static int dwc3_otg_set_power(struct usb_phy *phy, unsigned mA)
{
	static int power_supply_type;
	struct dwc3_otg *dotg = container_of(phy->otg, struct dwc3_otg, otg);
#if defined(CONFIG_DWC3_MSM_BC_12_VZW_SUPPORT) && defined(CONFIG_LGE_PM)
	static bool chglogo_check = false;
#endif

	if (!dotg->psy || !dotg->charger) {
		dev_err(phy->dev, "no usb power supply/charger registered\n");
		return 0;
	}

	if (dotg->charger->charging_disabled)
		return 0;

#ifdef CONFIG_LGE_PM
	if (dotg->charger->chg_type == DWC3_SDP_CHARGER ||
			dotg->charger->chg_type == DWC3_FLOATED_CHARGER)
#else
	if (dotg->charger->chg_type == DWC3_SDP_CHARGER)
#endif
		power_supply_type = POWER_SUPPLY_TYPE_USB;
	else if (dotg->charger->chg_type == DWC3_CDP_CHARGER)
		power_supply_type = POWER_SUPPLY_TYPE_USB_CDP;
	else if (dotg->charger->chg_type == DWC3_DCP_CHARGER ||
			dotg->charger->chg_type == DWC3_PROPRIETARY_CHARGER)
		power_supply_type = POWER_SUPPLY_TYPE_USB_DCP;
	else
#ifdef CONFIG_LGE_PM
		/*                   
                                               
                                         
                                               
   */
		power_supply_type = POWER_SUPPLY_TYPE_UNKNOWN;
#else
		power_supply_type = POWER_SUPPLY_TYPE_BATTERY;
#endif

#ifndef CONFIG_LGE_PM
	power_supply_set_supply_type(dotg->psy, power_supply_type);
#endif

#if defined (CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI4)
	update_status(1, dotg->charger->chg_type);
#endif

#if defined(CONFIG_DWC3_MSM_BC_12_VZW_SUPPORT) && defined(CONFIG_LGE_PM)
	if (!chglogo_check && lge_get_boot_mode() == LGE_BOOT_MODE_CHARGERLOGO &&
			dotg->charger->chg_type == DWC3_SDP_CHARGER) {
		if (mA > IUNIT)
			chglogo_check = true;
		else if (mA <= 2) {
			dotg->charger->max_power = mA;
			return 0;
		}
	}

	if (mA > 2 && lge_pm_get_cable_type() != NO_INIT_CABLE) {
		if (dotg->charger->chg_type == DWC3_DCP_CHARGER)
			mA = lge_pm_get_ta_current();
	}
#elif defined(CONFIG_LGE_PM)
	if (mA > 2 && lge_pm_get_cable_type() != NO_INIT_CABLE) {
		if (dotg->charger->chg_type == DWC3_SDP_CHARGER) {
			if (dotg->dwc->gadget.speed == USB_SPEED_SUPER) {
				if (dotg->charger->max_power > 2)
					dotg->charger->max_power = 0;
				mA = DWC3_USB30_CHG_CURRENT;
			} else {
				mA = lge_pm_get_usb_current();
			}
#ifdef CONFIG_QPNP_CHARGER
			/*                                                      */
			if (pseudo_batt_info.mode && mA == 500 )
				mA = DWC3_USB30_CHG_CURRENT;
#endif
		} else if (dotg->charger->chg_type == DWC3_DCP_CHARGER) {
			mA = lge_pm_get_ta_current();
		} else if (dotg->charger->chg_type == DWC3_FLOATED_CHARGER) {
			mA = lge_pm_get_usb_current();
		}
	}
#endif

	if (dotg->charger->chg_type == DWC3_CDP_CHARGER)
		mA = DWC3_IDEV_CHG_MAX;

	if (dotg->charger->max_power == mA)
		return 0;

	dev_info(phy->dev, "Avail curr from USB = %u\n", mA);

/*                                                                                           */
#ifdef CONFIG_LGE_PM
#ifndef CONFIG_USB_DWC3_LGE_SINGLE_PSY
	if (dwc3_otg_get_psy(phy) < 0)
		goto psy_error;
#else
	if (strcmp(dotg->psy->name, "usb")) {
		pr_info("%s psy name is %s, so change psy to usb.\n", __func__, dotg->psy->name);
		dotg->psy = power_supply_get_by_name("usb");
		if (!dotg->psy)
			goto psy_error;
	}
#endif
	power_supply_set_supply_type(dotg->psy, power_supply_type);
#endif

	if (dotg->charger->max_power <= 2 && mA > 2) {
		/*                 */
		if (power_supply_set_online(dotg->psy, true))
			goto psy_error;
		if (power_supply_set_current_limit(dotg->psy, 1000*mA))
			goto psy_error;
#ifdef CONFIG_QPNP_CHARGER
		if (!strncmp(dotg->psy->name, "ac", 2)) {
			dotg->psy = power_supply_get_by_name("usb");
			if (!dotg->psy)
				goto psy_error;

			if (power_supply_set_online(dotg->psy, true))
				goto psy_error;

			if (power_supply_set_supply_type(dotg->psy, power_supply_type))
				goto psy_error;

			if (power_supply_set_current_limit(dotg->psy, 1000*mA))
				goto psy_error;

			dotg->psy = power_supply_get_by_name("ac");
			if (!dotg->psy)
				goto psy_error;
		}
#endif
	} else if (dotg->charger->max_power > 0 && (mA == 0 || mA == 2)) {
		/*                  */
		if (power_supply_set_online(dotg->psy, false))
			goto psy_error;
		/*                       */
		if (power_supply_set_current_limit(dotg->psy, 0))
			goto psy_error;

#ifdef CONFIG_QPNP_CHARGER
		if (!strncmp(dotg->psy->name, "ac", 2)) {
			dotg->psy = power_supply_get_by_name("usb");
			if (!dotg->psy)
				goto psy_error;

			if (power_supply_set_online(dotg->psy, false))
				goto psy_error;

			if (power_supply_set_supply_type(dotg->psy, power_supply_type))
				goto psy_error;

			if (power_supply_set_current_limit(dotg->psy, 0))
				goto psy_error;

			dotg->psy = power_supply_get_by_name("ac");
			if (!dotg->psy)
				goto psy_error;
		}
#endif
	}

	power_supply_changed(dotg->psy);

	dotg->charger->max_power = mA;

	return 0;

psy_error:
	dev_dbg(phy->dev, "power supply error when setting property\n");
	return -ENXIO;
}

/*                                                 */
#define DWC3_OEVT_MASK		(DWC3_OEVTEN_OTGCONIDSTSCHNGEVNT | \
				 DWC3_OEVTEN_OTGBDEVVBUSCHNGEVNT)

/* 
                                                              
                                                      
  
                                                     
 */
static irqreturn_t dwc3_otg_interrupt(int irq, void *_dotg)
{
	struct dwc3_otg *dotg = (struct dwc3_otg *)_dotg;
	u32 osts, oevt_reg;
	int ret = IRQ_NONE;
	int handled_irqs = 0;
	struct usb_phy *phy = dotg->otg.phy;

	oevt_reg = dwc3_readl(dotg->regs, DWC3_OEVT);

	if (!(oevt_reg & DWC3_OEVT_MASK))
		return IRQ_NONE;

	osts = dwc3_readl(dotg->regs, DWC3_OSTS);

	if ((oevt_reg & DWC3_OEVTEN_OTGCONIDSTSCHNGEVNT) ||
	    (oevt_reg & DWC3_OEVTEN_OTGBDEVVBUSCHNGEVNT)) {
		/*
                                                           
                                                 
   */

		if (oevt_reg & DWC3_OEVTEN_OTGCONIDSTSCHNGEVNT) {
			if (osts & DWC3_OTG_OSTS_CONIDSTS) {
				dev_dbg(phy->dev, "ID set\n");
				set_bit(ID, &dotg->inputs);
			} else {
				dev_dbg(phy->dev, "ID clear\n");
				clear_bit(ID, &dotg->inputs);
			}
			handled_irqs |= DWC3_OEVTEN_OTGCONIDSTSCHNGEVNT;
		}

		if (oevt_reg & DWC3_OEVTEN_OTGBDEVVBUSCHNGEVNT) {
			if (osts & DWC3_OTG_OSTS_BSESVALID) {
				dev_dbg(phy->dev, "BSV set\n");
				set_bit(B_SESS_VLD, &dotg->inputs);
			} else {
				dev_dbg(phy->dev, "BSV clear\n");
				clear_bit(B_SESS_VLD, &dotg->inputs);
			}
			handled_irqs |= DWC3_OEVTEN_OTGBDEVVBUSCHNGEVNT;
		}

		queue_delayed_work(system_nrt_wq, &dotg->sm_work, 0);

		ret = IRQ_HANDLED;

		/*                                 */
		dwc3_writel(dotg->regs, DWC3_OEVT, handled_irqs);
	}

	return ret;
}

/* 
                                                       
                                           
  
 */
void dwc3_otg_init_sm(struct dwc3_otg *dotg)
{
	u32 osts = dwc3_readl(dotg->regs, DWC3_OSTS);
	struct usb_phy *phy = dotg->otg.phy;
	struct dwc3_ext_xceiv *ext_xceiv;
	int ret;

	dev_dbg(phy->dev, "Initialize OTG inputs, osts: 0x%x\n", osts);

	/*
                                             
                                       
  */
	ret = wait_for_completion_timeout(&dotg->dwc3_xcvr_vbus_init, HZ * 5);
	if (!ret) {
		dev_err(phy->dev, "%s: completion timeout\n", __func__);
		/*                                         */
		set_bit(ID, &dotg->inputs);
	}

	ext_xceiv = dotg->ext_xceiv;
	dwc3_otg_reset(dotg);
	if (ext_xceiv && !ext_xceiv->otg_capability) {
		if (osts & DWC3_OTG_OSTS_CONIDSTS)
			set_bit(ID, &dotg->inputs);
		else
			clear_bit(ID, &dotg->inputs);

		if (osts & DWC3_OTG_OSTS_BSESVALID)
			set_bit(B_SESS_VLD, &dotg->inputs);
		else
			clear_bit(B_SESS_VLD, &dotg->inputs);
	}
}

/* 
                                         
  
                                        
  
                                        
                                       
 */
static void dwc3_otg_sm_work(struct work_struct *w)
{
	struct dwc3_otg *dotg = container_of(w, struct dwc3_otg, sm_work.work);
	struct usb_phy *phy = dotg->otg.phy;
	struct dwc3_charger *charger = dotg->charger;
	bool work = 0;
	int ret = 0;
	unsigned long delay = 0;

	pm_runtime_resume(phy->dev);
	dev_dbg(phy->dev, "%s state\n", otg_state_string(phy->state));

	/*                 */
	switch (phy->state) {
	case OTG_STATE_UNDEFINED:
		dwc3_otg_init_sm(dotg);
		if (!dotg->psy) {
			dotg->psy = power_supply_get_by_name("usb");

			if (!dotg->psy)
				dev_err(phy->dev,
					 "couldn't get usb power supply\n");
		}

		/*                                               */
		if (!test_bit(ID, &dotg->inputs)) {
			dev_dbg(phy->dev, "!id\n");
			phy->state = OTG_STATE_A_IDLE;
			work = 1;
		} else if (test_bit(B_SESS_VLD, &dotg->inputs)) {
			dev_dbg(phy->dev, "b_sess_vld\n");
			phy->state = OTG_STATE_B_IDLE;
			work = 1;
		} else {
			phy->state = OTG_STATE_B_IDLE;
			dev_dbg(phy->dev, "No device, trying to suspend\n");
			pm_runtime_put_sync(phy->dev);
		}
		break;

	case OTG_STATE_B_IDLE:
		if (!test_bit(ID, &dotg->inputs)) {
			dev_dbg(phy->dev, "!id\n");
			phy->state = OTG_STATE_A_IDLE;
			work = 1;
			dotg->charger_retry_count = 0;
			if (charger) {
				if (charger->chg_type == DWC3_INVALID_CHARGER)
					charger->start_detection(dotg->charger,
									false);
				else
					charger->chg_type =
							DWC3_INVALID_CHARGER;
			}
		} else if (test_bit(B_SESS_VLD, &dotg->inputs)) {
			dev_dbg(phy->dev, "b_sess_vld\n");
			if (charger) {
				/*                                            */
				switch (charger->chg_type) {
				case DWC3_PROPRIETARY_CHARGER:
#if defined(CONFIG_LGE_PM)
					dwc3_otg_set_power(phy,
							DWC3_IDEV_CHG_PROPRIETARY_MAX);
					pm_runtime_put_sync(phy->dev);
					break;
#endif
				case DWC3_DCP_CHARGER:
					dev_dbg(phy->dev, "lpm, DCP charger\n");
					dwc3_otg_set_power(phy,
							DWC3_IDEV_CHG_MAX);
					pm_runtime_put_sync(phy->dev);
					break;
				case DWC3_CDP_CHARGER:
					dwc3_otg_set_power(phy,
							DWC3_IDEV_CHG_MAX);
					dwc3_otg_start_peripheral(&dotg->otg,
									1);
					phy->state = OTG_STATE_B_PERIPHERAL;
					work = 1;
					break;
				case DWC3_SDP_CHARGER:
					dwc3_otg_set_power(phy,
								IUNIT);
					dwc3_otg_start_peripheral(&dotg->otg,
									1);
					phy->state = OTG_STATE_B_PERIPHERAL;
					work = 1;
					break;
				case DWC3_FLOATED_CHARGER:
					if (dotg->charger_retry_count <
							max_chgr_retry_count)
						dotg->charger_retry_count++;
					/*
                                       
                                            
                                          
                                          
                                          
                                            
                                    
      */
					if (dotg->charger_retry_count ==
						max_chgr_retry_count) {
#ifdef CONFIG_LGE_PM
						power_supply_set_floated_charger(dotg->psy, 1);
#endif
#if defined (CONFIG_SLIMPORT_ANX7816) || defined(CONFIG_SLIMPORT_ANX7808)
						if (slimport_is_connected()) {
							dwc3_otg_set_power(phy, IDEV_CHG_MIN);
							pm_runtime_put_sync(phy->dev);
							break;
						}
#endif
#ifdef CONFIG_LGE_PM
#ifdef CONFIG_DWC3_MSM_BC_12_VZW_SUPPORT
						queue_delayed_work(system_nrt_wq, dotg->charger->drv_check_state_wq, 0);
						dwc3_otg_set_power(phy, 0);
						dwc3_otg_start_peripheral(&dotg->otg, 1);
						phy->state = OTG_STATE_B_PERIPHERAL;
						work = 1;
#else
						dwc3_otg_set_power(phy,	IUNIT);
						dwc3_otg_start_peripheral(&dotg->otg, 1);
						phy->state = OTG_STATE_B_PERIPHERAL;
						work = 1;
#endif
#else
						dwc3_otg_set_power(phy, 0);
						pm_runtime_put_sync(phy->dev);
#endif
						break;
					}
					charger->start_detection(dotg->charger,
									false);

				default:
					dev_dbg(phy->dev, "chg_det started\n");
					charger->start_detection(charger, true);
					break;
				}
			} else {
				/*                                         */
				if (dwc3_otg_start_peripheral(&dotg->otg, 1)) {
					/*
                                          
                                         
                      
      */
					dev_err(phy->dev, "enter lpm as\n"
						"unable to start B-device\n");
					phy->state = OTG_STATE_UNDEFINED;
					pm_runtime_put_sync(phy->dev);
					return;
				}
			}
		} else {
			if (charger)
				charger->start_detection(dotg->charger, false);
#ifdef CONFIG_LGE_PM
			if (!dotg->psy)
				dotg->psy = power_supply_get_by_name("usb");
			if (dotg->psy)
				power_supply_set_floated_charger(dotg->psy, 0);
#endif
			dotg->charger_retry_count = 0;
			dwc3_otg_set_power(phy, 0);
			dev_dbg(phy->dev, "No device, trying to suspend\n");
			pm_runtime_put_sync(phy->dev);
		}
		break;

	case OTG_STATE_B_PERIPHERAL:
		if (!test_bit(B_SESS_VLD, &dotg->inputs) ||
				!test_bit(ID, &dotg->inputs)) {
			dev_dbg(phy->dev, "!id || !bsv\n");
			dwc3_otg_start_peripheral(&dotg->otg, 0);
			phy->state = OTG_STATE_B_IDLE;
#if defined(CONFIG_DWC3_MSM_CDP_SUPPORT)
			if ((charger) && (charger->chg_type != DWC3_CDP_CHARGER)) {
				charger->chg_type = DWC3_INVALID_CHARGER;
			}
#else
			if (charger)
				charger->chg_type = DWC3_INVALID_CHARGER;
#endif
			work = 1;
		}
		break;

	case OTG_STATE_A_IDLE:
		/*                   */
		if (test_bit(ID, &dotg->inputs)) {
			dev_dbg(phy->dev, "id\n");
			phy->state = OTG_STATE_B_IDLE;
			dotg->vbus_retry_count = 0;
			work = 1;
		} else {
			phy->state = OTG_STATE_A_HOST;
#ifdef CONFIG_LGE_PM
			/*
                                        
                                                              
    */
			dotg->psy = power_supply_get_by_name("usb");
			if (!dotg->psy)
				dev_err(phy->dev, "couldn't get usb power supply\n");
#endif
			ret = dwc3_otg_start_host(&dotg->otg, 1);
			if ((ret == -EPROBE_DEFER) &&
						dotg->vbus_retry_count < 3) {
				/*
                                                  
                                                    
     */
				phy->state = OTG_STATE_A_IDLE;
				dev_dbg(phy->dev, "Unable to get vbus regulator. Retrying...\n");
				delay = VBUS_REG_CHECK_DELAY;
				work = 1;
				dotg->vbus_retry_count++;
			} else if (ret) {
				/*
                                            
                                                  
     */
				dev_dbg(phy->dev, "enter lpm as\n"
					"unable to start A-device\n");
				phy->state = OTG_STATE_A_IDLE;
				pm_runtime_put_sync(phy->dev);
				return;
			}
		}
		break;

	case OTG_STATE_A_HOST:
		if (test_bit(ID, &dotg->inputs)) {
			dev_dbg(phy->dev, "id\n");
			dwc3_otg_start_host(&dotg->otg, 0);
			phy->state = OTG_STATE_B_IDLE;
			dotg->vbus_retry_count = 0;
			work = 1;
		}
		break;

	default:
		dev_err(phy->dev, "%s: invalid otg-state\n", __func__);

	}

	if (work)
		queue_delayed_work(system_nrt_wq, &dotg->sm_work, delay);
}


/* 
                                             
  
                                        
 */
static void dwc3_otg_reset(struct dwc3_otg *dotg)
{
	static int once;
	struct dwc3_ext_xceiv *ext_xceiv = dotg->ext_xceiv;

	/*
                             
                        
                        
  */
	if (ext_xceiv && !ext_xceiv->otg_capability)
		dwc3_writel(dotg->regs, DWC3_OCFG, 0x4);

	/*
                          
                           
                        
                        
                                
                             
                             
  */
	if (!once) {
		if (ext_xceiv && !ext_xceiv->otg_capability)
			dwc3_writel(dotg->regs, DWC3_OCTL, 0x40);
		once++;
	}

	/*                                                */
	dwc3_writel(dotg->regs, DWC3_OEVT, 0xFFFF);

	/*                              */
	if (ext_xceiv && !ext_xceiv->otg_capability)
		dwc3_writel(dotg->regs, DWC3_OEVTEN,
				DWC3_OEVTEN_OTGCONIDSTSCHNGEVNT |
				DWC3_OEVTEN_OTGBDEVVBUSCHNGEVNT);
}

/* 
                                                    
                                                    
  
                                                 
 */
int dwc3_otg_init(struct dwc3 *dwc)
{
	u32	reg;
	int ret = 0;
	struct dwc3_otg *dotg;

	dev_dbg(dwc->dev, "dwc3_otg_init\n");

	/*
                                     
                                          
  */
	reg = dwc3_readl(dwc->regs, DWC3_GHWPARAMS6);
	if (!(reg & DWC3_GHWPARAMS6_SRP_SUPPORT)) {
		/*
                                   
                                                               
                                                                
   */
		dev_dbg(dwc->dev, "dwc3_otg address space is not supported\n");
		return 0;
	}

	/*                                */
	dotg = kzalloc(sizeof(struct dwc3_otg), GFP_KERNEL);
	if (!dotg) {
		dev_err(dwc->dev, "unable to allocate dwc3_otg\n");
		return -ENOMEM;
	}

	/*                                                         */
	dotg->irq = platform_get_irq_byname(to_platform_device(dwc->dev),
								"otg_irq");
	if (dotg->irq < 0) {
		dev_err(dwc->dev, "%s: missing OTG IRQ\n", __func__);
		ret = -ENODEV;
		goto err1;
	}

	dotg->regs = dwc->regs;

	dotg->otg.set_peripheral = dwc3_otg_set_peripheral;
	dotg->otg.set_host = dwc3_otg_set_host;

	/*                                                                   */
	dwc->dotg = dotg;

	dotg->otg.phy = kzalloc(sizeof(struct usb_phy), GFP_KERNEL);
	if (!dotg->otg.phy) {
		dev_err(dwc->dev, "unable to allocate dwc3_otg.phy\n");
		ret = -ENOMEM;
		goto err1;
	}

	dotg->dwc = dwc;
	dotg->otg.phy->otg = &dotg->otg;
	dotg->otg.phy->dev = dwc->dev;
	dotg->otg.phy->set_power = dwc3_otg_set_power;
	dotg->otg.phy->set_suspend = dwc3_otg_set_suspend;
	ret = usb_set_transceiver(dotg->otg.phy);
	if (ret) {
		dev_err(dotg->otg.phy->dev,
			"%s: failed to set transceiver, already exists\n",
			__func__);
		goto err2;
	}

	dotg->otg.phy->state = OTG_STATE_UNDEFINED;

	init_completion(&dotg->dwc3_xcvr_vbus_init);
	INIT_DELAYED_WORK(&dotg->sm_work, dwc3_otg_sm_work);

	ret = request_irq(dotg->irq, dwc3_otg_interrupt, IRQF_SHARED,
				"dwc3_otg", dotg);
	if (ret) {
		dev_err(dotg->otg.phy->dev, "failed to request irq #%d --> %d\n",
				dotg->irq, ret);
		goto err3;
	}

	pm_runtime_get(dwc->dev);

	return 0;

err3:
	cancel_delayed_work_sync(&dotg->sm_work);
	usb_set_transceiver(NULL);
err2:
	kfree(dotg->otg.phy);
err1:
	dwc->dotg = NULL;
	kfree(dotg);

	return ret;
}

/* 
                
                                                    
  
                                                 
 */
void dwc3_otg_exit(struct dwc3 *dwc)
{
	struct dwc3_otg *dotg = dwc->dotg;

	/*                                                                  */
	if (dotg) {
		if (dotg->charger)
			dotg->charger->start_detection(dotg->charger, false);
		cancel_delayed_work_sync(&dotg->sm_work);
		usb_set_transceiver(NULL);
		pm_runtime_put(dwc->dev);
		free_irq(dotg->irq, dotg);
		kfree(dotg->otg.phy);
		kfree(dotg);
		dwc->dotg = NULL;
	}
}
