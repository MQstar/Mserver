src = $(wildcard src/*.cc)  
obj = $(patsubst %.cc, %.o, $(src))  
#obj = $(src:%.c=%.o)  
target = app 
CC=g++
CFLAGS = -I include -std=c++11
$(target): $(obj)
	$(CC) $(CFLAGS) $(obj) -o $(target)  
%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@  

.PHONY:clean
clean:
	rm -rf $(obj) $(target)
