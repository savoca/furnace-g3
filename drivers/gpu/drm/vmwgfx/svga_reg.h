/**********************************************************
 * Copyright 1998-2009 VMware, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************/

/*
                
  
                                                                 
 */

#ifndef _SVGA_REG_H_
#define _SVGA_REG_H_

/*
                  
 */
#define PCI_VENDOR_ID_VMWARE            0x15AD
#define PCI_DEVICE_ID_VMWARE_SVGA2      0x0405

/*
                                   
 */
#define SVGA_REG_ENABLE_DISABLE     0
#define SVGA_REG_ENABLE_ENABLE      1
#define SVGA_REG_ENABLE_HIDE        2
#define SVGA_REG_ENABLE_ENABLE_HIDE (SVGA_REG_ENABLE_ENABLE |\
				     SVGA_REG_ENABLE_HIDE)

/*
                                                                    
                                                                
                         
 */
#define SVGA_CURSOR_ON_HIDE            0x0   /*                                              */
#define SVGA_CURSOR_ON_SHOW            0x1   /*                                              */
#define SVGA_CURSOR_ON_REMOVE_FROM_FB  0x2   /*                                                                               */
#define SVGA_CURSOR_ON_RESTORE_TO_FB   0x3   /*                                                               */

/*
                                                                             
                                                                               
                                                                         
                                                                            
        
  
                                                                           
                      
 */
#define SVGA_FB_MAX_TRACEABLE_SIZE      0x1000000

#define SVGA_MAX_PSEUDOCOLOR_DEPTH      8
#define SVGA_MAX_PSEUDOCOLORS           (1 << SVGA_MAX_PSEUDOCOLOR_DEPTH)
#define SVGA_NUM_PALETTE_REGS           (3 * SVGA_MAX_PSEUDOCOLORS)

#define SVGA_MAGIC         0x900000UL
#define SVGA_MAKE_ID(ver)  (SVGA_MAGIC << 8 | (ver))

/*                                                                    */
#define SVGA_VERSION_2     2
#define SVGA_ID_2          SVGA_MAKE_ID(SVGA_VERSION_2)

/*                                                                   
                          */
#define SVGA_VERSION_1     1
#define SVGA_ID_1          SVGA_MAKE_ID(SVGA_VERSION_1)

/*                                  */
#define SVGA_VERSION_0     0
#define SVGA_ID_0          SVGA_MAKE_ID(SVGA_VERSION_0)

/*                                                                                 */
#define SVGA_ID_INVALID    0xFFFFFFFF

/*                                */
#define SVGA_INDEX_PORT         0x0
#define SVGA_VALUE_PORT         0x1
#define SVGA_BIOS_PORT          0x2
#define SVGA_IRQSTATUS_PORT     0x8

/*
                                                         
  
                                         
                                          
 */
#define SVGA_IRQFLAG_ANY_FENCE            0x1    /*                      */
#define SVGA_IRQFLAG_FIFO_PROGRESS        0x2    /*                                   */
#define SVGA_IRQFLAG_FENCE_GOAL           0x4    /*                              */

/*
            
 */

enum {
   SVGA_REG_ID = 0,
   SVGA_REG_ENABLE = 1,
   SVGA_REG_WIDTH = 2,
   SVGA_REG_HEIGHT = 3,
   SVGA_REG_MAX_WIDTH = 4,
   SVGA_REG_MAX_HEIGHT = 5,
   SVGA_REG_DEPTH = 6,
   SVGA_REG_BITS_PER_PIXEL = 7,       /*                          */
   SVGA_REG_PSEUDOCOLOR = 8,
   SVGA_REG_RED_MASK = 9,
   SVGA_REG_GREEN_MASK = 10,
   SVGA_REG_BLUE_MASK = 11,
   SVGA_REG_BYTES_PER_LINE = 12,
   SVGA_REG_FB_START = 13,            /*              */
   SVGA_REG_FB_OFFSET = 14,
   SVGA_REG_VRAM_SIZE = 15,
   SVGA_REG_FB_SIZE = 16,

   /*                                                                    */

