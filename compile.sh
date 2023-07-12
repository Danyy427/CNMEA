gcc src/NMEAReceive.c -I inc -c -o bin/NMEAReceive.o
gcc src/NMEAReceive.c -I inc -c -o bin/NMEATransmit.o
gcc src/NMEAReceive.c -I inc -c -o bin/NMEUtils.o
ar rcs bin/libCNEMA.a bin/NMEAReceive.o bin/NMEATransmit.o bin/NMEUtils.o