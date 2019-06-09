all:
export RTE_SDK := /home/emil/Documents/dpdk
export RTE_TARGET := x86_64-native-linux-gcc

include $(RTE_SDK)/mk/rte.vars.mk

APP_DIR := /home/emil/Documents/MGR/mgr
CC := g++-8 
APP := mgr
_SRC_DIR := $(APP_DIR)/src
SRCS-y := $(shell find $(_SRC_DIR) -type f -name *.cpp -or -name *.c)
INCLUDE_DIR := include
_OUTPUT_DIR := $(APP_DIR)/build
RTE_EXTMK = $(APP_DIR)/Makefile
RTE_SRCDIR = $(_SRC_DIR)
RTE_OUTPUT := $(_OUTPUT_DIR)

INCLUDES := -I$(INCLUDE_DIR)
CFLAGS := -std=c++2a -O0 -g -ggdb $(INCLUDES) $(CFLAGS)
_TARGET_TYPE := APP
_LDLIBS := -L/usr/lib/x86_64-linux-gnu -lpthread -lm
LDLIBS := $(_LDLIBS) 

POSTBUILD += move_files
move_files:
	@mkdir -p $(_OUTPUT_DIR)/obj
	@mv $(_SRC_DIR)/*.o $(_OUTPUT_DIR)/obj
	@mv $(_SRC_DIR)/.*.o.* $(_OUTPUT_DIR)/obj

include $(RTE_SDK)/mk/rte.extapp.mk

clean:
	@echo "Cleaning"
	@rm -rvf $(_OUTPUT_DIR) $(_SRC_DIR)/*.o