   SVGA_REG_CAPABILITIES = 17,
   SVGA_REG_MEM_START = 18,           /*              */
   SVGA_REG_MEM_SIZE = 19,
   SVGA_REG_CONFIG_DONE = 20,         /*                                 */
   SVGA_REG_SYNC = 21,                /*                                      */
   SVGA_REG_BUSY = 22,                /*                                      */
   SVGA_REG_GUEST_ID = 23,            /*                         */
   SVGA_REG_CURSOR_ID = 24,           /*              */
   SVGA_REG_CURSOR_X = 25,            /*              */
   SVGA_REG_CURSOR_Y = 26,            /*              */
   SVGA_REG_CURSOR_ON = 27,           /*              */
   SVGA_REG_HOST_BITS_PER_PIXEL = 28, /*              */
   SVGA_REG_SCRATCH_SIZE = 29,        /*                             */
   SVGA_REG_MEM_REGS = 30,            /*                          */
   SVGA_REG_NUM_DISPLAYS = 31,        /*              */
   SVGA_REG_PITCHLOCK = 32,           /*                           */
   SVGA_REG_IRQMASK = 33,             /*                */

   /*                              */
   SVGA_REG_NUM_GUEST_DISPLAYS = 34,/*                                           */
   SVGA_REG_DISPLAY_ID = 35,        /*                                                 */
   SVGA_REG_DISPLAY_IS_PRIMARY = 36,/*                                   */
   SVGA_REG_DISPLAY_POSITION_X = 37,/*                        */
   SVGA_REG_DISPLAY_POSITION_Y = 38,/*                        */
   SVGA_REG_DISPLAY_WIDTH = 39,     /*                     */
   SVGA_REG_DISPLAY_HEIGHT = 40,    /*                      */

   /*                                   */
   SVGA_REG_GMR_ID = 41,
   SVGA_REG_GMR_DESCRIPTOR = 42,
   SVGA_REG_GMR_MAX_IDS = 43,
   SVGA_REG_GMR_MAX_DESCRIPTOR_LENGTH = 44,

   SVGA_REG_TRACES = 45,            /*                                                 */
   SVGA_REG_GMRS_MAX_PAGES = 46,    /*                                          */
   SVGA_REG_MEMORY_SIZE = 47,       /*                                              */
   SVGA_REG_TOP = 48,               /*                                       */

   SVGA_PALETTE_BASE = 1024,        /*                        */
   /*                                                  */

   SVGA_SCRATCH_BASE = SVGA_PALETTE_BASE + SVGA_NUM_PALETTE_REGS
                                    /*                           */
   /*                                                                   
                                                                         
                                          */
};


/*
                               
  
                                                                 
                                                                  
                                                                      
                                                                    
                                                                
                                                                     
                                                                   
                       
  
                                                              
                                                  
  
                       
  
                  
                                                                   
                                                                     
                           
  
                               
  
                  
                                                                     
                                                                    
                                                                      
                                                          
                                 
  
                                                                
                                                                    
                                                                
                                                                  
                                                                    
                                                    
  
                                                                
                               
  
                            
  
                 
                                                                
                                                                        
                                             
  
                                          
  
                 
                                                                    
                                                                      
                    
  
                                                                 
                                                         
  
                                         
                            
  
                                                                     
                                
  
                                                                      
                                                                   
                                                       
                                
  
                                                                    
                                                                    
                                                                      
                                                                   
                                                         
  
                                                                   
                                                                    
                                                                   
       
  
                                                                  
             
 */

/*
                                                                  
                                                                  
                                                                    
  
                                                                    
                                                                    
            
 */
#define SVGA_GMR_NULL         ((uint32) -1)
#define SVGA_GMR_FRAMEBUFFER  ((uint32) -2)  /*                         */

typedef
struct SVGAGuestMemDescriptor {
   uint32 ppn;
   uint32 numPages;
} SVGAGuestMemDescriptor;

typedef
struct SVGAGuestPtr {
   uint32 gmrId;
   uint32 offset;
} SVGAGuestPtr;


/*
                        
  
                                                                   
                                                                      
                                                                      
                                                                      
                                       
  
                                  
  
                               
                               
                               
                              
                             
                             
  
 */

typedef
struct SVGAGMRImageFormat {
   union {
      struct {
         uint32 bitsPerPixel : 8;
         uint32 colorDepth   : 8;
         uint32 reserved     : 16;  /*              */
      };

      uint32 value;
   };
} SVGAGMRImageFormat;

