all: hasher router indexer

hasher: hasher.o hasher_sender.o
	g++ -o hasher hasher.o hasher_sender.o -D_REETRANT -lpthread -lpcap

hasher.o: hasher.cc
	g++ -c -g hasher.cc -D_REETRANT -lpthread -lpcap

hasher_sender.o: hasher_sender.cc
	g++ -c -g hasher_sender.cc -D_REETRANT -lpthread -lpcap

router: router.o
	g++ -o router router.o -D_REETRANT -lpthread -lpcap

router.o: router.cc
	g++ -c -g router.cc -D_REETRANT -lpthread -lpcap

indexer: indexer_reader.o
	g++ -o indexer indexer_reader.o -D_REETRANT -lpthread -lpcap

indexer_reader.o: indexer_reader.cc
	g++ -c -g indexer_reader.cc -D_REETRANT -lpthread -lpcap

clean:
	rm -rf *.o hasher router indexer
