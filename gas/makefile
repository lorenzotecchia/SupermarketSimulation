run_master: compile_master 
	./master


compile_master:
	clang master.c cliente.c -o master


compile_server:
	clang cassa.c cassiere.c queue.c cliente.c server.c -o prova


run_server: compile_server
	./prova

clean:
	rm master prova
