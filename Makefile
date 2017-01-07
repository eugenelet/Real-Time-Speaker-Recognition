RCS = $(wildcard *.cpp)
PROGS = $(patsubst %.cpp,%,$(SRCS))
OBJS = $(SRCS:.cpp=.o)
TEMPS = $(SRCS:.cpp=.txt)
SRC = main.cpp train_test.cpp record.cpp
CFLAGS = -O3
LDFLAGS = -lalut -lopenal
OUT = record

all: $(OUT)

$(OUT): $(SRC)
	g++ $(SRC) $(LDFLAGS) $(CFLAGS)  -o $@

clean:
	@rm -f $(PROGS) $(OBJS) $(TEMPS) $(OUT)
	@echo "Limpo!"
