# 🏗️ Полное руководство по созданию Makefile для C проектов

*Полный гайд по всем аспектам создания Makefile, основанный на наших обсуждениях*

---

## 📁 Оглавление

1. [Структура проекта](#-структура-проекта)
2. [Базовый синтаксис Makefile](#-базовый-синтаксис-makefile)
3. [Переменные и присваивание](#-переменные-и-присваивание)
4. [Компиляция и линковка](#-компиляция-и-линковка)
5. [Тестирование с Check](#-тестирование-с-check)
6. [Coverage отчеты](#-coverage-отчеты)
7. [Статические библиотеки](#-статические-библиотеки)
8. [Полезные цели](#-полезные-цели)
9. [Частые ошибки](#-частые-ошибки)

---

## 🗂️ Структура проекта

### Рекомендуемая структура:
```
project/
├── include/           # 📚 Заголовочные файлы
│   └── *.h
├── src/               # 💻 Исходный код
│   └── *.c
├── tests/             # 🧪 Тесты
│   ├── main.c
│   └── test_*.c
└── Makefile          # 🛠️ Этот файл!
```

### Build структура (создается автоматически):
```
build/
├── obj/
│   ├── main/         # 🎯 Объектные файлы основной программы
│   └── tests/        # 🧪 Объектные файлы тестов
├── bin/
│   ├── main/         # 🚀 Исполняемые файлы
│   └── tests/
├── lib/              # 📚 Библиотеки
└── gcov_report/      # 📊 Отчеты coverage
    ├── data/         # 📈 Данные .gcda
    └── html/         # 🌐 HTML отчет
```

---

## 📝 Базовый синтаксис Makefile

### 1. Правила (Rules)
```makefile
цеель: зависимости
	команда1
	команда2
```

**Пример:**
```makefile
main: main.o utils.o
	gcc main.o utils.o -o main

main.o: main.c
	gcc -c main.c -o main.o
```

### 2. Автоматические переменные 🔄
- **`$@`** - имя цели
- **`$<`** - первая зависимость
- **`$^`** - все зависимости
- **`$?`** - зависимости новее цели
- **`$*`** - В pattern-правиле test_%:, символ % представляет собой шаблон, а $* содержит то, что было сопоставлено с этим %.

### 3. Комментарии и echo
```makefile
# Обычный комментарий (не виден при выполнении)

test:
	@echo "Это НЕ покажет команду echo"  # 👻 Скрывает саму команду
	echo "Это ПОКАЖЕТ команду echo"      # 👀 Показывает команду
```

---

## 🔧 Переменные и присваивание

### Типы присваивания:

#### 1. **`=`** - Recursive (ленивое вычисление) ⏳
```makefile
CC = gcc
CFLAGS = -Wall

# Вычисляется при использовании!
FLAGS = $(CC) $(CFLAGS)  # Если CC изменится, FLAGS тоже изменится
```

#### 2. **`:=`** - Simple (немедленное вычисление) ⚡
```makefile
CC := gcc
CFLAGS := -Wall

# Вычисляется сразу!
FLAGS := $(CC) $(CFLAGS)  # Фиксируется на момент определения
```

#### 3. **`::=`** - Immediate (фиксированное) 🔒
```makefile
CC ::= gcc
CFLAGS ::= -Wall

# Фиксируется и не может быть переопределено
```

#### 4. **`+=`** - Append (добавление) ➕
```makefile
CFLAGS = -Wall
CFLAGS += -Werror  # Теперь CFLAGS = -Wall -Werror
```

#### 5. **`?=`** - Conditional (условное) ❓
```makefile
CC ?= gcc  # Установится только если CC еще не определено
```

**💡 Рекомендация:** Используйте `:=` для большинства случаев!

---

## 🚀 Компиляция и линковка

### Базовые флаги компилятора 🛡️

```makefile
CFLAGS = -Wall          # ⚠️  Все предупреждения
CFLAGS += -Werror       # ❌  Превратить предупреждения в ошибки
CFLAGS += -Wextra       # 🔍  Дополнительные предупреждения
CFLAGS += -std=c11      # 📅  Стандарт C11
CFLAGS += -pedantic     # 📏  Строгое соответствие стандарту
CFLAGS += -I./include   # 📁  Путь к заголовочным файлам
```

### Паттерн-правила для компиляции 🔄

```makefile
# Преобразование исходников в объектные файлы
SRC = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, build/obj/%.o, $(SRC))

# Правило компиляции
build/obj/%.o: src/%.c | build/obj
	$(CC) $(CFLAGS) -c $< -o $@

# Order-only зависимость (директория)
build/obj:
	mkdir -p $@
```

### Линковка исполняемого файла 🔗

```makefile
build/bin/main: $(OBJS) | build/bin
	$(CC) $^ -o $@  # $^ = ВСЕ зависимости

# ❌ НЕПРАВИЛЬНО:
	$(CC) $< -o $@  # $< = только ПЕРВАЯ зависимость
```

**📝 Запомните:** 
- **Компиляция** (`-c`) создает `.o` файлы из `.c` файлов
- **Линковка** объединяет `.o` файлы в исполняемый файл
- **Порядок:** Компиляция → Линковка (никогда наоборот!)

---

## 🧪 Тестирование с Check

### Установка Check 📦
```bash
# Ubuntu/Debian
sudo apt-get install check pkg-config

# macOS
brew install check pkg-config
```

### Флаги для тестов 🎯

```makefile
# Получаем флаги от pkg-config
TST_CFLAGS = $(CFLAGS) $(shell pkg-config --cflags check)
TST_LIBS = $(shell pkg-config --libs check)

# Что возвращает pkg-config:
# --cflags check: -I/usr/include/check
# --libs check: -lcheck -lm -lpthread
```

### Структура тестового main.c 📋

```c
#include <check.h>
#include "your_header.h"

// Объявления test cases
TCase* test_math(void);
TCase* test_string(void);

int main(void) {
    Suite *s = suite_create("All Tests");
    
    // Регистрируем test cases
    suite_add_tcase(s, test_math());
    suite_add_tcase(s, test_string());
    
    // Запускаем
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

### Makefile для тестов 🛠️

```makefile
# Тестовые файлы
TESTS = tests/main.c $(wildcard tests/test_*.c)
TEST_OBJS = $(patsubst tests/%.c, build/obj/tests/%.o, $(TESTS))
TEST_BIN = build/bin/tests/runner

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJS) $(OBJS) | build/bin/tests
	$(CC) $(TEST_OBJS) $(OBJS) -o $@ $(TST_LIBS)

build/obj/tests/%.o: tests/%.c | build/obj/tests
	$(CC) $(TST_CFLAGS) -c $< -o $@
```

---

## 📊 Coverage отчеты

### Флаги для coverage 📈

```makefile
GCOV_FLAGS = -fprofile-arcs -ftest-coverage  # 🎯 Для компиляции
LDFLAGS = -lgcov                             # 🔗 Для линковки

# Объяснение флагов:
# -fprofile-arcs     : Сбор информации о прохождении ветвей кода
# -ftest-coverage    : Генерация .gcno/.gcda файлов
# -lgcov            : Подключение библиотеки coverage
```

### Файлы coverage 📁

- **`.gcno`** - карта кода (создается при компиляции) 🗺️
- **`.gcda`** - данные выполнения (создается при запуске) 📊

### Цель gcov_report 🎯

```makefile
GCOV_ENV = GCOV_PREFIX=$(GCOV_DATA_DIR)
GCOV_DATA_DIR = build/gcov_report/data

gcov_report: $(TEST_BIN) | build/gcov_report build/gcov_report/data
	# 🚀 Запускаем тесты с перенаправлением .gcda
	$(GCOV_ENV) ./$(TEST_BIN)
	
	# 📦 Собираем данные coverage
	lcov -t "Coverage Report" -o build/gcov_report/coverage.info \
	     -c -d $(GCOV_DATA_DIR) --rc lcov_branch_coverage=1
	
	# 🌐 Генерируем HTML отчет
	genhtml build/gcov_report/coverage.info \
	        -o build/gcov_report/html --rc genhtml_branch_coverage=1
	
	@echo "========================================="
	@echo "📊 Coverage report: build/gcov_report/html/index.html"
	@echo "========================================="
```

### Установка инструментов coverage 🔧

```bash
# Ubuntu/Debian
sudo apt-get install lcov gcovr

# macOS  
brew install lcov gcovr
```

---

## 📚 Статические библиотеки

### Создание библиотеки 🏗️

```makefile
AR = ar rcs
RANLIB = ranlib
LIB_DIR = build/lib
STATIC_LIB = $(LIB_DIR)/s21_matrix.a

lib: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS) | $(LIB_DIR)
	$(AR) $@ $(OBJS)    # 📦 Создаем архив
	$(RANLIB) $@        # 📇 Создаем индекс

$(LIB_DIR):
	mkdir -p $@
```

### Объяснение ar rcs 🔍

- **`ar`** - архиватор
- **`r`** - replace/добавить файлы в архив
- **`c`** - create/создать архив если не существует  
- **`s`** - index/создать индекс символов

### Проверка библиотеки 🔎

```bash
# Просмотр содержимого
ar -t build/lib/s21_matrix.a

# Просмотр символов (функций)
nm build/lib/s21_matrix.a
```

---

## 🎯 Полезные цели

### Базовые цели 🎯

```makefile
.PHONY: all test clean lib gcov_report view_report

all: build/bin/main

test: build/bin/tests/runner
	./build/bin/tests/runner

lib: build/lib/s21_matrix.a

clean:
	rm -rf build/
	find . -name "*.gcno" -delete
	find . -name "*.gcda" -delete
	find . -name "*.gcov" -delete

# 🔍 Просмотр отчета (кроссплатформенный)
view_report:
	xdg-open build/gcov_report/html/index.html 2>/dev/null || \
	open build/gcov_report/html/index.html 2>/dev/null || \
	echo "📂 Open manually: build/gcov_report/html/index.html"
```

### Отладочные цели 🐛

```makefile
# 📝 Информация о проекте
info:
	@echo "Sources: $(SRC)"
	@echo "Objects: $(OBJS)"
	@echo "Tests: $(TESTS)"

# 🎯 Запуск конкретного теста
test_%: build/bin/tests/runner
	./build/bin/tests/runner -s $*

# 🔧 Debug сборка
debug: CFLAGS += -g -DDEBUG
debug: clean all
```

---

## ❌ Частые ошибки

### 1. **Неправильные пути** 🗺️
```makefile
# ❌ НЕПРАВИЛЬНО
TESTS = $(wildcard src/*.c)  # Берет исходники вместо тестов!

# ✅ ПРАВИЛЬНО  
TESTS = $(wildcard tests/*.c)
```

### 2. **Пробелы в командах** ␣
```makefile
# ❌ НЕПРАВИЛЬНО
	gcc -c file.c - o file.o  # Пробел между - и o!

# ✅ ПРАВИЛЬНО
	gcc -c file.c -o file.o
```

### 3. **Путаница с автоматическими переменными** 🔄
```makefile
# ❌ НЕПРАВИЛЬНО (для линковки)
	$(CC) $< -o $@  # $< = только первая зависимость

# ✅ ПРАВИЛЬНО
	$(CC) $^ -o $@  # $^ = все зависимости
```

### 4. **Отсутствие .PHONY** 👻
```makefile
# ❌ Без .PHONY
clean:
	rm -rf build/

# ✅ С .PHONY  
.PHONY: clean
clean:
	rm -rf build/
```

### 5. **Смешивание .gcda с объектными файлами** 📊
```makefile
# ❌ .gcda в obj директории
lcov -d build/obj/ ...

# ✅ .gcda в отдельной директории  
lcov -d build/gcov_report/data/ ...
```

---

## 🎉 Заключение

### Ключевые моменты для запоминания:

1. **🎯 Всегда используйте `:=` для присваивания**
2. **📁 Разделяйте исходные, объектные и бинарные файлы**
3. **🧪 Используйте `.PHONY` для целей-команд**
4. **📊 Храните .gcda файлы отдельно от объектных файлов**
5. **🔧 Используйте автоматические переменные правильно**