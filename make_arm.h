#---------------compile---------------
CC = @echo $(notdir $<);arm-poky-linux-gnueabi-gcc
CC_NO_ECHO = arm-poky-linux-gnueabi-gcc
AR = arm-poky-linux-gnueabi-ar
LD = @echo $@;arm-poky-linux-gnueabi-gcc

CP = cp
RM = rm
MAKE = make
MKDIR = mkdir

CFLAGS = -g -c -O2 -Wall
LIB_A_FLAGS = rcs
LIB_SO_FLAGS = -shared -fPIC

#---------------variable---------------
XML2SQL_TARGET_EXE 		= $(XML2SQL_BASE)/x2s
XML2SQL_TARGET_LIBA 	= $(XML2SQL_BASE)/lib/libx2s.a
XML2SQL_TARGET_LIBSO 	= $(XML2SQL_BASE)/lib/libx2s.so
XML2SQL_DFN_EXE			= -D MACRO_DFN_EXE
XML2SQL_DFN_LIBA		= -D MACRO_DFN_LIBA
XML2SQL_DFN_LIBSO		= -D MACRO_DFN_LIBSO

XML2SQL_INC_DIR		= $(XML2SQL_BASE)/inc
XML2SQL_SRC_DIR		= $(XML2SQL_BASE)/src
XML2SQL_OBJ_DIR		= $(XML2SQL_BASE)/obj

XML2SQL_DIRS		= $(XML2SQL_INC_DIR)  $(XML2SQL_SRC_DIR)  $(XML2SQL_OBJ_DIR)
XML2SQL_SRC		= $(wildcard $(XML2SQL_SRC_DIR)/*.c)
XML2SQL_OBJ		= $(XML2SQL_SRC:.c=.o)
XML2SQL_OBJ		:= $(filter %.o, $(XML2SQL_OBJ))
XML2SQL_OBJ		:= $(patsubst $(XML2SQL_SRC_DIR)/%, $(XML2SQL_OBJ_DIR)/%, $(XML2SQL_OBJ)) 
XML2SQL_DEP		= $(XML2SQL_SRC:.c=.dep)
XML2SQL_DEP		:= $(filter %.dep, $(XML2SQL_DEP))
XML2SQL_DEP		:= $(patsubst $(XML2SQL_SRC_DIR)/%, $(XML2SQL_OBJ_DIR)/%, $(XML2SQL_DEP)) 
XML2SQL_INC		= -I/usr/include/libxml2 -I$(XML2SQL_INC_DIR) 

XML2SQL_INC_LIB_PATH	= -L.
XML2SQL_INC_LIB			= -lxml2 -lsqlite3

# if not add -fPIC, can not get a .so file 
XML2SQL_CFLAGS		= $(CFLAGS) -fPIC
