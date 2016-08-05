#---------------compile---------------
CC = @echo $(notdir $<);gcc
CC_NO_ECHO = gcc
AR = ar
LD = @echo $@;gcc

CP = cp
RM = rm
MAKE = make
MKDIR = mkdir

CFLAGS = -g -c -O2 -Wall
LIB_A_FLAGS = rcs
LIB_SO_FLAGS = -shared -fPic

#---------------variable---------------
XML2SQL_TARGET 		= $(XML2SQL_BASE)/xml2sql

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
XML2SQL_INC		= -I$(XML2SQL_INC_DIR)

XML2SQL_LIB_PATH	= -L.
XML2SQL_LIB		= -lxml2 -lsqlite3

XML2SQL_CFLAGS		= $(CFLAGS) 
