/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) Fuzhou Rockchip Electronics Co.Ltd
 * Author:Mark Yao <mark.yao@rock-chips.com>
 */

#ifndef _ROCKCHIP_DRM_VOP2_H
#define _ROCKCHIP_DRM_VOP2_H

#include <linux/regmap.h>
#include <drm/drm_modes.h>
#include "rockchip_drm_vop.h"

#define VOP2_VP_FEATURE_OUTPUT_10BIT        BIT(0)

#define VOP2_FEATURE_HAS_SYS_GRF	BIT(0)
#define VOP2_FEATURE_HAS_VO0_GRF	BIT(1)
#define VOP2_FEATURE_HAS_VO1_GRF	BIT(2)
#define VOP2_FEATURE_HAS_VOP_GRF	BIT(3)
#define VOP2_FEATURE_HAS_SYS_PMU	BIT(4)

#define WIN_FEATURE_AFBDC		BIT(0)
#define WIN_FEATURE_CLUSTER		BIT(1)

#define HIWORD_UPDATE(v, h, l)  ((GENMASK(h, l) << 16) | ((v) << (l)))
/*
 *  the delay number of a window in different mode.
 */
enum win_dly_mode {
	VOP2_DLY_MODE_DEFAULT,   /**< default mode */
	VOP2_DLY_MODE_HISO_S,    /** HDR in SDR out mode, as a SDR window */
	VOP2_DLY_MODE_HIHO_H,    /** HDR in HDR out mode, as a HDR window */
	VOP2_DLY_MODE_MAX,
};

enum vop2_scale_up_mode {
	VOP2_SCALE_UP_NRST_NBOR,
	VOP2_SCALE_UP_BIL,
	VOP2_SCALE_UP_BIC,
};

enum vop2_scale_down_mode {
	VOP2_SCALE_DOWN_NRST_NBOR,
	VOP2_SCALE_DOWN_BIL,
	VOP2_SCALE_DOWN_AVG,
};

/*
 * vop2 internal power domain id,
 * should be all none zero, 0 will be treat as invalid;
 */
#define VOP2_PD_CLUSTER0	BIT(0)
#define VOP2_PD_CLUSTER1	BIT(1)
#define VOP2_PD_CLUSTER2	BIT(2)
#define VOP2_PD_CLUSTER3	BIT(3)
#define VOP2_PD_DSC_8K		BIT(5)
#define VOP2_PD_DSC_4K		BIT(6)
#define VOP2_PD_ESMART		BIT(7)

enum vop2_win_regs {
	VOP2_WIN_ENABLE,
	VOP2_WIN_FORMAT,
	VOP2_WIN_CSC_MODE,
	VOP2_WIN_XMIRROR,
	VOP2_WIN_YMIRROR,
	VOP2_WIN_RB_SWAP,
	VOP2_WIN_UV_SWAP,
	VOP2_WIN_ACT_INFO,
	VOP2_WIN_DSP_INFO,
	VOP2_WIN_DSP_ST,
	VOP2_WIN_YRGB_MST,
	VOP2_WIN_UV_MST,
	VOP2_WIN_YRGB_VIR,
	VOP2_WIN_UV_VIR,
	VOP2_WIN_YUV_CLIP,
	VOP2_WIN_Y2R_EN,
	VOP2_WIN_R2Y_EN,
	VOP2_WIN_COLOR_KEY,
	VOP2_WIN_COLOR_KEY_EN,
	VOP2_WIN_DITHER_UP,

