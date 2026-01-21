#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xfa985410, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x87a740ad, __VMLINUX_SYMBOL_STR(driver_unregister) },
	{ 0x34ce7e95, __VMLINUX_SYMBOL_STR(spi_register_driver) },
	{ 0x4ee6e196, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0x67b52003, __VMLINUX_SYMBOL_STR(regmap_init_spi) },
	{ 0xe42d049a, __VMLINUX_SYMBOL_STR(devm_gpio_request) },
	{ 0xde6fb46c, __VMLINUX_SYMBOL_STR(of_get_named_gpio_flags) },
	{ 0x93e3940, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0xfbe0f876, __VMLINUX_SYMBOL_STR(spi_setup) },
	{ 0x5bc6c4e1, __VMLINUX_SYMBOL_STR(regmap_exit) },
	{ 0xd9285795, __VMLINUX_SYMBOL_STR(misc_deregister) },
	{ 0xfef3f0ab, __VMLINUX_SYMBOL_STR(regmap_write) },
	{ 0xc61ff1f2, __VMLINUX_SYMBOL_STR(gpiod_direction_output_raw) },
	{ 0x3089e8ae, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0x8e865d3c, __VMLINUX_SYMBOL_STR(arm_delay_ops) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x67c2fa54, __VMLINUX_SYMBOL_STR(__copy_to_user) },
	{ 0x12b8611, __VMLINUX_SYMBOL_STR(regmap_read) },
	{ 0xfbc74f64, __VMLINUX_SYMBOL_STR(__copy_from_user) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Csmarthome_rc522*");

MODULE_INFO(srcversion, "6E19E8CC7F8599DDD7AA57C");
