/*
 * MAX14688 - Headset Jack Dectection IC with Accessory Power
 *
 * Copyright (C) 2013 Maxim Integrated
 * Jayden Cha <jayden.cha@maximintegrated.com>
 *
 * Copyright and License statement to be determined with Customer.
 * GNU Public License version 2 requires software code to be
 * publically open source if the code is to be statically linked with
 * the Linux kernel binary object.
 */

#ifndef __MAX14688_H__
#define __MAX14688_H__

#define MAX14688_NAME                    "max14688"
#define MAX14688_I2C_ADDR                0x15

/*              */
#define MAX14688_MODE_LOW                0b00
#define MAX14688_MODE_HIGH_Z             0b01
//                                           
#define MAX14688_MODE_HIGH               0b11

/*               */
#define SW_ADVANCED_HEADPHONE_INSERT	0x11
#define SW_AUX_ACCESSORY_INSERT		0x12

/*             */
struct max14688_match_key {
    /*                 */
    int min_Z, max_Z; /*                                         */
};

/*                                          */
struct max14688_jack_match {
    char                      *name;

    /*              */
    struct max14688_match_key  mic;              /*                     */
    struct max14688_match_key  left;             /*                        */

    /*                                       */
    char                       *switch_name;
    unsigned int               switch_state;

    /*                                */
    unsigned int               evt_type, evt_code1, evt_code2;

    /*                 */
    bool                       has_button;       /*                   */
    u8                         mode0, mode1;     /*              */
};

/*                                            */
struct max14688_button_match {
    char                      *name;

    /*              */
    struct max14688_match_key  mic;              /*                     */
    struct max14688_match_key  left;             /*                        */

    /*                                */
    unsigned int               evt_type, evt_code;
};

/*                  
                                       
                                                            
                                   
                                                                     
                           
                                                                     
                           
 */
#define MAX14688_ADCCTL_OFF              0b00
#define MAX14688_ADCCTL_CONTINUOUS       0b01
#define MAX14688_ADCCTL_SINGLE_OF_100MS  0b10
#define MAX14688_ADCCTL_SINGLE_OF_200MS  0b11

/*                                            */
#define MAX14688_JACK_IN_VALUE           1
#define MAX14688_JACK_OUT_VALUE          0
#define MAX14688_BUTTON_DOWN_VALUE       1
#define MAX14688_BUTTON_UP_VALUE         0

/*               */
struct max14688_platform_data {
    const char             *switch_name;
    const char             *switch_name_advanced;
    const char             *switch_name_aux;
    unsigned int           gpio_detect;
    unsigned int           gpio_int;
    unsigned int           gpio_mic_en;
    unsigned int           gpio_mic_en_2;

    /*                */
    struct max14688_jack_match    *jack_matches;
    int                            num_of_jack_matches;
    struct max14688_button_match  *button_matches;
    int                            num_of_button_matches;

    /*                              */
    bool (*detect_jack) (struct device *dev);
    int (*read_mic_impedence) (struct device *dev);
    int (*read_left_impedence) (struct device *dev);

    /*                               */
    void (*report_jack) (struct device *dev,
        struct max14688_jack_match *match, int value);
    void (*report_button) (struct device *dev,
        struct max14688_button_match *match, int value);
};

/*  
                      
   */

struct input_dev;
extern struct device *max14688_device (struct input_dev *input_dev);

extern struct input_dev *max14688_input_device (struct device *dev);
struct qpnp_vadc_chip *input_vadc = NULL;
#endif /*                 */