	/* scale regs */
	VOP2_WIN_SCALE_YRGB_X,
	VOP2_WIN_SCALE_YRGB_Y,
	VOP2_WIN_SCALE_CBCR_X,
	VOP2_WIN_SCALE_CBCR_Y,
	VOP2_WIN_YRGB_HOR_SCL_MODE,
	VOP2_WIN_YRGB_HSCL_FILTER_MODE,
	VOP2_WIN_YRGB_VER_SCL_MODE,
	VOP2_WIN_YRGB_VSCL_FILTER_MODE,
	VOP2_WIN_CBCR_VER_SCL_MODE,
	VOP2_WIN_CBCR_HSCL_FILTER_MODE,
	VOP2_WIN_CBCR_HOR_SCL_MODE,
	VOP2_WIN_CBCR_VSCL_FILTER_MODE,
	VOP2_WIN_VSD_CBCR_GT2,
	VOP2_WIN_VSD_CBCR_GT4,
	VOP2_WIN_VSD_YRGB_GT2,
	VOP2_WIN_VSD_YRGB_GT4,
	VOP2_WIN_BIC_COE_SEL,

	/* cluster regs */
	VOP2_WIN_CLUSTER_ENABLE,
	VOP2_WIN_AFBC_ENABLE,
	VOP2_WIN_CLUSTER_LB_MODE,

	/* afbc regs */
	VOP2_WIN_AFBC_FORMAT,
	VOP2_WIN_AFBC_RB_SWAP,
	VOP2_WIN_AFBC_UV_SWAP,
	VOP2_WIN_AFBC_AUTO_GATING_EN,
	VOP2_WIN_AFBC_BLOCK_SPLIT_EN,
	VOP2_WIN_AFBC_PIC_VIR_WIDTH,
	VOP2_WIN_AFBC_TILE_NUM,
	VOP2_WIN_AFBC_PIC_OFFSET,
	VOP2_WIN_AFBC_PIC_SIZE,
	VOP2_WIN_AFBC_DSP_OFFSET,
	VOP2_WIN_AFBC_TRANSFORM_OFFSET,
	VOP2_WIN_AFBC_HDR_PTR,
	VOP2_WIN_AFBC_HALF_BLOCK_EN,
	VOP2_WIN_AFBC_ROTATE_270,
	VOP2_WIN_AFBC_ROTATE_90,
	VOP2_WIN_MAX_REG,
};

struct vop2_win_data {
	const char *name;
	unsigned int phys_id;

	u32 base;
	enum drm_plane_type type;

	u32 nformats;
	const u32 *formats;
	const uint64_t *format_modifiers;
	const unsigned int supported_rotations;

	/**
	 * @layer_sel_id: defined by register OVERLAY_LAYER_SEL of VOP2
	 */
	unsigned int layer_sel_id;
	uint64_t feature;

	unsigned int max_upscale_factor;
	unsigned int max_downscale_factor;
	const u8 dly[VOP2_DLY_MODE_MAX];
};

struct vop2_video_port_data {
	unsigned int id;
	u32 feature;
	u16 gamma_lut_len;
	u16 cubic_lut_len;
	struct vop_rect max_output;
	const u8 pre_scan_max_dly[4];
	unsigned int offset;
};

struct vop2_data {
	u8 nr_vps;
	u64 feature;
	const struct vop2_win_data *win;
	const struct vop2_video_port_data *vp;
	struct vop_rect max_input;
	struct vop_rect max_output;

	unsigned int win_size;
	unsigned int soc_id;
};

/* interrupt define */
#define FS_NEW_INTR			BIT(4)
#define ADDR_SAME_INTR			BIT(5)
#define LINE_FLAG1_INTR			BIT(6)
#define WIN0_EMPTY_INTR			BIT(7)
#define WIN1_EMPTY_INTR			BIT(8)
#define WIN2_EMPTY_INTR			BIT(9)
#define WIN3_EMPTY_INTR			BIT(10)
#define HWC_EMPTY_INTR			BIT(11)
#define POST_BUF_EMPTY_INTR		BIT(12)
#define PWM_GEN_INTR			BIT(13)
#define DMA_FINISH_INTR			BIT(14)
#define FS_FIELD_INTR			BIT(15)
#define FE_INTR				BIT(16)
#define WB_UV_FIFO_FULL_INTR		BIT(17)
#define WB_YRGB_FIFO_FULL_INTR		BIT(18)
#define WB_COMPLETE_INTR		BIT(19)


