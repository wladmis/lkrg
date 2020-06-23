/*
 * pi3's Linux kernel Runtime Guard
 *
 * Component:
 *  - Handle *_JUMP_LABEL self-modifying code.
 *    Hook 'arch_jump_label_transform_apply' function.
 *
 * Notes:
 *  - Linux kernel is heavily consuming *_JUMP_LABEL (if enabled). Most of the
 *    Linux distributions provide kernel with these options compiled. It makes
 *    Linux kernel being self-modifying code. It is very troublesome for this
 *    project. We are relying on comparing hashes from the specific memory
 *    regions and by design self-modifications break this functionality.
 *  - We are hooking into low-level *_JUMP_LABEL functions to be able to
 *    monitor whenever new modification is on the way.
 *
 * Caveats:
 *  - Since kernel 5.3 Linux has support for 'batch mode' *_JUMP_LABEL.
 *    Let's handle that as well.
 *
 *    https://lore.kernel.org/patchwork/patch/1064287/
 *
 * Timeline:
 *  - Created: 31.X.2019
 *
 * Author:
 *  - Adam 'pi3' Zabrocki (http://pi3.com.pl)
 *
 */

#if defined(CONFIG_X86)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,3,0)

#ifndef P_LKRG_CI_ARCH_JUMP_LABEL_TRANSFORM_APPLY_H
#define P_LKRG_CI_ARCH_JUMP_LABEL_TRANSFORM_APPLY_H

#include <asm/text-patching.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
typedef struct _p_text_poke_loc {
    s32 rel_addr; /* addr := _stext + rel_addr */
    s32 rel32;
    u8 opcode;
    const u8 text[POKE_MAX_OPCODE_SIZE];
} p_text_poke_loc;
#else
typedef struct text_poke_loc p_text_poke_loc;
#endif

#define P_TP_VEC_MAX (PAGE_SIZE / sizeof(p_text_poke_loc))

/* per-instance private data */
struct p_arch_jump_label_transform_apply_data {
    ktime_t entry_stamp;
};


int p_arch_jump_label_transform_apply_ret(struct kretprobe_instance *ri, struct pt_regs *p_regs);
int p_arch_jump_label_transform_apply_entry(struct kretprobe_instance *p_ri, struct pt_regs *p_regs);
int p_install_arch_jump_label_transform_apply_hook(void);
void p_uninstall_arch_jump_label_transform_apply_hook(void);

#endif

#endif

#endif
