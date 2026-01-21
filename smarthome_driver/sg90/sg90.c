#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/pwm.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/errno.h>

#define DEVICE_NAME     "sg90"
#define PWM_PERIOD_NS   20000000    // 20ms = 50Hz
#define PWM_MIN_NS      500000      // 0.5ms → 0°
#define PWM_MAX_NS      2500000     // 2.5ms → 180°

struct sg90_priv {
    struct pwm_device *pwm;
    struct miscdevice miscdev;
};

static struct sg90_priv *sg90_dev;

/* file_operations 實現 */
static int sg90_open(struct inode *inode, struct file *filp)
{
    filp->private_data = sg90_dev;

    if (!sg90_dev->pwm) {
        printk(KERN_ERR "sg90: pwm device not available\n");
        return -ENODEV;
    }

    // 預設轉到中間位置 (90°)
    pwm_config(sg90_dev->pwm, 1500000, PWM_PERIOD_NS);
    pwm_set_polarity(sg90_dev->pwm, PWM_POLARITY_NORMAL);
    pwm_enable(sg90_dev->pwm);

    printk(KERN_INFO "sg90: device opened, pwm enabled at 90°\n");
    return 0;
}

static int sg90_release(struct inode *inode, struct file *filp)
{
    // 可選擇在關閉時把舵機回到0°或關閉pwm
    // pwm_disable(sg90_dev->pwm);  // 視需求決定是否關閉
    printk(KERN_INFO "sg90: device released\n");
    return 0;
}

static ssize_t sg90_write(struct file *filp, const char __user *buf,
                          size_t count, loff_t *ppos)
{
    struct sg90_priv *priv = filp->private_data;
    u8 angle;
    unsigned long duty_ns;

    if (count < 1)
        return -EINVAL;

    if (copy_from_user(&angle, buf, 1))
        return -EFAULT;

    // 限制角度範圍 0~180
    if (angle > 180)
        angle = 180;

    // 計算占空比：0.5ms ~ 2.5ms
    duty_ns = PWM_MIN_NS + (angle * (PWM_MAX_NS - PWM_MIN_NS) / 180);

    pwm_config(priv->pwm, duty_ns, PWM_PERIOD_NS);
    // 如果你的內核版本支援，可加 pwm_apply_state()，但大多數情況 config 就夠了

    printk(KERN_INFO "sg90: set angle %d° → duty %lu ns\n", angle, duty_ns);

    return count;  // 正常情況回傳寫入的字節數
}

static ssize_t sg90_read(struct file *filp, char __user *buf,
                         size_t count, loff_t *ppos)
{
    return 0;  // 暫時不支援讀取
}

static const struct file_operations sg90_fops = {
    .owner   = THIS_MODULE,
    .open    = sg90_open,
    .release = sg90_release,
    .write   = sg90_write,
    .read    = sg90_read,
};

/* miscdevice 結構 */
static struct miscdevice sg90_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = DEVICE_NAME,
    .fops  = &sg90_fops,
};

/* 模組初始化 */
static int __init sg90_init(void)
{
    int ret;

    sg90_dev = kzalloc(sizeof(*sg90_dev), GFP_KERNEL);
    if (!sg90_dev)
        return -ENOMEM;

    // 取得 PWM 設備
    // 這裡最關鍵：請根據你的系統實際 pwm 控制器名稱修改！
    // 常見可能名稱： "pwm1", "pwmchip0", "pwm1-0", "imx-pwm.0" 等
    sg90_dev->pwm = pwm_get(NULL, "pwm1");   // ←←← 這裡要改成你實際的名稱！

    if (IS_ERR(sg90_dev->pwm)) {
        ret = PTR_ERR(sg90_dev->pwm);
        printk(KERN_ERR "sg90: failed to get pwm device: %d\n", ret);
        kfree(sg90_dev);
        return ret;
    }

    // 註冊 misc 設備 → 自動產生 /dev/sg90
    ret = misc_register(&sg90_misc);
    if (ret) {
        printk(KERN_ERR "sg90: failed to register misc device: %d\n", ret);
        pwm_put(sg90_dev->pwm);
        kfree(sg90_dev);
        return ret;
    }

    printk(KERN_INFO "sg90 driver loaded successfully\n");
    printk(KERN_INFO "Use: echo <angle(0-180)> > /dev/sg90\n");

    return 0;
}

/* 模組卸載 */
static void __exit sg90_exit(void)
{
    misc_deregister(&sg90_misc);

    if (sg90_dev && sg90_dev->pwm) {
        pwm_disable(sg90_dev->pwm);
        pwm_put(sg90_dev->pwm);
    }

    kfree(sg90_dev);

    printk(KERN_INFO "sg90 driver unloaded\n");
}

module_init(sg90_init);
module_exit(sg90_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple SG90 servo driver using miscdevice");