enum vop_csc_format {
	CSC_BT601L,
	CSC_BT709L,
	CSC_BT601F,
	CSC_BT2020,
};

enum src_factor_mode {
	SRC_FAC_ALPHA_ZERO,
	SRC_FAC_ALPHA_ONE,
	SRC_FAC_ALPHA_DST,
	SRC_FAC_ALPHA_DST_INVERSE,
	SRC_FAC_ALPHA_SRC,
	SRC_FAC_ALPHA_SRC_GLOBAL,
};

enum dst_factor_mode {
	DST_FAC_ALPHA_ZERO,
	DST_FAC_ALPHA_ONE,
	DST_FAC_ALPHA_SRC,
	DST_FAC_ALPHA_SRC_INVERSE,
	DST_FAC_ALPHA_DST,
	DST_FAC_ALPHA_DST_GLOBAL,
};

#define RK3568_GRF_VO_CON1			0x0364

#define RK3588_GRF_SOC_CON1			0x0304
#define RK3588_GRF_VOP_CON2			0x08
#define RK3588_GRF_VO1_CON0			0x00

/* System registers definition */
#define RK3568_REG_CFG_DONE			0x000
#define RK3568_VERSION_INFO			0x004
#define RK3568_SYS_AUTO_GATING_CTRL		0x008
#define RK3568_SYS_AXI_LUT_CTRL			0x024
#define RK3568_DSP_IF_EN			0x028
#define RK3568_DSP_IF_CTRL			0x02c
#define RK3568_DSP_IF_POL			0x030
#define RK3588_SYS_PD_CTRL			0x034
#define RK3568_WB_CTRL				0x40
#define RK3568_WB_XSCAL_FACTOR			0x44
#define RK3568_WB_YRGB_MST			0x48
#define RK3568_WB_CBR_MST			0x4C
#define RK3568_OTP_WIN_EN			0x050
#define RK3568_LUT_PORT_SEL			0x058
#define RK3568_SYS_STATUS0			0x060
#define RK3568_VP_LINE_FLAG(vp)			(0x70 + (vp) * 0x4)
#define RK3568_SYS0_INT_EN			0x80
#define RK3568_SYS0_INT_CLR			0x84
#define RK3568_SYS0_INT_STATUS			0x88
#define RK3568_SYS1_INT_EN			0x90
#define RK3568_SYS1_INT_CLR			0x94
#define RK3568_SYS1_INT_STATUS			0x98
#define RK3568_VP_INT_EN(vp)			(0xA0 + (vp) * 0x10)
#define RK3568_VP_INT_CLR(vp)			(0xA4 + (vp) * 0x10)
#define RK3568_VP_INT_STATUS(vp)		(0xA8 + (vp) * 0x10)
#define RK3568_VP_INT_RAW_STATUS(vp)		(0xAC + (vp) * 0x10)

/* Video Port registers definition */
#define RK3568_VP0_CTRL_BASE			0x0C00
#define RK3568_VP1_CTRL_BASE			0x0D00
#define RK3568_VP2_CTRL_BASE			0x0E00
#define RK3588_VP3_CTRL_BASE			0x0F00
#define RK3568_VP_DSP_CTRL			0x00
#define RK3568_VP_MIPI_CTRL			0x04
#define RK3568_VP_COLOR_BAR_CTRL		0x08
#define RK3588_VP_CLK_CTRL			0x0C
#define RK3568_VP_3D_LUT_CTRL			0x10
#define RK3568_VP_3D_LUT_MST			0x20
#define RK3568_VP_DSP_BG			0x2C
#define RK3568_VP_PRE_SCAN_HTIMING		0x30
#define RK3568_VP_POST_DSP_HACT_INFO		0x34
#define RK3568_VP_POST_DSP_VACT_INFO		0x38
#define RK3568_VP_POST_SCL_FACTOR_YRGB		0x3C
#define RK3568_VP_POST_SCL_CTRL			0x40
#define RK3568_VP_POST_DSP_VACT_INFO_F1		0x44
#define RK3568_VP_DSP_HTOTAL_HS_END		0x48
#define RK3568_VP_DSP_HACT_ST_END		0x4C
#define RK3568_VP_DSP_VTOTAL_VS_END		0x50
#define RK3568_VP_DSP_VACT_ST_END		0x54
#define RK3568_VP_DSP_VS_ST_END_F1		0x58
#define RK3568_VP_DSP_VACT_ST_END_F1		0x5C
#define RK3568_VP_BCSH_CTRL			0x60
#define RK3568_VP_BCSH_BCS			0x64
#define RK3568_VP_BCSH_H			0x68
#define RK3568_VP_BCSH_COLOR_BAR		0x6C

