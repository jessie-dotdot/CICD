# Имя исполняемого файла
TARGET = s21_grep

# Исходные файлы
SRCS = $(wildcard *.c)

# Заголовочные файлы
HDRS = $(wildcard *.h)

# Объектные файлы
OBJS = $(SRCS:.c=.o)

# Компилятор
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Werror -Wextra -std=c11 -lregex -fsanitize=leak

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Правило для компиляции .c файлов в .o файлы
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для запуска valgrind
valgrind: $(TARGET)
	@echo "Running valgrind memory checks..."
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

# Правило для очистки проекта
clean:
	rm -f $(OBJS) $(TARGET)

# Правило для пересоздания проекта
rebuild: clean all

# Правило для запуска программы
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run valgrind
