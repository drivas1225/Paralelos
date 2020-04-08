#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <map>
#include <iterator>
#include <vector>
#include <string>

using namespace std;

multimap <string, string> gdb;
multimap <string, string> gdb_value;

void imprimir(){

    std::multimap<string,string>::iterator it;
    cout << "-------------------- relaciones de la base de datos -----------------"<<endl;
    for (it=gdb.begin(); it!=gdb.end(); ++it)
    std::cout << (*it).first << " => " << (*it).second << '\n';
}


///Funciones para la base de datos

int hashToDB(string Nodo){
    int suma = 0;
    int ascChar;
    for (int i = 0; i < Nodo.length(); i++) 
    {
        ascChar = Nodo[i];
        suma = suma + ascChar;
    }
    return suma % 4;
}

bool insertarNodo (string Nodo , string Value){
    
    std::multimap<string,string>::iterator itr = gdb_value.find(Nodo);

    if ( itr->first == Nodo){
        return false;
    }
    
    gdb_value.insert( pair<string , string> ( Nodo , Value));
    return true;
}

bool instertarRelacion(string Nodo1, string Nodo2){
    std::multimap<string,string>::iterator itr = gdb_value.find(Nodo1);

    if ( itr->first == Nodo1){
        gdb.insert( pair<string , string> ( Nodo1 , Nodo2));
        return true;
    }
    return false;
}

bool update(string Nodo , string Valor){
    std::multimap<string,string>::iterator itr = gdb_value.find(Nodo);
    if  ( itr->first == Nodo ){
        itr->second = Valor;
        return true;
    }
    return false;
}

bool borrarNodo( string Nodo){
    
    std::multimap<string,string>::iterator it = gdb.find(Nodo);
    gdb.erase ( it, gdb.end() );

    std::multimap<string,string>::iterator it2 = gdb_value.find(Nodo);
    gdb_value.erase ( it2, gdb_value.end() );

    // for (it = gdb.begin(); it != gdb.end(); ++it) 
    // { 
    //     if(it->first == Nodo || it->second == Nodo){
    //         gdb.erase(it);
    //         it--;
    //     } 
    // }



    return true;
}

bool borrarRelacion( string Nodo1 , string Nodo2 ){
    
    std::pair <std::multimap<string,string>::iterator, std::multimap<string,string>::iterator> ret;
    ret = gdb.equal_range(Nodo1);
    
    for (std::multimap <string,string>::iterator it=ret.first; it!=ret.second; ++it){
        if (it->second == Nodo2){
            gdb.erase(it);
        }
    }

    std::pair <std::multimap<string,string>::iterator, std::multimap<string,string>::iterator> ret2;
    ret2 = gdb.equal_range(Nodo2);
    
    for (std::multimap <string,string>::iterator it=ret2.first; it!=ret2.second; ++it){
        if (it->second == Nodo1){
            gdb.erase(it);
        }
    }
    
    return true;

}

string queryNodo(string Nodo){
    std::multimap<string,string>::iterator itr = gdb_value.find(Nodo);

    if ( itr->first == Nodo){
        return itr->second;
    }
    return "";
}

vector<string> queryRelation(string Nodo){
    
    vector<string> solu;
    std::pair <std::multimap<string,string>::iterator, std::multimap<string,string>::iterator> ret;
    ret = gdb.equal_range(Nodo);
    
    for (std::multimap <string,string>::iterator it=ret.first; it!=ret.second; ++it){
        solu.push_back(it->second);
    }

    return solu;

}


/////
//Check the size of a string
string read_size(string text)
{
	char buffer[3];
	sprintf(buffer,"%03d",(unsigned int)text.size());
	return buffer;
}

string read_sizeI(int val){

    string text = to_string(val);
    char buffer[3];
	sprintf(buffer,"%03d",(unsigned int)text.size());
	return buffer;
}

