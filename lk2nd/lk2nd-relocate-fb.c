// SPDX-License-Identifier: GPL-2.0-only

#include <libfdt.h>
#include <bits.h>
#include <reg.h>
#if DISPLAY_TYPE_MDP3
#include <mdp3.h>
#else
#include <mdp5.h>
#endif
#include <platform/iomap.h>

void lk2nd_relocate_fb(const void *fdt, int offset)
{
	int len; 
	const uint32_t *val;
	uint32_t new;

	val = fdt_getprop(fdt, offset, "lk2nd,relocate-fb", &len);

	if (len < 0) {
		return;
	}

	new = fdt32_to_cpu(*val);
	
	#if DISPLAY_TYPE_MDP3
	writel(new, MDP_DMA_P_BUF_ADDR);
	dprintf(INFO, "Relocated framebuffer to %p, begone rainbow screen!\n", new);
	#else
	// Assume the pipe for now
	writel(new, MDP_VP_0_RGB_0_BASE + PIPE_SSPP_SRC0_ADDR);
	writel(BIT(3), MDP_CTL_0_BASE + CTL_FLUSH);
	dprintf(INFO, "Relocated framebuffer to %p, begone rainbow screen!\n", new);
	#endif
}