/* Overlay registers definition    */
#define RK3568_OVL_CTRL				0x600
#define RK3568_OVL_LAYER_SEL			0x604
#define RK3568_OVL_PORT_SEL			0x608
#define RK3568_CLUSTER0_MIX_SRC_COLOR_CTRL	0x610
#define RK3568_CLUSTER0_MIX_DST_COLOR_CTRL	0x614
#define RK3568_CLUSTER0_MIX_SRC_ALPHA_CTRL	0x618
#define RK3568_CLUSTER0_MIX_DST_ALPHA_CTRL	0x61C
#define RK3568_MIX0_SRC_COLOR_CTRL		0x650
#define RK3568_MIX0_DST_COLOR_CTRL		0x654
#define RK3568_MIX0_SRC_ALPHA_CTRL		0x658
#define RK3568_MIX0_DST_ALPHA_CTRL		0x65C
#define RK3568_HDR0_SRC_COLOR_CTRL		0x6C0
#define RK3568_HDR0_DST_COLOR_CTRL		0x6C4
#define RK3568_HDR0_SRC_ALPHA_CTRL		0x6C8
#define RK3568_HDR0_DST_ALPHA_CTRL		0x6CC
#define RK3568_VP_BG_MIX_CTRL(vp)		(0x6E0 + (vp) * 4)
#define RK3568_CLUSTER_DLY_NUM			0x6F0
#define RK3568_SMART_DLY_NUM			0x6F8

/* Cluster register definition, offset relative to window base */
#define RK3568_CLUSTER0_CTRL_BASE		0x1000
#define RK3568_CLUSTER1_CTRL_BASE		0x1200
#define RK3588_CLUSTER2_CTRL_BASE		0x1400
#define RK3588_CLUSTER3_CTRL_BASE		0x1600
#define RK3568_ESMART0_CTRL_BASE		0x1800
#define RK3568_ESMART1_CTRL_BASE		0x1A00
#define RK3568_SMART0_CTRL_BASE			0x1C00
#define RK3568_SMART1_CTRL_BASE			0x1E00
#define RK3588_ESMART2_CTRL_BASE		0x1C00
#define RK3588_ESMART3_CTRL_BASE		0x1E00

#define RK3568_CLUSTER_WIN_CTRL0		0x00
#define RK3568_CLUSTER_WIN_CTRL1		0x04
#define RK3568_CLUSTER_WIN_YRGB_MST		0x10
#define RK3568_CLUSTER_WIN_CBR_MST		0x14
#define RK3568_CLUSTER_WIN_VIR			0x18
#define RK3568_CLUSTER_WIN_ACT_INFO		0x20
#define RK3568_CLUSTER_WIN_DSP_INFO		0x24
#define RK3568_CLUSTER_WIN_DSP_ST		0x28
#define RK3568_CLUSTER_WIN_SCL_FACTOR_YRGB	0x30
#define RK3568_CLUSTER_WIN_AFBCD_TRANSFORM_OFFSET	0x3C
#define RK3568_CLUSTER_WIN_AFBCD_OUTPUT_CTRL	0x50
#define RK3568_CLUSTER_WIN_AFBCD_ROTATE_MODE	0x54
#define RK3568_CLUSTER_WIN_AFBCD_HDR_PTR	0x58
#define RK3568_CLUSTER_WIN_AFBCD_VIR_WIDTH	0x5C
#define RK3568_CLUSTER_WIN_AFBCD_PIC_SIZE	0x60
#define RK3568_CLUSTER_WIN_AFBCD_PIC_OFFSET	0x64
#define RK3568_CLUSTER_WIN_AFBCD_DSP_OFFSET	0x68
#define RK3568_CLUSTER_WIN_AFBCD_CTRL		0x6C

