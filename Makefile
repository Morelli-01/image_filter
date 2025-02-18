
CXX = g++
CXXFLAGS = -std=c++23 -w $(EXTRA_FLAGS)
OPENMP_FLAGS = -fopenmp
LDFLAGS = $(OPENMP_FLAGS)

BUILD_DIR = build
BIN_DIR = bin


OPENCV_LIBS = $(shell pkg-config --libs opencv4)
OPENCV_INCLUDES = $(shell pkg-config --cflags opencv4)
TESSERACT_LIBS = -ltesseract -llept
BOOST_LIBS = -lboost_program_options  
INCLUDES = -I/usr/include/tesseract -I/usr/include/leptonica $(OPENCV_INCLUDES)


SRC = main.cpp
OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC))
TARGET = $(BIN_DIR)/image_filter

DEBUG_ARGS = -i /home/nicola/Desktop/immagini_ecografie/ -o /home/nicola/Desktop/video_ecografie -e .avi -t TESTI

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $@ $^ $(LDFLAGS) $(TESSERACT_LIBS) $(OPENCV_LIBS) $(BOOST_LIBS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) $(INCLUDES) -c $< -o $@

RELEASE_CXXFLAGS = -std=c++23 -O3 -DNDEBUG -w $(EXTRA_FLAGS)
release: clean
release: CXXFLAGS=$(RELEASE_CXXFLAGS)
release: all

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

debug: clean all
	gdb --args ./$(TARGET) $(ARGS)

run: clean all
	./$(TARGET) $(DEBUG_ARGS)

.PHONY: all clean release
