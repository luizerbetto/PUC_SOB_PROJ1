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
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x37a0cba, "kfree" },
	{ 0x53f2ef10, "wait_for_completion_interruptible" },
	{ 0x48eb0c0d, "__init_waitqueue_head" },
	{ 0xb6244511, "sg_init_one" },
	{ 0xba98ff5c, "kmem_cache_alloc" },
	{ 0xcc811a88, "kmalloc_caches" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0xd0d8621b, "strlen" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x50eedeb8, "printk" },
	{ 0x19a9e62b, "complete" },
	{ 0x91715312, "sprintf" },
	{ 0x966e767a, "module_put" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B4BBD1B8D85375DC5CA7E20");