void processClient_thread(int socketClient)
{
    
    cout<<"ingreso un clinete "<<socketClient<<endl;
    
    char buffer[1000];
    string message;
    string messages;

    int size_name1;
    int size_name2;
    int size_value1;
    int size_depth;
    
    string name1;
    string name2;
    string value1;
    int depth;

    
    int size_nickname;
    int size_message;
    int socketTo;
    int n;
 	for(;;)
 	{
 		buffer[0] = 'X';
 		n = read(socketClient, buffer, 2);
        
 		if(n==0)
 		{
 			printf("Client [%d] say bye\n",socketClient);
            break;
 		}
        printf("Client: new comando \n");
        printf("Client: [%s]\n",buffer);

 		if(buffer[0] == 'M' && buffer[1] == 'S' ) //Mensaje a otro cliente
		{
			message = "WW";
			message = message + read_size("prueba") + "prueba" ;//read_size(nick(socketClient)) + nick(socketClient);
			
            read(socketClient,buffer,3); //reading header with size of from
            printf("Client: [%s]\n",buffer);
			size_nickname = atoi(buffer);
			bzero(buffer,3); //cleaning the buffer of the header-size
			
            read(socketClient,buffer,size_nickname); //read the right size of from
			printf("Client: [%s]\n",buffer);
            bzero(buffer,size_nickname);
			//message
			read(socketClient,buffer,3); //reading header with size of message
			printf("Client: [%s]\n",buffer);
            size_message = atoi(buffer);
			bzero(buffer,3); //cleaning the buffer of the header-size
			
            read(socketClient,buffer,size_message); //read the right size of mesage
			printf("Client: [%s]\n",buffer);
            message = message + read_size(buffer) + buffer;
            bzero(buffer,size_message);
			
            write(socketClient, message.c_str(), message.size());
			write(socketClient,"AA20",4); //acknowledgment
		}

        else if (buffer[0] == 'I' && buffer[1] == 'N')
        {
            //tam1
            read(socketClient,buffer,3);
            printf("Client: [%s]\n",buffer);
			size_name1 = atoi(buffer);
			bzero(buffer,3); 
			
            //name1
            read(socketClient,buffer,size_name1); 
			printf("Client: [%s]\n",buffer);
            name1 = buffer;
            bzero(buffer,size_name1);
			
            //tam2
			read(socketClient,buffer,3); 
			printf("Client: [%s]\n",buffer);
            size_value1 = atoi(buffer);
			bzero(buffer,3); 
			
            //value1
            read(socketClient,buffer,size_value1); 
			printf("Client: [%s]\n",buffer);
            value1 = buffer;
            bzero(buffer,size_value1);

            if ( insertarNodo(name1,value1) == true ){

                write(socketClient,"AA20",4); //acknowledgment
            }else{

                write(socketClient,"EE10",4); //Error el dato ya esta insertado
            }
            
        }

        else if (buffer[0] == 'I' && buffer[1] == 'R')
        {
            //tam1
            read(socketClient,buffer,3);
            printf("Client: [%s]\n",buffer);
			size_name1 = atoi(buffer);
			bzero(buffer,3); 
			
            //name1
            read(socketClient,buffer,size_name1); 
			printf("Client: [%s]\n",buffer);
            name1 = buffer;
            bzero(buffer,size_name1);
			
            //tam2
			read(socketClient,buffer,3); 
			printf("Client: [%s]\n",buffer);
            size_name2 = atoi(buffer);
			bzero(buffer,3); 
			
            //name2
            read(socketClient,buffer,size_name2); 
			printf("Client: [%s]\n",buffer);
            name2 = buffer;
            bzero(buffer,size_name2);

            if (instertarRelacion(name1,name2)== true){
                write(socketClient,"AA20",4); //acknowledgment
            }else{
                write(socketClient,"EE10",4); //Error el dato ya esta insertado
            }
            
        }

        else if (buffer[0] == 'U' && buffer[1] == 'P')
        {
            //tam1
            read(socketClient,buffer,3);
            printf("Client: [%s]\n",buffer);
			size_name1 = atoi(buffer);
			bzero(buffer,3); 
			
            //name1
            read(socketClient,buffer,size_name1); 
			printf("Client: [%s]\n",buffer);
            name1 = buffer;
            bzero(buffer,size_name1);
			
            //tam2
			read(socketClient,buffer,3); 
			printf("Client: [%s]\n",buffer);
            size_value1 = atoi(buffer);
			bzero(buffer,3); 
			
            //value
            read(socketClient,buffer,size_value1); 
			printf("Client: [%s]\n",buffer);
            value1 = buffer;
            bzero(buffer,size_value1);

            if(update(name1,value1)== true){
                write(socketClient,"AA20",4); //acknowledgment
            }else{
                 write(socketClient,"EE30",4); //Error el update
            }
            
            
        }

        else if (buffer[0] == 'D' && buffer[1] == 'N')
        {
            //tam1
            read(socketClient,buffer,3);
            printf("Client: [%s]\n",buffer);
			size_name1 = atoi(buffer);
			bzero(buffer,3); 
			
            //name1
            read(socketClient,buffer,size_name1); 
			printf("Client: [%s]\n",buffer);
            name1 = buffer;
            bzero(buffer,size_name1);
			
            vector<string> ret = queryRelation(name1);
            
            if(ret.size() > 0){
                message = "RD" + read_sizeI( ret.size() ) + to_string(ret.size()) ;
                for(int i = 0 ; i <ret.size() ; i++ ){
                    message = message + read_size(ret[i]) +ret[i];
                }
                write(socketClient, message.c_str(), message.size());
                write(socketClient,"AA20",4); //acknowledgment
            }else{
                write(socketClient,"EE40",4); //Error no hay vecinos
            }

            borrarNodo(name1);
            
            write(socketClient,"AA20",4); //acknowledgment
        }
        
        else if (buffer[0] == 'D' && buffer[1] == 'R')
        {
            //tam1
            read(socketClient,buffer,3);
            printf("Client: [%s]\n",buffer);
			size_name1 = atoi(buffer);
			bzero(buffer,3); 
			
            //name1
            read(socketClient,buffer,size_name1); 
			printf("Client: [%s]\n",buffer);
            name1 = buffer;
            bzero(buffer,size_name1);
			
            //tam2
			read(socketClient,buffer,3); 
			printf("Client: [%s]\n",buffer);
            size_name2 = atoi(buffer);
			bzero(buffer,3); 
			
            //name2
            read(socketClient,buffer,size_name2); 
			printf("Client: [%s]\n",buffer);
            name2 = buffer;
            bzero(buffer,size_name2);

            borrarRelacion(name1,name2);
            
            write(socketClient,"AA20",4); //acknowledgment
        }

        else if (buffer[0] == 'Q' && buffer[1] == 'N')
        {
            //tam1
            read(socketClient,buffer,3);
            printf("Client: [%s]\n",buffer);
			size_name1 = atoi(buffer);
			bzero(buffer,3); 
			
            //name1
            read(socketClient,buffer,size_name1); 
			printf("Client: [%s]\n",buffer);
            name1 = buffer;
            bzero(buffer,size_name1);
			
            string ret = queryNodo(name1);
            if (ret != ""){
                message = "RV" + read_size(ret) + ret;
                write(socketClient, message.c_str(), message.size());
                write(socketClient,"AA20",4); //acknowledgment
            }else{
                write(socketClient,"EE40",4); //Error el query value
                
            }

        }

        else if (buffer[0] == 'Q' && buffer[1] == 'R')
        {
            //tam1
            read(socketClient,buffer,3);
            printf("Client: [%s]\n",buffer);
			size_name1 = atoi(buffer);
			bzero(buffer,3); 
			
            //name1
            read(socketClient,buffer,size_name1); 
			printf("Client: [%s]\n",buffer);
            name1 = buffer;
            bzero(buffer,size_name1);
			
            //tam2
			read(socketClient,buffer,3); 
			printf("Client: [%s]\n",buffer);
            size_depth = atoi(buffer);
			bzero(buffer,3); 
			
            //depth
            read(socketClient,buffer,size_depth); 
			printf("Client: [%s]\n",buffer);
            depth = atoi(buffer);
            bzero(buffer,size_depth);

            vector<string> ret = queryRelation(name1);
            
            if(ret.size() > 0){
                message = "RR" + read_sizeI( ret.size() ) + to_string(ret.size()) ;
                for(int i = 0 ; i <ret.size() ; i++ ){
                    message = message + read_size(ret[i]) +ret[i];
                }
                write(socketClient, message.c_str(), message.size());
                write(socketClient,"AA20",4); //acknowledgment
            }else{
                write(socketClient,"EE40",4); //Error no hay vecinos
            }
			
        }
        

        else
        {
            n = read(socketClient ,buffer,255);

            if (n < 0) perror("ERROR reading from socket");
            printf("Client error de comando : [%s]\n",buffer);
            bzero(buffer,255);
        }

        imprimir();
        
	}
}

int main(void)
{
    int puerto;
    cout << "ingrese su numero de puerto: ";
    cin>>puerto;
    cout <<"Creando Servidor..."<<endl;
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;
    string txtserv;

    if(-1 == SocketFD)
    {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(puerto);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

        
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("error bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if(-1 == listen(SocketFD, 10))
    {
        perror("error listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        
        int ClientFD = accept(SocketFD, NULL, NULL);
        if(0 > ClientFD)
		{
			perror("error accept failed");
			close(SocketFD);
			exit(EXIT_FAILURE);
		}
		else
		{
            thread(processClient_thread,ClientFD).detach();
		}
    
    }

    close(SocketFD);
    return 0;
}