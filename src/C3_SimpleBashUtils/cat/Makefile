# Имя исполняемого файла
TARGET = s21_cat

# Исходные файлы
SRCS = s21_cat.c

# Заголовочные файлы
HDRS = s21_cat.h

# Объектные файлы
OBJS = $(SRCS:.c=.o)

# Компилятор
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Werror -Wextra -std=c11 -g -pedantic -lm

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Правило для компиляции .c файлов в .o файлы
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для очистки проекта
clean:
	rm -f $(OBJS) $(TARGET)

# Правило для пересоздания проекта
rebuild: clean all

# Правило для запуска программы
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