#define RK3568_CLUSTER_CTRL			0x100

/* (E)smart register definition, offset relative to window base */
#define RK3568_SMART_CTRL0			0x00
#define RK3568_SMART_CTRL1			0x04
#define RK3568_SMART_REGION0_CTRL		0x10
#define RK3568_SMART_REGION0_YRGB_MST		0x14
#define RK3568_SMART_REGION0_CBR_MST		0x18
#define RK3568_SMART_REGION0_VIR		0x1C
#define RK3568_SMART_REGION0_ACT_INFO		0x20
#define RK3568_SMART_REGION0_DSP_INFO		0x24
#define RK3568_SMART_REGION0_DSP_ST		0x28
#define RK3568_SMART_REGION0_SCL_CTRL		0x30
#define RK3568_SMART_REGION0_SCL_FACTOR_YRGB	0x34
#define RK3568_SMART_REGION0_SCL_FACTOR_CBR	0x38
#define RK3568_SMART_REGION0_SCL_OFFSET		0x3C
#define RK3568_SMART_REGION1_CTRL		0x40
#define RK3568_SMART_REGION1_YRGB_MST		0x44
#define RK3568_SMART_REGION1_CBR_MST		0x48
#define RK3568_SMART_REGION1_VIR		0x4C
#define RK3568_SMART_REGION1_ACT_INFO		0x50
#define RK3568_SMART_REGION1_DSP_INFO		0x54
#define RK3568_SMART_REGION1_DSP_ST		0x58
#define RK3568_SMART_REGION1_SCL_CTRL		0x60
#define RK3568_SMART_REGION1_SCL_FACTOR_YRGB	0x64
#define RK3568_SMART_REGION1_SCL_FACTOR_CBR	0x68
#define RK3568_SMART_REGION1_SCL_OFFSET		0x6C
#define RK3568_SMART_REGION2_CTRL		0x70
#define RK3568_SMART_REGION2_YRGB_MST		0x74
#define RK3568_SMART_REGION2_CBR_MST		0x78
#define RK3568_SMART_REGION2_VIR		0x7C
#define RK3568_SMART_REGION2_ACT_INFO		0x80
#define RK3568_SMART_REGION2_DSP_INFO		0x84
#define RK3568_SMART_REGION2_DSP_ST		0x88
#define RK3568_SMART_REGION2_SCL_CTRL		0x90
#define RK3568_SMART_REGION2_SCL_FACTOR_YRGB	0x94
#define RK3568_SMART_REGION2_SCL_FACTOR_CBR	0x98
#define RK3568_SMART_REGION2_SCL_OFFSET		0x9C
#define RK3568_SMART_REGION3_CTRL		0xA0
#define RK3568_SMART_REGION3_YRGB_MST		0xA4
#define RK3568_SMART_REGION3_CBR_MST		0xA8
#define RK3568_SMART_REGION3_VIR		0xAC
#define RK3568_SMART_REGION3_ACT_INFO		0xB0
#define RK3568_SMART_REGION3_DSP_INFO		0xB4
#define RK3568_SMART_REGION3_DSP_ST		0xB8
#define RK3568_SMART_REGION3_SCL_CTRL		0xC0
#define RK3568_SMART_REGION3_SCL_FACTOR_YRGB	0xC4
#define RK3568_SMART_REGION3_SCL_FACTOR_CBR	0xC8
#define RK3568_SMART_REGION3_SCL_OFFSET		0xCC
#define RK3568_SMART_COLOR_KEY_CTRL		0xD0

