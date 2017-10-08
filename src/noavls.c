//noAVLS by SilicaServer (https://discord.gg/j4eGHhF)
//LISTEN TO THE SOUND OF FREEDOM

///@SilicaAndPina - Project Manager
///@dots_tb - RE and Dev

#include <stdio.h>
#include <stdarg.h>

#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/cpu.h>
#include <taihen.h>

static int hook = -1;
static tai_hook_ref_t ref_hook;

//r1 unk pointer
//r2 unk pointer
//r3 - pointer to int (avls)
static int sceAVConfigGetVolCtrlEnable_patched(int r1, int r2, int r3, int r4) {
	int ret, state;
	ENTER_SYSCALL(state);
	ret = TAI_CONTINUE(int, ref_hook,r1,r2,r3,r4);
	uint32_t avls = 0;
	ksceKernelMemcpyKernelToUser((uintptr_t)r3, &avls, sizeof(avls));
	EXIT_SYSCALL(state);
	return ret;
}	
void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args)
{
	hook = taiHookFunctionExportForKernel(KERNEL_PID,
		&ref_hook, 
		"SceAVConfig",
		TAI_ANY_LIBRARY,
		0x830b950b, 
		sceAVConfigGetVolCtrlEnable_patched);
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
	if (hook >= 0) taiHookReleaseForKernel(hook, ref_hook);   
	return SCE_KERNEL_STOP_SUCCESS;
}
