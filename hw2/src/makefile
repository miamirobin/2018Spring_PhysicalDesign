CFLAGS = -O3 -DNDEBUG -Wall
#CFLAGS = -g -Wall
files = FPParse.o Floorplan.o main.o Cell.o Net.o

Floorplan: $(files)
	@echo "> linking..."
	@g++ $(CFLAGS) $(files) -o  $@
	

-include $(files:.o=.d)
%.o: %.cpp
	@echo "> compiling "$<
	@g++ $(CFLAGS) $< -c -MMD

clean:
	rm -f *.o *.d Floorplan
	