/* HDR register definition */
#define RK3568_HDR_LUT_CTRL			0x2000
#define RK3568_HDR_LUT_MST			0x2004
#define RK3568_SDR2HDR_CTRL			0x2010
#define RK3568_HDR2SDR_CTRL			0x2020
#define RK3568_HDR2SDR_SRC_RANGE		0x2024
#define RK3568_HDR2SDR_NORMFACEETF		0x2028
#define RK3568_HDR2SDR_DST_RANGE		0x202C
#define RK3568_HDR2SDR_NORMFACCGAMMA		0x2030
#define RK3568_HDR_EETF_OETF_Y0			0x203C
#define RK3568_HDR_SAT_Y0			0x20C0
#define RK3568_HDR_EOTF_OETF_Y0			0x20F0
#define RK3568_HDR_OETF_DX_POW1			0x2200
#define RK3568_HDR_OETF_XN1			0x2300

#define RK3568_REG_CFG_DONE__GLB_CFG_DONE_EN		BIT(15)

#define RK3568_VP_DSP_CTRL__STANDBY			BIT(31)
#define RK3568_VP_DSP_CTRL__DITHER_DOWN_MODE		BIT(20)
#define RK3568_VP_DSP_CTRL__DITHER_DOWN_SEL		GENMASK(19, 18)
#define RK3568_VP_DSP_CTRL__DITHER_DOWN_EN		BIT(17)
#define RK3568_VP_DSP_CTRL__PRE_DITHER_DOWN_EN		BIT(16)
#define RK3568_VP_DSP_CTRL__POST_DSP_OUT_R2Y		BIT(15)
#define RK3568_VP_DSP_CTRL__DSP_RG_SWAP			BIT(10)
#define RK3568_VP_DSP_CTRL__DSP_RB_SWAP			BIT(9)
#define RK3568_VP_DSP_CTRL__DSP_BG_SWAP			BIT(8)
#define RK3568_VP_DSP_CTRL__DSP_INTERLACE		BIT(7)
#define RK3568_VP_DSP_CTRL__DSP_FILED_POL		BIT(6)
#define RK3568_VP_DSP_CTRL__P2I_EN			BIT(5)
#define RK3568_VP_DSP_CTRL__CORE_DCLK_DIV		BIT(4)
#define RK3568_VP_DSP_CTRL__OUT_MODE			GENMASK(3, 0)

#define RK3588_VP_CLK_CTRL__DCLK_OUT_DIV		GENMASK(3, 2)
#define RK3588_VP_CLK_CTRL__DCLK_CORE_DIV		GENMASK(1, 0)

#define RK3568_VP_POST_SCL_CTRL__VSCALEDOWN		BIT(1)
#define RK3568_VP_POST_SCL_CTRL__HSCALEDOWN		BIT(0)

#define RK3568_SYS_DSP_INFACE_EN_LVDS1_MUX		GENMASK(26, 25)
#define RK3568_SYS_DSP_INFACE_EN_LVDS1			BIT(24)
#define RK3568_SYS_DSP_INFACE_EN_MIPI1_MUX		GENMASK(22, 21)
#define RK3568_SYS_DSP_INFACE_EN_MIPI1			BIT(20)
#define RK3568_SYS_DSP_INFACE_EN_LVDS0_MUX		GENMASK(19, 18)
#define RK3568_SYS_DSP_INFACE_EN_MIPI0_MUX		GENMASK(17, 16)
#define RK3568_SYS_DSP_INFACE_EN_EDP_MUX		GENMASK(15, 14)
#define RK3568_SYS_DSP_INFACE_EN_HDMI_MUX		GENMASK(11, 10)
#define RK3568_SYS_DSP_INFACE_EN_RGB_MUX		GENMASK(9, 8)
#define RK3568_SYS_DSP_INFACE_EN_LVDS0			BIT(5)
#define RK3568_SYS_DSP_INFACE_EN_MIPI0			BIT(4)
#define RK3568_SYS_DSP_INFACE_EN_EDP			BIT(3)
#define RK3568_SYS_DSP_INFACE_EN_HDMI			BIT(1)
#define RK3568_SYS_DSP_INFACE_EN_RGB			BIT(0)

