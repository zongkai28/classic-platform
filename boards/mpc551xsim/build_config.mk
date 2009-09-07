
# ARCH defines
ARCH=mpc55xx
ARCH_FAM=ppc
ARCH_MCU=mpc5516

# CFG (y/n) macros
CFG=PPC BOOKE E200Z1 MPC55XX MPC5516 BRD_MPC551XSIM SIMULATOR

# What buildable modules does this board have, 
# default or private
MOD_AVAIL=KERNEL MCU GPT LIN CAN WDG WDGM T32_TERM WINIDEA_TERM

# Needed by us
MOD_USE=KERNEL MCU