typedef
struct SVGAGuestImage {
   SVGAGuestPtr         ptr;

   /*
                                                                   
                                                       
                                                                    
                                                                
                                                             
                                                            
     
                                                                                
     
                                                      
     
                                                                    
                                                    
    */
   uint32 pitch;
} SVGAGuestImage;

/*
                   
  
                                                                
                                                                 
                  
 */

typedef
struct SVGAColorBGRX {
   union {
      struct {
         uint32 b : 8;
         uint32 g : 8;
         uint32 r : 8;
         uint32 x : 8;  /*        */
      };

      uint32 value;
   };
} SVGAColorBGRX;


/*
                    
                     
  
                                                                      
                                                                     
                                      
  
                                                                    
                                                                      
          
 */

typedef
struct SVGASignedRect {
   int32  left;
   int32  top;
   int32  right;
   int32  bottom;
} SVGASignedRect;

typedef
struct SVGASignedPoint {
   int32  x;
   int32  y;
} SVGASignedPoint;


/*
                
  
                                                                      
                                                                     
                                       
  
                   
                                                                     
                                                                 
                           
  
                              
                                                                      
                                   
 */

#define SVGA_CAP_NONE               0x00000000
#define SVGA_CAP_RECT_COPY          0x00000002
#define SVGA_CAP_CURSOR             0x00000020
#define SVGA_CAP_CURSOR_BYPASS      0x00000040   /*                                      */
#define SVGA_CAP_CURSOR_BYPASS_2    0x00000080   /*                                      */
#define SVGA_CAP_8BIT_EMULATION     0x00000100
#define SVGA_CAP_ALPHA_CURSOR       0x00000200
#define SVGA_CAP_3D                 0x00004000
#define SVGA_CAP_EXTENDED_FIFO      0x00008000
#define SVGA_CAP_MULTIMON           0x00010000   /*                              */
#define SVGA_CAP_PITCHLOCK          0x00020000
#define SVGA_CAP_IRQMASK            0x00040000
#define SVGA_CAP_DISPLAY_TOPOLOGY   0x00080000   /*                              */
#define SVGA_CAP_GMR                0x00100000
#define SVGA_CAP_TRACES             0x00200000
#define SVGA_CAP_GMR2               0x00400000
#define SVGA_CAP_SCREEN_OBJECT_2    0x00800000


/*
                         
  
                                                                      
                                     
  
                                                              
                                                                      
                                                                       
                                                                   
                    
                                                                
                                                       
 */

enum {
   /*
                                                                       
                                                                      
    */

   SVGA_FIFO_MIN = 0,
   SVGA_FIFO_MAX,       /*                                                   */
   SVGA_FIFO_NEXT_CMD,
   SVGA_FIFO_STOP,

   /*
                                                                        
                                                           
                                                                  
                                           
     
                                                                       
                                                                       
                                                                       
                                                                        
                                                                         
                                                                     
    */

   SVGA_FIFO_CAPABILITIES = 4,
   SVGA_FIFO_FLAGS,
   /*                                 */
   SVGA_FIFO_FENCE,

   /*
                                                                          
                                                             
                                                                            
                          
     
                                                                      
                    
    */

   /*                                                  */
   SVGA_FIFO_3D_HWVERSION,       /*                                           */
   /*                                     */
   SVGA_FIFO_PITCHLOCK,

   /*                                           */
   SVGA_FIFO_CURSOR_ON,          /*                                    */
   SVGA_FIFO_CURSOR_X,           /*                            */
   SVGA_FIFO_CURSOR_Y,           /*                            */
   SVGA_FIFO_CURSOR_COUNT,       /*                                            */
   SVGA_FIFO_CURSOR_LAST_UPDATED,/*                                       */

   /*                                   */
   SVGA_FIFO_RESERVED,           /*                                        */

   /*
                                                                              
     
                                                                     
                                                                     
                                                                      
                                                          
    */
   SVGA_FIFO_CURSOR_SCREEN_ID,

   /*
                                   
     
                                                                        
    */
   SVGA_FIFO_DEAD,

   /*
                                                    
     
                                                                       
                                                             
    */
   SVGA_FIFO_3D_HWVERSION_REVISED,