#define RK3588_SYS_DSP_INFACE_EN_MIPI1_MUX		GENMASK(22, 21)
#define RK3588_SYS_DSP_INFACE_EN_MIPI0_MUX		GENMASK(20, 20)
#define RK3588_SYS_DSP_INFACE_EN_EDP_HDMI1_MUX		GENMASK(19, 18)
#define RK3588_SYS_DSP_INFACE_EN_EDP_HDMI0_MUX		GENMASK(17, 16)
#define RK3588_SYS_DSP_INFACE_EN_DP1_MUX		GENMASK(15, 14)
#define RK3588_SYS_DSP_INFACE_EN_DP0_MUX		GENMASK(13, 12)
#define RK3588_SYS_DSP_INFACE_EN_DPI			GENMASK(9, 8)
#define RK3588_SYS_DSP_INFACE_EN_MIPI1			BIT(7)
#define RK3588_SYS_DSP_INFACE_EN_MIPI0			BIT(6)
#define RK3588_SYS_DSP_INFACE_EN_HDMI1			BIT(5)
#define RK3588_SYS_DSP_INFACE_EN_EDP1			BIT(4)
#define RK3588_SYS_DSP_INFACE_EN_HDMI0			BIT(3)
#define RK3588_SYS_DSP_INFACE_EN_EDP0			BIT(2)
#define RK3588_SYS_DSP_INFACE_EN_DP1			BIT(1)
#define RK3588_SYS_DSP_INFACE_EN_DP0			BIT(0)

#define RK3588_DSP_IF_MIPI1_PCLK_DIV			GENMASK(27, 26)
#define RK3588_DSP_IF_MIPI0_PCLK_DIV			GENMASK(25, 24)
#define RK3588_DSP_IF_EDP_HDMI1_PCLK_DIV		GENMASK(22, 22)
#define RK3588_DSP_IF_EDP_HDMI1_DCLK_DIV		GENMASK(21, 20)
#define RK3588_DSP_IF_EDP_HDMI0_PCLK_DIV		GENMASK(18, 18)
#define RK3588_DSP_IF_EDP_HDMI0_DCLK_DIV		GENMASK(17, 16)

#define RK3568_DSP_IF_POL__MIPI_PIN_POL			GENMASK(19, 16)
#define RK3568_DSP_IF_POL__EDP_PIN_POL			GENMASK(15, 12)
#define RK3568_DSP_IF_POL__HDMI_PIN_POL			GENMASK(7, 4)
#define RK3568_DSP_IF_POL__RGB_LVDS_PIN_POL		GENMASK(3, 0)

#define RK3588_DSP_IF_POL__DP1_PIN_POL			GENMASK(14, 12)
#define RK3588_DSP_IF_POL__DP0_PIN_POL			GENMASK(10, 8)

#define RK3568_VP0_MIPI_CTRL__DCLK_DIV2_PHASE_LOCK	BIT(5)
#define RK3568_VP0_MIPI_CTRL__DCLK_DIV2			BIT(4)

#define RK3568_SYS_AUTO_GATING_CTRL__AUTO_GATING_EN	BIT(31)

#define RK3568_DSP_IF_POL__CFG_DONE_IMD			BIT(28)

#define VOP2_SYS_AXI_BUS_NUM				2

#define VOP2_CLUSTER_YUV444_10				0x12

#define VOP2_COLOR_KEY_MASK				BIT(31)

#define RK3568_OVL_CTRL__LAYERSEL_REGDONE_IMD		BIT(28)
#define RK3568_OVL_CTRL__YUV_MODE(vp)			BIT(vp)

