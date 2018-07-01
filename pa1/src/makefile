CFLAGS = -O3 -DNDEBUG -Wall
#CFLAGS = -g -Wall
files = FMParse.o FMPartition.o main.o Cell.o

fm: $(files)
	@echo "> linking..."
	@g++ $(CFLAGS) $(files) -o  $@
	

-include $(files:.o=.d)
%.o: %.cpp
	@echo "> compiling "$<
	@g++ $(CFLAGS) $< -c -MMD

clean:
	rm -f *.o *.d fm
	
