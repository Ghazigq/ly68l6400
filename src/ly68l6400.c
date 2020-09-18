#include <rtthread.h>
#include "ly68l6400.h"

#ifdef PKG_USING_LY68L6400 /* BSP_USING_LY68L6400SLIT */

#include "board.h"
#include <drv_spi.h>

#define DBG_TAG "ly68l6400"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>


static struct rt_ly68l6400_device ly68l6400 = {0};

rt_size_t rt_ly68l6400_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    struct rt_spi_device * ly68l6400_dev = (struct rt_spi_device *)dev;
    rt_uint8_t read = LY68L6400_FastRead;

    RT_ASSERT(ly68l6400_dev != RT_NULL);
    if(pos >= LY68L6400_SIZE)
    {
        LOG_E("ERROR: read outrange sram size(0x%08x)!", LY68L6400_SIZE);
        return 0;
    }

    rt_spi_send_then_recv(ly68l6400_dev, &read, sizeof(read), buffer, size);
    return size;
};

rt_size_t rt_ly68l6400_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    struct rt_spi_device * ly68l6400_dev = (struct rt_spi_device *)dev;
    rt_uint8_t write = LY68L6400_Write;

    RT_ASSERT(ly68l6400_dev != RT_NULL);
    if(pos >= LY68L6400_SIZE)
    {
        LOG_E("ERROR: write outrange sram size(0x%08x)!", LY68L6400_SIZE);
        return 0;
    }

    rt_spi_send_then_send(ly68l6400_dev, &write, sizeof(write), buffer, size);
    return size;
};


#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops ly68l6400_ops =
{
  RT_NULL,
  RT_NULL,
  RT_NULL,
  rt_ly68l6400_read,
  rt_ly68l6400_write,
  RT_NULL
};
#endif
rt_err_t ly68l6400_device_register(struct rt_ly68l6400_device *ly68l6400,
                                const char              *name,
                                rt_uint32_t              flag,
                                void                    *data)
{
  rt_err_t ret;
  struct rt_device *device;
  RT_ASSERT(ly68l6400 != RT_NULL);

  device = &(ly68l6400->parent);

  device->type        = RT_Device_Class_MTD;
  device->rx_indicate = RT_NULL;
  device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
  device->ops         = &serial_ops;
#else
  device->init        = RT_NULL;
  device->open        = RT_NULL;
  device->close       = RT_NULL;
  device->read        = rt_ly68l6400_read;
  device->write       = rt_ly68l6400_write;
  device->control     = RT_NULL;
#endif
  device->user_data   = data;

  /* register a character device */
  ret = rt_device_register(device, name, flag);

  return ret;
}

int ly68l6400_device_init(void)
{
    struct rt_spi_device * device = RT_NULL;
    struct rt_spi_configuration cfg = RT_LY68L6400_DEFAULT_SPI_CFG;

    rt_hw_spi_device_attach(PKG_LY68L6400_SPI_BUS_NAME, "ly68l6400", GPIOC, GPIO_PIN_13);
    device = (struct rt_spi_device *)rt_device_find("ly68l6400");

    if(device != RT_NULL)
    {
        rt_uint8_t reset[2] = {LY68L6400_ResetEnable, LY68L6400_Reset};
        rt_uint8_t send[4] = {LY68L6400_ReadID};
        rt_uint8_t recv[8] = {0};

        if(device->config.max_hz > LY68L6400_MAX_FREQ || device->config.max_hz == 0)
            rt_spi_configure(device, &cfg);
        LOG_D("%s device max freq is %dHz", BSP_LY68L6400_DEVICE_NAME, device->config.max_hz);
        //reset ly68l6400
        rt_spi_send_then_send(device, reset, 1, &reset[1], 1);
        rt_thread_mdelay(1);
        //read ly68l6400 id
        rt_spi_send_then_recv(device, &send, sizeof(send), recv, sizeof(recv));
        ly68l6400.MFID = recv[0];
        ly68l6400.KGD  = recv[1];
        ly68l6400.EID  = (rt_uint64_t)recv[2] << 48 | (rt_uint64_t)recv[3] << 32 | (rt_uint64_t)recv[4] << 24 | (rt_uint64_t)recv[5] << 16 | (rt_uint64_t)recv[6] << 8 | (rt_uint64_t)recv[7];

        LOG_D("MFID:0x%02x", ly68l6400.MFID);
        LOG_D("KGD: 0x%02x", ly68l6400.KGD);
        LOG_D("EID: 0x%012x", ly68l6400.EID);

        if(ly68l6400.MFID == LY68L6400_MFID && ly68l6400.KGD == LY68L6400_KGD_Pass)
        {
            if(ly68l6400_device_register(&ly68l6400, BSP_LY68L6400_DEVICE_NAME, RT_DEVICE_FLAG_RDWR, RT_NULL) != RT_EOK)
            {
                LOG_E("%s device register failed", BSP_LY68L6400_DEVICE_NAME);
                return -RT_ERROR;
            }
            else
                return RT_EOK;
        }
        else
        {
            LOG_E("%s device info check error", BSP_LY68L6400_DEVICE_NAME);
            return -RT_ERROR;
        }
    }
    LOG_E("%s device register failed", BSP_LY68L6400_DEVICE_NAME);
    return -RT_ERROR;
}
INIT_COMPONENT_EXPORT(ly68l6400_device_init);

#endif /* PKG_USING_LY68L6400 */

