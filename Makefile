FLAGS=-Wall -g -lrt -pthread

APP_OUTPUT=./app.exe
SLAVE_OUTPUT=./esclavos.exe
VISUALIZER_OUTPUT=./visualizer.exe

all: app slave visualizer

app:
	gcc $(FLAGS) app.c utility.c -o $(APP_OUTPUT)
slave:
	gcc $(FLAGS) esclavos.c utility.c  -o $(SLAVE_OUTPUT)
visualizer:
	gcc $(FLAGS) visualizer.c utility.c  -o $(VISUALIZER_OUTPUT)
clean:
	rm -f $(APP_OUTPUT);rm -f $(SLAVE_OUTPUT);rm -f $(VISUALIZER_OUTPUT)

.PHONY: all slave visualizer clean