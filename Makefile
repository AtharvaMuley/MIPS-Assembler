CC = gcc
bin_programs = mips_assembler
OBJS = main.o

$(bin_programs): $(OBJS)
	$(CC) -o $@ $^
%.o: %.c
	$(CC) -c -o $@ $<
clean:
	rm -f  $(bin_programs) $(OBJS) *.txt