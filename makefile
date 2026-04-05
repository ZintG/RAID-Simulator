# 1. Nombre base del programa
TARGET_BASE := programa_compilado

# 2. Deteccion del Sistema Operativo
ifeq ($(OS),Windows_NT)
    # --- Configuracion para WINDOWS ---
    # En Windows los ejecutables llevan .exe
    TARGET  := $(TARGET_BASE).exe
    # Comando para borrar (del) suprimiendo errores (2>nul)
    RM_CMD  := del /Q /F *.o 2>nul & del /Q /F libraries\*.o 2>nul & del /Q /F $(TARGET_BASE).exe 2>nul
    # Comando para ejecutar en cmd/PowerShell
    RUN_CMD := $(TARGET)
    MSG     := Compilacion en entorno Windows finalizada.
else
    # --- Configuracion para LINUX / DEBIAN ---
    # En Linux no hay extension obligatoria
    TARGET  := $(TARGET_BASE)
    # Comando para borrar (rm) forzado (-f)
    RM_CMD  := rm -f *.o libraries/*.o $(TARGET)
    # Comando para ejecutar en bash
    RUN_CMD := ./$(TARGET)
    MSG     := Compilacion en entorno Linux finalizada.
endif

# 3. Compilador y Flags
CXX      := g++
CXXFLAGS := -std=c++98 -Wall -Wextra
INCLUDES := -Ilibraries

# 4. Deteccion automatica (Make maneja '/' internamente sin problema)
SOURCES  := $(wildcard *.cpp libraries/*.cpp)
OBJECTS  := $(SOURCES:.cpp=.o)

# 5. Reglas de compilacion
.PHONY: all clean run

all: $(TARGET)

# Vinculacion final
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJECTS) -o $(TARGET)
	@echo "--------------------------------------------"
	@echo "$(MSG)"
	@echo "Listo! Ejecutable creado como: $(TARGET)"
	@echo "--------------------------------------------"

# Compilacion de objetos
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regla para ejecutar
run: all
	$(RUN_CMD)

# Regla para limpiar la carpeta
clean:
	-$(RM_CMD)
	@echo "Archivos temporales eliminados."