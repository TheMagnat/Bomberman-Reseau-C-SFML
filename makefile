CC=g++
SRC=main.cpp SoundHandler.cpp Carte.cpp Case.cpp menu.cpp partie.cpp loby.cpp Log.cpp multiplayer.cpp Protocole.cpp StringToVec.cpp Player.cpp ProtocoleServer.cpp Bomb.cpp Bonus.cpp TempoMessage.cpp OptionChoice.cpp
MACRO=macro.h
OBJ=$(SRC:.cpp=.o)
TARGET=bombermanGm
CFLAGS=-O3 -Wall
LIBRARY=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -pthread -lstdc++fs

main: $(OBJ) 
	$(CC) -o $(TARGET) $(OBJ) $(LIBRARY)

%.o:	%.cpp %.hpp $(MACRO)
	$(CC) -c $< $(CFLAGS)

%.o:	%.cpp %.h $(MACRO)
	$(CC) -c $< $(CFLAGS)

%.o:	%.cpp $(MACRO)
	$(CC) -c $< $(CFLAGS)

clean:
	rm *.o

cleanall:
	rm *.o $(TARGET)