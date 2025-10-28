export LIB_EXT = .dll
export EXE_EXT = .exe
export BIN = $(abspath Bin)
export BIN_OBJ = $(abspath Bin-Obj)
export TARGET = $(BIN)/tester$(EXE_EXT)
export TARGET_LIB = $(BIN)/vren$(LIB_EXT)
export VREN_INCL = $(abspath VREN/Source)

.PHONY: all scaffold tester vren

all: scaffold vren tester 

tester:
	@make -C Tester/Vendor
	@make -C Tester

vren:
	@make -C VREN/Vendor
	@make -C VREN

scaffold:
	@mkdir -p $(BIN)
	@mkdir -p $(BIN_OBJ)
