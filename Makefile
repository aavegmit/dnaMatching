all: hasher router indexer

hasher: hasher.o hasher_sender.o fileIO.o
	g++ -o hasher hasher.o hasher_sender.o fileIO.o -D_REETRANT -lpthread

hasher.o: hasher.cc
	g++ -c -g hasher.cc -D_REETRANT -lpthread 

hasher_sender.o: hasher_sender.cc
	g++ -c -g hasher_sender.cc -D_REETRANT -lpthread 

router: router.o
	g++ -o router router.o -D_REETRANT -lpthread 

router.o: router.cc
	g++ -c -g router.cc -D_REETRANT -lpthread 

indexer: indexer_reader.o indexer_parser.o
	g++ -o indexer indexer_reader.o indexer_parser.o -D_REETRANT -lpthread 

indexer_reader.o: indexer_reader.cc
	g++ -c -g indexer_reader.cc -D_REETRANT -lpthread 

indexer_parser.o: indexer_parser.cc
	g++ -c -g indexer_parser.cc -D_REETRANT -lpthread 

fileIO: fileIO.o
	g++ -o fileIO fileIO.o -D_REETRANT -lpthread -lpcap

fileIO.o: fileIO.cc
	g++ -c -g fileIO.cc -D_REETRANT -lpthread -lpcap

clean:
	rm -rf *.o hasher router indexer
