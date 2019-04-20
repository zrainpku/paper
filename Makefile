CPP = g++

TARGET = Hash

OBJS =  frangi.o \
		mytools.o \
	   	rgb2rbx.o \
	   	hash.o \
	   	wavelet.o \
		main.o
$(TARGET) : $(OBJS)
	$(CPP) -o $(TARGET)  $(OBJS)
frangi.o :frangi.cpp
	$(CPP)  -c $>
rgb2rbx.o :rgb2rbx.cpp
	$(CPP)  -c $>
wavelet.o :wavelet.cpp
	$(CPP)  -c $>
mytools.o :mytools.cpp frangi.o
	$(CPP)  -c $>
hash.o : hash.cpp mytools.o wavelet.o rgb2rbx.o
	$(CPP) -c $>
main.o : main.cpp hash.o
	$(CPP) -c $>

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
