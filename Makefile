APPPATH=app
ENGINEPATH=engine
LIBPATH=${ENGINEPATH}/libraries

# Operating system detection if not defined by user
ifeq ($(origin System), undefined)
  UNAME := $(shell uname -s)
  ifneq (,$(findstring NT,$(UNAME)))
    System := Windows
  else ifneq (,$(findstring Darwin,$(UNAME)))
    System := Mac
  else ifneq (,$(findstring Linux,$(UNAME)))
    System := Linux
  endif
endif

# Compiler and linker settings based on operating system
ifeq ($(System), Windows)
	CXX := clang++
	INC := -I. -I ${LIBPATH}/glfw-3.5.1/include -I ${LIBPATH}/stb -I ${LIBPATH}/glad/include -I ${LIBPATH}/json
	LIB := -L ${LIBPATH}/glfw-3.5.1/bin/win-lib-mingw-w64
	LNK := -stdlib=libc++ -static-libstdc++ -l glfw3 -l gdi32 -l opengl32
	OPT := -std=c++20 -Og -g3 -fno-inline -fno-limit-debug-info

else ifeq ($(System), Mac)
	CXX := clang++
	INC := -I. -I ${LIBPATH}/glfw-3.5.1/include -I ${LIBPATH}/stb -I ${LIBPATH}/glad/include -I ${LIBPATH}/json
	LIB := -L ${LIBPATH}/glfw-3.5.1/bin/mac-lib-universal
	LNK := -l glfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework QuartzCore
	OPT := -std=c++20 -arch arm64 -arch x86_64 -Wno-deprecated-declarations -Og -g3 -fno-inline -fno-limit-debug-info

else ifeq ($(System), Linux)
	CXX := clang++
	INC := -I. -I ${LIBPATH}/glfw-3.5.1/include -I ${LIBPATH}/stb -I ${LIBPATH}/glad/include -I ${LIBPATH}/json
	LNK := -l glfw -l rt -l m -l dl -l GL -l pthread
	OPT := -std=c++20 -Og -g3 -fno-inline -fno-limit-debug-info

endif

SRC=$(wildcard ${ENGINEPATH}/src/*/*.cpp ${APPPATH}/*.cpp ${APPPATH}/src/*.cpp)
OBJ=$(SRC:%.cpp=obj/%.o)
DEP=$(OBJ:.o=.d)

bin/mygame: ${OBJ}
	@echo Linking $@ ...
	@mkdir -p $(dir $@)
	@$(CXX) $^ -o $@ $(LIB) $(LNK) $(OPT)

obj/%.o: %.cpp
	@echo Compiling $< ➔ $@ ...
	@mkdir -p $(dir $@)
	@$(CXX) -MMD -MP -MF $(@:.o=.d) -c $< -o $@ $(INC) $(OPT)

run: bin/mygame
	@echo Running $< ...
	@./$< "$(CURDIR)/app/assets"

clean:
	@echo Cleaning ...
	@rm -rf obj bin

.PHONY: clean run

-include $(DEP)