   /*
                                                                        
                                                                          
                                                                      
                                                                           
                                                                            
                                                              
                 
               
                                  
                
                                                               
                                                                          
                                                                
                    
    */

   /*
                                                            
                              
    */
   SVGA_FIFO_3D_CAPS      = 32,
   SVGA_FIFO_3D_CAPS_LAST = 32 + 255,

   /*
                                                                   
                                                                          
                                                                           
                             
     
                                                                            
                                                                      
                                       
     
                                                                     
                                                            
    */

   /*                           */
   SVGA_FIFO_GUEST_3D_HWVERSION, /*                           */
   SVGA_FIFO_FENCE_GOAL,         /*                                             */
   SVGA_FIFO_BUSY,               /*                                      */

   /*
                                                                
                                                                    
                                                                    
                                                               
    */
    SVGA_FIFO_NUM_REGS
};


/*
                                                             
  
                                                                    
                                                                   
                         
  
                                                                     
                                                                      
                             
  
                                                                  
                                                                     
                                                          
  
                                                                     
                                                                     
                                                           
  
                                                               
                                                    
 */
#define SVGA_FIFO_EXTENDED_MANDATORY_REGS  (SVGA_FIFO_3D_CAPS_LAST + 1)


/*
                                 
  
                                                           
                                                        
  
                    
  
                                                                          
  
                                                                 
                                                              
                                                              
  
                                                                    
                                                                 
                                                                  
                                                               
                                  
  
                                                                
                                                               
                                                                  
                          
  
                                                                   
                                                                  
                                                                   
                
  
                                                               
                                                               
                                                                 
                                                                  
                                       
  
                                                                   
                                                                  
                                                                      
                                                                 
  
                                                                  
  
                                                        
                                                                       
                                                                     
                            
  
                                                                    
                                                                     
                                                                   
                                                                    
                                                                   
                                    
  
                    
  
                                                                    
                                                                
                 
  
                                                                 
                                                                    
                                         
  
                                                                  
                   
  
                                                                 
                                                                  
                                                                  
                                                                
  
                     
  
                                                                  
                                                                  
                                                                   
  
                                                                      
                                                                      
                                    
  
                                                                  
              
  
                                                                
                                                                  
                                                                     
                                                 
  
                                                                  
                                  
  
                                                                     
                                                                    
                                             
  
                                                                
                                                    
 */


/*
                    
  
                                                         
                                                               
                                                      
                                                       
                                             
  
                                                                     
                                                          
  
                                 
  
                                                                     
                                                            
                                                                   
                                                                    
                                                            
                                                              
                                                                     
  
                    
                                                                     
  
                      
                                                                           
                                                               
  
                      
                               
  
                     
  
                                                                           
                                                             
  
                                                                          
                                                                  
  
                                                                      
                                                                     
                                                                    
                                    
  
                                                                          
                                                                        
                                                                         
                                                                          
  
                                                                     
                                                                      
                                                                      
                                                                 
                                                                
  
                                                                           
                                                                 
  
                        
  
                                                                           
  
                      
                                 
  
                                        
  
                                                                   
                                                                   
                                                                      
                                                                    
                                                
  
                                                                          
                                                                
  
                                   
  
                                                                    
                                                                      
                                                                      
                                                                      
                                                  
  
                        
  
                                                                   
                                                
 */

#define SVGA_FIFO_CAP_NONE                  0
#define SVGA_FIFO_CAP_FENCE             (1<<0)
#define SVGA_FIFO_CAP_ACCELFRONT        (1<<1)
#define SVGA_FIFO_CAP_PITCHLOCK         (1<<2)
#define SVGA_FIFO_CAP_VIDEO             (1<<3)
#define SVGA_FIFO_CAP_CURSOR_BYPASS_3   (1<<4)
#define SVGA_FIFO_CAP_ESCAPE            (1<<5)
#define SVGA_FIFO_CAP_RESERVE           (1<<6)
#define SVGA_FIFO_CAP_SCREEN_OBJECT     (1<<7)
#define SVGA_FIFO_CAP_GMR2              (1<<8)
#define SVGA_FIFO_CAP_3D_HWVERSION_REVISED  SVGA_FIFO_CAP_GMR2
#define SVGA_FIFO_CAP_SCREEN_OBJECT_2   (1<<9)
#define SVGA_FIFO_CAP_DEAD              (1<<10)


