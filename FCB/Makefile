

##----------------------------------------
## MakeFile
##-----------------------------------------
CC = gcc
CFLAGS = -g -Wall
LIBS = -lusb-1.0

all: visca

visca: visca.o camInfo.o camera.o lens.o expwb.o spot.o motionDetect.o iq.o extend.o
	$(CC) $(CFLAGS) -o visca visca.o camInfo.o camera.o lens.o expwb.o spot.o motionDetect.o iq.o extend.o $(LIBS)

visca.o: visca.c visca.h
	$(CC) $(CFLAGS) -c visca.c

camInfo.o: camInfo.c visca.h
	$(CC) $(CFLAGS) -c camInfo.c

camera.o: camera.c visca.h
	$(CC) $(CFLAGS) -c camera.c

lens.o: lens.c visca.h
	$(CC) $(CFLAGS) -c lens.c

expwb.o: expwb.c visca.h
	$(CC) $(CFLAGS) -c expwb.c


spot.o: spot.c visca.h
	$(CC) $(CFLAGS) -c spot.c
	
motionDetect.o: motionDetect.c visca.h
	$(CC) $(CFLAGS) -c motionDetect.c
	
iq.o: iq.c visca.h
	$(CC) $(CFLAGS) -c iq.c	
	
extend.o: extend.c visca.h
	$(CC) $(CFLAGS) -c extend.c
	
clean:
	rm -f visca visca.o camInfo.o camera.o lens.o expwb.o spot.o motionDetect.o iq.o extend.o
    
    
    
    
    
    
    
