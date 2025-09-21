# Compiler settings
CXX := g++
CXXFLAGS := -Wall -Wextra -g3 
LDFLAGS := 
# -dynamic
# -W -rpath='$$ORIGIN'
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

# Project directories
TEST_DIR := test
TEST_BUILD_DIR := test_output

SRC_DIR := src
BUILD_DIR := bin
ICON_DIR := icon

##下面两个路径结合自身情况修改
INCLUDE_DIR := D:/AAA_C/AAA_Lib/SFML/SFML-3_0_0-64bit/include
LIB_DIR := D:/AAA_C/AAA_Lib/SFML/SFML-3_0_0-64bit/lib

# Source files and target
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_TARGET := $(TEST_BUILD_DIR)/main.exe

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
TARGET := $(BUILD_DIR)/PVZ.exe

# Additional flags
INCLUDE_FLAGS := -I$(INCLUDE_DIR)
LIB_FLAGS := -L$(LIB_DIR)

.PHONY: testall testclean testrun all cl run icon

all: $(TARGET)
$(TARGET):$(SRCS)
	mkdir -p $(ICON_DIR) 2>nul || echo
# powershell 格式命令
	powershell -Command "echo 'MAINICON ICON \"$(ICON_DIR)/PVZ.ico\"' | Out-File -Encoding ASCII $(ICON_DIR)/PVZ.rc"
	windres $(ICON_DIR)/PVZ.rc -O coff -o $(ICON_DIR)/PVZ.res
	
	mkdir -p $(BUILD_DIR) 2>nul || echo
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) $^  -o $@ $(LIB_FLAGS) $(LDLIBS) $(LDFLAGS)  $(ICON_DIR)/PVZ.res -mwindows

# 可以选添 -mwindows 效果是程序不打开命令行窗口


testall: $(TEST_TARGET)
$(TEST_TARGET): $(TEST_SRCS)
	mkdir -p $(TEST_BUILD_DIR) 2>nul || echo
# 2>nul || echo 可以把重复创建文件错误信息忽略到空的文件中
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) $^ -o $@ $(LIB_FLAGS) $(LDLIBS)


testclean:
	rm -rf $(TEST_BUILD_DIR)/*.exe

cl:
	rm -rf $(BUILD_DIR)/PVZ.exe

testrun:
	$(TEST_BUILD_DIR)/main.exe

run:all
	./PVZ.exe

icon/PVZ.rc: icon/PVZ.ico
	
icon:
# PowerShell 格式# echo 'MAINICON ICON "$(ICON_DIR)/PVZ.ico"' | Out-File -Encoding ASCII $(ICON_DIR)/PVZ.rc

#Shell 格式
	echo 'MAINICON ICON "$(ICON_DIR)/PVZ.ico"' > $(ICON_DIR)/PVZ.rc 
	windres $(ICON_DIR)/PVZ.rc -O coff -o $(ICON_DIR)/PVZ.res


# if not exist $(BUILD_DIR)
# mkdir -p $(BUILD_DIR)