/*
             
  
                                                                   
 */

#define SVGA_FIFO_FLAG_NONE                 0
#define SVGA_FIFO_FLAG_ACCELFRONT       (1<<0)
#define SVGA_FIFO_FLAG_RESERVED        (1<<31) /*                   */

/*
                                  
 */

#define SVGA_FIFO_RESERVED_UNKNOWN      0xffffffff


/*
                        
 */

#define SVGA_NUM_OVERLAY_UNITS 32


/*
                                                       
 */

#define SVGA_VIDEO_FLAG_COLORKEY        0x0001


/*
                                          
 */

enum {
   SVGA_VIDEO_ENABLED = 0,
   SVGA_VIDEO_FLAGS,
   SVGA_VIDEO_DATA_OFFSET,
   SVGA_VIDEO_FORMAT,
   SVGA_VIDEO_COLORKEY,
   SVGA_VIDEO_SIZE,          /*            */
   SVGA_VIDEO_WIDTH,
   SVGA_VIDEO_HEIGHT,
   SVGA_VIDEO_SRC_X,
   SVGA_VIDEO_SRC_Y,
   SVGA_VIDEO_SRC_WIDTH,
   SVGA_VIDEO_SRC_HEIGHT,
   SVGA_VIDEO_DST_X,         /*              */
   SVGA_VIDEO_DST_Y,         /*              */
   SVGA_VIDEO_DST_WIDTH,
   SVGA_VIDEO_DST_HEIGHT,
   SVGA_VIDEO_PITCH_1,
   SVGA_VIDEO_PITCH_2,
   SVGA_VIDEO_PITCH_3,
   SVGA_VIDEO_DATA_GMRID,    /*                                            */
   SVGA_VIDEO_DST_SCREEN_ID, /*                                                        */
   SVGA_VIDEO_NUM_REGS
};


/*
                     
  
                                                                 
                                                                         
                                    
 */

typedef struct SVGAOverlayUnit {
   uint32 enabled;
   uint32 flags;
   uint32 dataOffset;
   uint32 format;
   uint32 colorKey;
   uint32 size;
   uint32 width;
   uint32 height;
   uint32 srcX;
   uint32 srcY;
   uint32 srcWidth;
   uint32 srcHeight;
   int32  dstX;
   int32  dstY;
   uint32 dstWidth;
   uint32 dstHeight;
   uint32 pitches[3];
   uint32 dataGMRId;
   uint32 dstScreenId;
} SVGAOverlayUnit;


/*
                      
  
                                                                      
                                                            
                                                        
  
                                                                
                                                                     
                                                                      
  
                                                          
                                                                     
                                                    
  
                                                                     
                                                                    
                                                                 
                                                                      
                                                                  
                                                               
                                                                    
  
                                                                    
                                                                  
                                                              
  
                                                                   
                                                                    
                                                   
 */

#define SVGA_SCREEN_MUST_BE_SET     (1 << 0) /*                                  */
#define SVGA_SCREEN_HAS_ROOT SVGA_SCREEN_MUST_BE_SET /*            */
#define SVGA_SCREEN_IS_PRIMARY      (1 << 1) /*                                             */
#define SVGA_SCREEN_FULLSCREEN_HINT (1 << 2) /*                                        */

/*
                                                                
                                                                 
                                                                   
                                                                   
 */
#define SVGA_SCREEN_DEACTIVATE  (1 << 3)

/*
                                                                   
                                                                 
                                                                      
                                                                     
                                                                 
                                                                      
                                                                     
                                             
 */
#define SVGA_SCREEN_BLANKING (1 << 4)

typedef
struct SVGAScreenObject {
   uint32 structSize;   /*                          */
   uint32 id;
   uint32 flags;
   struct {
      uint32 width;
      uint32 height;
   } size;
   struct {
      int32 x;
      int32 y;
   } root;

   /*
                                                                   
                                       
    */
   SVGAGuestImage backingStore;
   uint32 cloneCount;
} SVGAScreenObject;


/*
                                 
  
                                                                  
                                                                         
                        
  
                                                                      
                                                   
  
                                                                    
             
  
                                                               
                  
 */

