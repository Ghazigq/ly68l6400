#ifndef PKG_LY68L6400_H__
#define PKG_LY68L6400_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PKG_USING_LY68L6400 /* PKG_USING_LY68L6400 */

#include <rtdevice.h>

struct rt_ly68l6400_device
{
  struct rt_device parent;
  rt_uint8_t MFID;
  rt_uint8_t KGD;
  rt_uint64_t EID;
  void  *data;
};

#define LY68L6400_Read              0x03
#define LY68L6400_FastRead          0x0B
#define LY68L6400_FastReadQuad      0xEB
#define LY68L6400_Write             0X02
#define LY68L6400_QuadWrite         0X38
#define LY68L6400_EnterQuadMode     0X35
#define LY68L6400_ExitQuadMode      0XF5
#define LY68L6400_ResetEnable       0X66
#define LY68L6400_Reset             0X99
#define LY68L6400_SetBurstLength    0XC0
#define LY68L6400_ReadID            0X9F

#define LY68L6400_MFID              0X0D
#define LY68L6400_KGD_Fail          0X55
#define LY68L6400_KGD_Pass          0X5D

#define LY68L6400_SIZE              (8*1024*1024)   //8MB
#define LY68L6400_MAX_FREQ          100000000   //datasheet 144000000


#define RT_LY68L6400_DEFAULT_SPI_CFG                                        \
{                                                                           \
    .mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB,                     \
    .data_width = 8,                                                        \
    .max_hz = LY68L6400_MAX_FREQ,                                           \
}

#endif /* PKG_USING_LY68L6400 */

#ifdef __cplusplus
}
#endif

#endif
