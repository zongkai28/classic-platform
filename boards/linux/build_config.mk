
# ARCH defines
ARCH=linux
ARCH_FAM=generic
#ARCH=mpc55xx
#ARCH_FAM=ppc
#ARCH_MCU=mpc5516

# CFG (y/n) macros
CFG= BRD_LINUX

# What buildable modules does this board have, 
# default or private
MOD_AVAIL+=COM PDUR WDGM WDGIF DET DCM DEM CANTP CANIF RTE FEE NVM ECUM MCU
# Network management
MOD_AVAIL+=COMM NM CANNM CANSM

# Needed by us
MOD_USE=

# Stubs
obj-y += McuExtensionsStub.o