typedef enum {
   SVGA_CMD_INVALID_CMD           = 0,
   SVGA_CMD_UPDATE                = 1,
   SVGA_CMD_RECT_COPY             = 3,
   SVGA_CMD_DEFINE_CURSOR         = 19,
   SVGA_CMD_DEFINE_ALPHA_CURSOR   = 22,
   SVGA_CMD_UPDATE_VERBOSE        = 25,
   SVGA_CMD_FRONT_ROP_FILL        = 29,
   SVGA_CMD_FENCE                 = 30,
   SVGA_CMD_ESCAPE                = 33,
   SVGA_CMD_DEFINE_SCREEN         = 34,
   SVGA_CMD_DESTROY_SCREEN        = 35,
   SVGA_CMD_DEFINE_GMRFB          = 36,
   SVGA_CMD_BLIT_GMRFB_TO_SCREEN  = 37,
   SVGA_CMD_BLIT_SCREEN_TO_GMRFB  = 38,
   SVGA_CMD_ANNOTATION_FILL       = 39,
   SVGA_CMD_ANNOTATION_COPY       = 40,
   SVGA_CMD_DEFINE_GMR2           = 41,
   SVGA_CMD_REMAP_GMR2            = 42,
   SVGA_CMD_MAX
} SVGAFifoCmdId;

#define SVGA_CMD_MAX_ARGS           64


/*
                     
  
                                                                    
                                                             
                                                    
  
                                                                     
                                                                 
                                                                    
                                                                     
                                                                  
                                                    
  
                                                          
                                                                  
                                                                  
                                                                     
                                                                      
  
                                                                      
                                                                  
                                      
  
                
                       
 */

typedef
struct SVGAFifoCmdUpdate {
   uint32 x;
   uint32 y;
   uint32 width;
   uint32 height;
} SVGAFifoCmdUpdate;


/*
                        
  
                                                                    
                                                                     
  
                
                        
 */

typedef
struct SVGAFifoCmdRectCopy {
   uint32 srcX;
   uint32 srcY;
   uint32 destX;
   uint32 destY;
   uint32 width;
   uint32 height;
} SVGAFifoCmdRectCopy;


/*
                            
  
                                                     
  
                                                                    
                                                        
                                               
  
                
                     
 */

typedef
struct SVGAFifoCmdDefineCursor {
   uint32 id;             /*                         */
   uint32 hotspotX;
   uint32 hotspotY;
   uint32 width;
   uint32 height;
   uint32 andMaskDepth;   /*                                            */
   uint32 xorMaskDepth;   /*                                            */
   /*
                                                            
                                                   
   */
} SVGAFifoCmdDefineCursor;


/*
                                  
  
                                                        
  
                                                                    
                                                        
                                               
  
                
                           
 */

typedef
struct SVGAFifoCmdDefineAlphaCursor {
   uint32 id;             /*                         */
   uint32 hotspotX;
   uint32 hotspotY;
   uint32 width;
   uint32 height;
   /*                           */
} SVGAFifoCmdDefineAlphaCursor;


/*
                             
  
                                                                 
                                                                
                                                              
              
  
                
                            
 */

typedef
struct SVGAFifoCmdUpdateVerbose {
   uint32 x;
   uint32 y;
   uint32 width;
   uint32 height;
   uint32 reason;
} SVGAFifoCmdUpdateVerbose;


/*
                             
  
                                                                    
                                                              
                                                                     
                                                                    
                                   
  
                
                              
 */

#define  SVGA_ROP_COPY                    0x03

typedef
struct SVGAFifoCmdFrontRopFill {
   uint32 color;     /*                               */
   uint32 x;
   uint32 y;
   uint32 width;
   uint32 height;
   uint32 rop;       /*                       */
} SVGAFifoCmdFrontRopFill;


/*
                    
  
                                                                   
                                                           
                                                                      
                                                                 
                                                                   
                           
  
                
                                       
                                                
 */

typedef
struct {
   uint32 fence;
} SVGAFifoCmdFence;


/*
                     
  
                                                                  
                                                              
                                                 
  
                
                          
 */

typedef
struct SVGAFifoCmdEscape {
   uint32 nsid;
   uint32 size;
   /*                                  */
} SVGAFifoCmdEscape;


