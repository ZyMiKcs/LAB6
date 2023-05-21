all :
	gcc ./src/client.c ./include/clientfunc.c -o build/client
	gcc ./src/server.c ./include/serverfunc.c -o build/server

clean :
	rm build/client build/server

rebuild : clean all