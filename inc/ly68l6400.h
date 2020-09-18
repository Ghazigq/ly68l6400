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

#endif /* PKG_USING_LY68L6400 */

#ifdef __cplusplus
}
#endif

#endif
