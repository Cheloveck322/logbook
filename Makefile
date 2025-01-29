# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -pthread

# Имена целевых файлов
LIBRARY := libbook.so
APP := logapp

# Исходные файлы
LIB_SOURCES := src/LogBook.cpp
APP_SOURCES := src/LogApp.cpp src/main.cpp

# Правила сборки
all: $(LIBRARY) $(APP)

# Сборка динамической библиотеки
$(LIBRARY): $(LIB_SOURCES) include/LogBook.h
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $(LIB_SOURCES)

# Сборка приложения
$(APP): $(APP_SOURCES) include/LogApp.h $(LIBRARY)
	$(CXX) $(CXXFLAGS) -o $@ $(APP_SOURCES) -L. -lbook -Wl,-rpath=.

#Цель для запуска приложения
run: all
	@echo "Running app..."
	@LD_LIBRARY_PATH=. ./$(APP) output.txt INFO

# Очистка
clean:
	rm -f $(LIBRARY) $(APP) *.o 

.PHONY: all clean