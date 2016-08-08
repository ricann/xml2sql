.PHONY: all clean

ifeq ($(PLATFORM),ARM)
include ${XML2SQL_BASE}/make_arm.h
else ifeq ($(PLATFORM),X86_64)
include ${XML2SQL_BASE}/make_x86_64.h
else
include ${XML2SQL_BASE}/make_x86.h
endif

$(XML2SQL_TARGET): $(XML2SQL_OBJ) $(CONF_TARGET) $(UNP_TARGET)
	$(LD)  $^ -o $@ $(XML2SQL_LIB_PATH) $(XML2SQL_LIB)

#define compile rules
$(XML2SQL_OBJ_DIR)/%.o: $(XML2SQL_SRC_DIR)/%.c
	$(CC) $(XML2SQL_CFLAGS) $(XML2SQL_INC) $< -o $@

ifneq ($(MAKECMDGOALS), clean)
sinclude $(XML2SQL_DEP)
endif

$(XML2SQL_OBJ_DIR)/%.dep: $(XML2SQL_SRC_DIR)/%.c
	@set -e ; \
	$(RM) -fr $@.tmp ; \
	$(CC_NO_ECHO) $(XML2SQL_CFLAGS) $(XML2SQL_INC) -E -MM $(filter %.c, $^) > $@.tmp ; \
	sed 's,\($*\)\.o[ :]*,$(XML2SQL_OBJ_DIR)/\1.o $@: ,g' < $@.tmp > $@ ; \
	$(RM) -fr $@.tmp

clean:
	@echo "Cleaning object files"
	$(RM) -f *.o
	$(RM) -f *.dep
	$(RM) -f *.out
	$(RM) -f tags
	$(RM) -f $(XML2SQL_OBJ_DIR)/*.o
	$(RM) -f $(XML2SQL_OBJ_DIR)/*.dep
	$(RM) -f $(XML2SQL_TARGET)

print:
	@echo XML2SQL_TARGET=${XML2SQL_TARGET}
	@echo XML2SQL_INC_DIR=${XML2SQL_INC_DIR}
	@echo XML2SQL_SRC_DIR=${XML2SQL_SRC_DIR}
	@echo XML2SQL_OBJ_DIR=${XML2SQL_OBJ_DIR}
	@echo _XML2SQL_OBJ_DIR=${_XML2SQL_OBJ_DIR}
	@echo XML2SQL_DIRS=${XML2SQL_DIRS}
	@echo XML2SQL_RMS=${XML2SQL_RMS}
	@echo XML2SQL_SRC=${XML2SQL_SRC}
	@echo XML2SQL_OBJ=${XML2SQL_OBJ}
	@echo XML2SQL_DEP=${XML2SQL_DEP}
	@echo XML2SQL_INC=${XML2SQL_INC}
	@echo XML2SQL_CFLAGS=${XML2SQL_CFLAGS}
