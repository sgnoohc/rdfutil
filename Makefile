# ===== User configuration =====
CXX        := g++
CXXFLAGS   := -O2 -Wall -Wextra -std=c++17
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS   := $(shell root-config --libs)

# ===== Sources and targets =====
SRCS := $(wildcard *.cc)
BINS := $(SRCS:.cc=)

# ===== Build rules =====
all: $(BINS)

%: %.cc
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -o $@ $< $(ROOTLIBS)

clean:
	rm -f $(BINS) *.o

rebuild: clean all

.PHONY: all clean rebuild
