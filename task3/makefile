PROG_NAME=prog

CFLAGS = -c -O2 -Wall -Werror -std=c11 -Wdiscarded-qualifiers -Wincompatible-pointer-types -Wint-conversion
CC = gcc

# Папка в которой будут храниться .d файлы - файлы зависимостей.
# Они генерируются непосредственно компилятором при помощи флагов -M...
DEPDIR := .deps
CDEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

LD=gcc

SRCDIR := src
CFLAGS += $(shell find ${SRCDIR} -type d | sed 's/^/-I/g')

# Парсим директорию и находим все .c файлы, пишем их в строку через пробел
C_FILES=$(shell find ${SRCDIR}/ -name '*.c' | xargs echo | sed 's/${SRCDIR}\///g')

OBJDIR := .build
# patsubst заменяет вхождения первого аргумента на второй
# в каждом слове третьего аргумента(слова разделены пробелом)
OBJ_FILES=$(patsubst %.c, $(OBJDIR)/%.o, $(C_FILES))

all: $(PROG_NAME)

debug: CFLAGS += -g
debug: $(PROG_NAME)

$(PROG_NAME) : $(OBJ_FILES)
	$(LD) $(LFLAGS) -o $@ $^

$(OBJDIR)/%.o : ${SRCDIR}/%.c | ${DEPDIR} ${OBJDIR}
	$(CC) $(CDEPFLAGS) $(CFLAGS) -o $@ $<
	$(POSTCOMPILE)
	
$(OBJDIR)/%.o : ${SRCDIR}/%.c $(DEPDIR)/%.d | $(DEPDIR) $(OBJDIR)
	$(CC) $(CDEPFLAGS) $(CFLAGS) -o $@ $<
	$(POSTCOMPILE)

$(DEPDIR) $(OBJDIR) : ; @mkdir -p $(shell find $(SRCDIR)/ -type d | sed 's/$(SRCDIR)/$@/g')

DEP_FILES = $(patsubst %.c, $(DEPDIR)/%.d, $(С_FILES))
$(DEP_FILES): $(DEPDIR)

TESTDIR := tests
TESTS_C = $(shell find $(TESTDIR)/ -name '*.c' | xargs echo | sed 's/tests\///g')
TESTS = $(subst .c,,$(TESTS_C))
TESTS_OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(TESTS_C))

# Запускаем каждый тестовый файл отдельно, линкуем его со всем кроме main.o
.SILENT:$(TESTS : %)
$(TESTS): % : $(OBJDIR)/%.o $(filter-out $(OBJDIR)/main.o, $(OBJ_FILES))
	$(LD) $(LFLAGS) -o $@ $^
	@echo "\nRunning file $@"
	@./$@
	
$(OBJDIR)/%.o : ${TESTDIR}/%.c | ${DEPDIR} ${OBJDIR}
	$(CC) $(CDEPFLAGS) $(CFLAGS) -o $@ $<
	$(POSTCOMPILE)
	
$(OBJDIR)/%.o : ${TESTDIR}/%.c $(DEPDIR)/%.d | $(DEPDIR) $(OBJDIR)
	$(CC) $(CDEPFLAGS) $(CFLAGS) -o $@ $<
	$(POSTCOMPILE)

# Создаем тестовые бинарники, запускаем и удаляем.
test: $(TESTS)
	@rm -rf $(TESTS)

.PHONY: run
run:
	@./$(PROG_NAME)

.PHONY: clean
clean:
	@rm -rf $(PROG_NAME)
	@rm -rf $(DEPDIR)
	@rm -rf $(OBJDIR)
	@rm -rf $(TESTS)

include $(wildcard $(DEPDIR)/*.d)