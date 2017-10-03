#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0xe884004, "module_layout" },
	{ 0x72aa82c6, "param_ops_charp" },
	{ 0xc9d9d6dd, "class_unregister" },
	{ 0xcda742e5, "device_destroy" },
	{ 0xfbef6057, "class_destroy" },
	{ 0x19b26639, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x6c0f51ce, "__class_create" },
	{ 0x346bd748, "__register_chrdev" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x50eedeb8, "printk" },
	{ 0xd0d8621b, "strlen" },
	{ 0x91715312, "sprintf" },
	{ 0x966e767a, "module_put" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8D0C10357B185419ADCD56B");
