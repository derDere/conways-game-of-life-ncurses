SRC = $(wildcard *.cpp)
APP = "cgol"

All: $(APP)
	@echo Done

$(APP): $(SRC)
	g++ -o $(APP) $(SRC) -lncursesw

test: $(APP)
	./$(APP) debug

install: $(APP)
	cp $(APP) /usr/local/bin/$(APP)
