/*
 * Maxim MAX77819 Charger Driver Header
 *
 * Copyright (C) 2013 Maxim Integrated Product
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __MAX77819_CHARGER_H__
#define __MAX77819_CHARGER_H__

/*                           */
#define MAX77819_CHARGER_CURRENT_SUSPEND  (0)
#define MAX77819_CHARGER_CURRENT_UNLIMIT  (1)
#define MAX77819_CHARGER_CURRENT_MAXIMUM  (2)
#define MAX77819_CHARGER_CURRENT_MINIMUM  (3)

struct max77819_charger_platform_data {
    bool disable_interrupt;
    int irq;

    char *psy_name;
    char *ext_psy_name;

    char **supplied_to;
    size_t num_supplicants;

    u32 fast_charge_current;        /*                      */
    u32 charge_termination_voltage; /*                      */
    u32 topoff_timer;               /*                                */
    u32 topoff_current;             /*                     */
    u32 charge_restart_threshold;   /*                      */

    /*                      */
    bool enable_coop;
    char *coop_psy_name;

    /*                        */
    bool enable_thermistor;

    /*              */
    bool enable_aicl;
    u32 aicl_detection_voltage;     /*                     */
    u32 aicl_reset_threshold;       /*                       */
    int current_limit_usb;
    int current_limit_ac;
	int otg_en;
};

/*                                                           */
#if !defined(CONFIG_LGE_PM)
#ifndef POWER_SUPPLY_PROP_CHARGING_ENABLED
#define POWER_SUPPLY_PROP_CHARGING_ENABLED_REPLACED
#define POWER_SUPPLY_PROP_CHARGING_ENABLED \
        POWER_SUPPLY_PROP_ONLINE
#endif
#endif
#ifndef POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT
#define POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT \
        POWER_SUPPLY_PROP_CURRENT_NOW
#endif
#ifndef POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX
#define POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX \
        POWER_SUPPLY_PROP_CURRENT_MAX
#endif

#endif /*                         */