#define RK3568_VP_BG_MIX_CTRL__BG_DLY			GENMASK(31, 24)

#define RK3568_OVL_PORT_SEL__SEL_PORT			GENMASK(31, 16)
#define RK3568_OVL_PORT_SEL__SMART1			GENMASK(31, 30)
#define RK3568_OVL_PORT_SEL__SMART0			GENMASK(29, 28)
#define RK3588_OVL_PORT_SEL__ESMART3			GENMASK(31, 30)
#define RK3588_OVL_PORT_SEL__ESMART2			GENMASK(29, 28)
#define RK3568_OVL_PORT_SEL__ESMART1			GENMASK(27, 26)
#define RK3568_OVL_PORT_SEL__ESMART0			GENMASK(25, 24)
#define RK3588_OVL_PORT_SEL__CLUSTER3			GENMASK(23, 22)
#define RK3588_OVL_PORT_SEL__CLUSTER2			GENMASK(21, 20)
#define RK3568_OVL_PORT_SEL__CLUSTER1			GENMASK(19, 18)
#define RK3568_OVL_PORT_SEL__CLUSTER0			GENMASK(17, 16)
#define RK3568_OVL_PORT_SET__PORT2_MUX			GENMASK(11, 8)
#define RK3568_OVL_PORT_SET__PORT1_MUX			GENMASK(7, 4)
#define RK3568_OVL_PORT_SET__PORT0_MUX			GENMASK(3, 0)
#define RK3568_OVL_LAYER_SEL__LAYER(layer, x)		((x) << ((layer) * 4))

#define RK3568_CLUSTER_DLY_NUM__CLUSTER1_1		GENMASK(31, 24)
#define RK3568_CLUSTER_DLY_NUM__CLUSTER1_0		GENMASK(23, 16)
#define RK3568_CLUSTER_DLY_NUM__CLUSTER0_1		GENMASK(15, 8)
#define RK3568_CLUSTER_DLY_NUM__CLUSTER0_0		GENMASK(7, 0)

#define RK3568_CLUSTER_WIN_CTRL0__WIN0_EN		BIT(0)

#define RK3568_SMART_REGION0_CTRL__WIN0_EN		BIT(0)

#define RK3568_SMART_DLY_NUM__SMART1			GENMASK(31, 24)
#define RK3568_SMART_DLY_NUM__SMART0			GENMASK(23, 16)
#define RK3568_SMART_DLY_NUM__ESMART1			GENMASK(15, 8)
#define RK3568_SMART_DLY_NUM__ESMART0			GENMASK(7, 0)

#define VP_INT_DSP_HOLD_VALID	BIT(6)
#define VP_INT_FS_FIELD		BIT(5)
#define VP_INT_POST_BUF_EMPTY	BIT(4)
#define VP_INT_LINE_FLAG1	BIT(3)
#define VP_INT_LINE_FLAG0	BIT(2)
#define VOP2_INT_BUS_ERRPR	BIT(1)
#define VP_INT_FS		BIT(0)

#define POLFLAG_DCLK_INV	BIT(3)

enum vop2_layer_phy_id {
	ROCKCHIP_VOP2_CLUSTER0 = 0,
	ROCKCHIP_VOP2_CLUSTER1,
	ROCKCHIP_VOP2_ESMART0,
	ROCKCHIP_VOP2_ESMART1,
	ROCKCHIP_VOP2_SMART0,
	ROCKCHIP_VOP2_SMART1,
	ROCKCHIP_VOP2_CLUSTER2,
	ROCKCHIP_VOP2_CLUSTER3,
	ROCKCHIP_VOP2_ESMART2,
	ROCKCHIP_VOP2_ESMART3,
	ROCKCHIP_VOP2_PHY_ID_INVALID = -1,
};

extern const struct component_ops vop2_component_ops;

#endif /* _ROCKCHIP_DRM_VOP2_H */
