PROG_NAME=prog

CFLAGS = -O2 -Wall -Werror -std=c17 -Wdiscarded-qualifiers -Wincompatible-pointer-types -Wint-conversion
CC = gcc

ASM=nasm
ASMFLAGS=-felf64

# Папка в которой будут храниться .d файлы - файлы зависимостей.
# Они генерируются непосредственно компилятором при помощи флагов -M...
# P.S. gcc так же поддерживает эти флаги
DEPDIR := .deps
ASMDEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.d
CDEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

LD=gcc

# Парсим директорию и находим все .asm файлы, пишем их в строку через пробел
ASM_FILES=$(shell find . -name '*.asm' | xargs echo | sed "s/.\///g")
C_FILES=$(shell find . -name '*.c' | xargs echo | sed "s/.\///g")

OBJDIR := .build
# patsubst заменяет вхождения первого аргумента на второй
# в каждом слове третьего аргумента(слова разделены пробелом)
OBJ_FILES=$(patsubst %.asm, $(OBJDIR)/%.o, $(ASM_FILES) $(C_FILES))

all: $(PROG_NAME)

debug: ASMFLAGS += -g
debug: CFLAGS += -g
debug: $(PROG_NAME)

break: CFLAGS += -fno-stack-protector -z execstack -no-pie
break: $(PROG_NAME)

$(PROG_NAME) : $(OBJ_FILES)
	$(LD) $(LFLAGS) -o $@ $^

$(OBJDIR)/%.o : %.asm $(DEPDIR)/%.d | $(DEPDIR) $(OBJDIR)
	$(ASM) $(ASMDEPFLAGS) $(ASMFLAGS) -o $@ $<

$(OBJDIR)/%.o : %.c $(DEPDIR)/%.d | $(DEPDIR) $(OBJDIR)
	$(CC) $(CDEPFLAGS) $(CFLAGS) -o $@ $<

$(DEPDIR) $(OBJDIR) : ; @mkdir -p $@

DEP_FILES = $(patsubst %.asm, $(DEPDIR)/%.d, $(ASM_FILES))
$(DEP_FILES): $(DEPDIR)

include $(wildcard $(DEP_FILES))

.PHONY: run
run:
	@./$(PROG_NAME)

.PHONY: clean
clean:
	rm -rf $(PROG_NAME)
	rm -rf $(DEPDIR)
	rm -rf $(OBJDIR)