/*
                            
  
                                                                    
                                                                  
                                                              
                                                              
                                                      
                                                                     
                       
  
                                         
  
                                                                      
                                                                      
                                                                    
                                                          
                                                                  
  
                                                                
                                                                  
  
                
                                                                  
 */

typedef
struct {
   SVGAScreenObject screen;   /*                                      */
} SVGAFifoCmdDefineScreen;


/*
                             
  
                                                                      
             
  
                
                                                                  
 */

typedef
struct {
   uint32 screenId;
} SVGAFifoCmdDestroyScreen;


/*
                           
  
                                                               
                                                               
                                                                   
                                                                  
                                                                  
                                      
  
                                                                
                                                             
                                                                
                                    
  
                                                                      
                                                                      
                                                                 
                                                                      
                                 
  
                                                                   
                                                                
                                                                     
                                                                      
                                                                  
                                                                     
                                                             
                                                                   
                                                            
                         
  
                                                                  
                                                                
                                                                   
                                                                
                                                                
              
  
                                                                    
                                                                    
            
  
                
                                                                  
 */

typedef
struct {
   SVGAGuestPtr        ptr;
   uint32              bytesPerLine;
   SVGAGMRImageFormat  format;
} SVGAFifoCmdDefineGMRFB;


/*
                                   
  
                                                                  
                                                                   
                                 
  
                                                               
                                                                      
                                                             
                                                                    
                                                                  
                                                         
  
                                                                      
                                                                     
                                                                   
                                               
  
                                                                    
                                                            
  
                                                  
                                                 
  
                
                                                                  
 */

typedef
struct {
   SVGASignedPoint  srcOrigin;
   SVGASignedRect   destRect;
   uint32           destScreenId;
} SVGAFifoCmdBlitGMRFBToScreen;


/*
                                   
  
                                                                  
                                                                     
                                
  
                                                             
                                                                     
                                                                  
                                    
  
                                                               
                                                                     
                                                            
  
                                                                 
                                                                    
                                                                   
                                                                
                            
  
                                                                      
                                                                     
                                                                   
                                                                  
                                                                    
                                                                   
                
  
                                                                  
                                     
  
                                                                     
                                                         
  
                
                                                                  
 */

typedef
struct {
   SVGASignedPoint  destOrigin;
   SVGASignedRect   srcRect;
   uint32           srcScreenId;
} SVGAFifoCmdBlitScreenToGMRFB;


/*
                              
  
                                                                     
                                                               
                                                              
                                                               
                                                                    
                                             
  
                                                                 
                                                                    
                                                                 
                           
  
                                                                    
                                                                 
                                                                    
                                                    
  
                                                                
                                                                
  
                
                                                                  
 */

typedef
struct {
   SVGAColorBGRX  color;
} SVGAFifoCmdAnnotationFill;


/*
                              
  
                                                                      
                                    
  
                                                                 
                                                                    
                                                                      
                                                                 
  
                                                                     
                                                                     
                                                                
  
                                                                     
                                                                 
                                                                      
                                                                  
                                                                   
                
  
                
                                                                  
 */

typedef
struct {
   SVGASignedPoint  srcOrigin;
   uint32           srcScreenId;
} SVGAFifoCmdAnnotationCopy;


/*
                          
  
                                                                        
  
                
                   
 */

typedef
struct {
   uint32 gmrId;
   uint32 numPages;
} SVGAFifoCmdDefineGMR2;


/*
                         
  
                                                                       
  
                                                                         
                                                                          
                                                                        
                                                            
  
                                   
  
                                                                          
                                                                         
                                                                      
                                                                
  
                                                                           
                               
  
                                                                           
                                                                     
                                                  
  
                
                   
 */

typedef enum {
   SVGA_REMAP_GMR2_PPN32         = 0,
   SVGA_REMAP_GMR2_VIA_GMR       = (1 << 0),
   SVGA_REMAP_GMR2_PPN64         = (1 << 1),
   SVGA_REMAP_GMR2_SINGLE_PPN    = (1 << 2),
} SVGARemapGMR2Flags;

typedef
struct {
   uint32 gmrId;
   SVGARemapGMR2Flags flags;
   uint32 offsetPages; /*                                */
   uint32 numPages; /*                          */
   /*
                                                                  
     
                                                                          
                                                                     
                                                                 
                                                                       
    */
} SVGAFifoCmdRemapGMR2;

#endif
