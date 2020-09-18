#include <rtthread.h>
#include <finsh.h>

#include <ly68l6400.h>

#define DBG_TAG "ly68l6400.example"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

int ly68l6400_test(int argc, char** argv)
{
    int i = 0, j = 0;
    rt_device_t ly68l6400 = rt_device_find(BSP_LY68L6400_DEVICE_NAME);

    if(ly68l6400 != RT_NULL)
    {
        uint8_t buff[256] = {0};
        //init buff
        for(i = 0;i < 256;i ++)
            buff[i] = i;
        //write sram
        for(i = 0;i < LY68L6400_SIZE / 256;i ++)
        {
            rt_device_write(ly68l6400, i * 256, buff, sizeof(buff));
        }

        //read sram
        for(i = 0;i < LY68L6400_SIZE / 256;i ++)
        {
            rt_device_read(ly68l6400, i * 256 + j, buff, sizeof(buff));
            for(j = 0;j < 256;j ++)
            {
                if(j != buff[j])
                {
                    LOG_E("%s test failed", BSP_LY68L6400_DEVICE_NAME);
                    return -RT_ERROR;
                }
            }
        }
        LOG_I("%s test success", BSP_LY68L6400_DEVICE_NAME);
    }
    else {
        LOG_E("not find %s device", BSP_LY68L6400_DEVICE_NAME);
    }
    
    return RT_EOK;
}
MSH_CMD_EXPORT(ly68l6400_test, ly68l6400 API test);
