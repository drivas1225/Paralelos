#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


using namespace std;

vector<int> servidores ;

vector<string> para_borrar;

int createClient(std::string ip="127.0.0.1", int port=0){
	
	struct sockaddr_in stSockAddr;
    int SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    stSockAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    int n=connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in));
    if(n<0) return -1;
	return SocketFD;
}

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


//Check the size of a string 
string read_size(string text)
{
	char buffer[3];
	sprintf(buffer,"%03d",(unsigned int)text.size());
	return buffer;
}

void showComandos(){
    cout<<"---------- Comandos ------------"<<endl;
	//cout<<"msg: send msg: "<<endl;
    cout<<"IN: Insertar Nodo: "<<endl;
    cout<<"IR: Insertar Relacion "<<endl;
    cout<<"UP: Update "<<endl;
    cout<<"DN: Borrar Nodo "<<endl;
    cout<<"DR: Borrar Relacion "<<endl;
    cout<<"QN: Consulta Nodo"<<endl;
    cout<<"QR: Consulta Relacion"<<endl;
    cout<<"help:  MUestra los comandos"<<endl;
    cout<<"CS: Conectar servidor"<<endl;

}

/*funcion de busqueda */
vector<string> busqueda(int my_socket, string bus ){
    
    char buffer[100];
    int size;
    vector <string> ret;
    string txt;
    string name1;
    string depth;
    int n;


    name1 = bus;
    depth = 1;

    txt = "QR" + read_size(name1) + name1 + read_size(depth) + depth ;
    my_socket = servidores[ hashToDB(name1) ];
    write(my_socket,txt.c_str(),txt.length());

    n = read(my_socket, buffer ,2);

    if (buffer[0]=='E',buffer[1]=='E'){
        return ret;
    }else{

        bzero(buffer,2); 
        ///tamaño de nuemro de vecinos
        read(my_socket,buffer,3); 
        printf(" server: [%s]\n",buffer);
        size = atoi(buffer);
        bzero(buffer,3); 

        ///numero de vecinos
        read(my_socket,buffer,size); 
        printf(" Rserver: [%s]\n",buffer);
        
        int numVecinos = atoi(buffer);
        bzero(buffer,size);

        
        int tem_tam;
        string tem_dato;


        for(int i = 0 ; i < numVecinos ; i++){
            
            
            ///tamaño de nuemro de vecinos
            read(my_socket,buffer,3); 
            printf(" server: [%s]\n",buffer);
            tem_tam = atoi(buffer);
            bzero(buffer,3); 

            ///numero de vecinos
            read(my_socket,buffer,tem_tam); 
            printf(" Rserver: [%s]\n",buffer);
            bzero(buffer,tem_tam);

            ret.push_back(buffer);
        } 
    }
    return ret;
}


template <class Container>
void split1(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(cont));
}




void writeC( ){
    int my_socket;
    string comando;
	string txt;
	string to;
	string message;	
    string name1;
    string name2;
    string value1;
    string depth;

    std::vector<std::string> words;
    
	//do{
        getline(cin, comando);
		//cin>>comando;
        split1(comando,words);

		if (words[0] == "I" && words[1] == "node")
        {
            // cout<< "Nombre del nodo: ";
            // cin >> name1;
            // cout<< "Valor del nodo: ";
            // cin >> value1;
            name1 = words[2];
            value1 = words[3];

            txt = "IN" + read_size(name1) + name1 + read_size(value1) + value1;
            //cout<<txt;
            my_socket = servidores[ hashToDB(name1) ];
            write(my_socket,txt.c_str(),txt.length());

            
        }

        else if (words[0] == "I" && words[1] == "R")
        {
            // cout<< "Nombre del nodo 1: ";
            // cin >> name1;
            // cout<< "Nombre del nodo 2: ";
            // cin >> name2;

            name1 = words[2];
            name2 = words[3];

            txt = "IR" + read_size(name1) + name1 + read_size(name2) + name2;
            my_socket = servidores[ hashToDB(name1) ];
            write(my_socket,txt.c_str(),txt.length());
            
            txt = "IR" + read_size(name2) + name2 + read_size(name1) + name1;
            my_socket = servidores[ hashToDB(name2) ];
            write(my_socket,txt.c_str(),txt.length());
        }

        else if (words[0] == "U" && words[1] == "node")
        {
            // cout<< "Nombre del nodo: ";
            // cin >> name1;
            // cout<< "Nuevo valor: ";
            // cin >> value1;

            name1 = words[2];
            value1 = words[3];

            txt = "UP" + read_size(name1) + name1 + read_size(value1) + value1;
            my_socket = servidores[ hashToDB(name1) ];
            write(my_socket,txt.c_str(),txt.length());
        }

        else if (words[0] == "D" && words[1] == "node")
        {
            // cout<< "Nombre del nodo: ";
            // cin >> name1;
            
            name1 = words[2];
            txt = "DN" + read_size(name1) + name1 ;
            my_socket = servidores[ hashToDB(name1) ];
            
            write(my_socket,txt.c_str(),txt.length());

            for (int i = 0 ; i < para_borrar.size() ; i ++ ){
                string txt2 = "DR" + read_size(para_borrar[i]) + para_borrar[i] + read_size(name1) + name1;
                write( servidores[ hashToDB(para_borrar[i]) ] ,txt.c_str(),txt.length() );
            }
        }

        else if (words[0] == "D" && words[1] == "R")
        {
            // cout<< "Nombre del nodo 1: ";
            // cin >> name1;
            // cout<< "Nombre del nodo 2: ";
            // cin >> name2;

            name1 = words[2];
            name2 = words[3];

            txt = "DR" + read_size(name1) + name1 + read_size(name2) + name2;
            my_socket = servidores[ hashToDB(name1) ];
            write(my_socket,txt.c_str(),txt.length());

            txt = "DR" + read_size(name2) + name2 + read_size(name1) + name1;
            my_socket = servidores[ hashToDB(name2) ];
            write(my_socket,txt.c_str(),txt.length());
        }

        else if (words[0] == "Q" && words[1] == "node")
        {


            name1 = words[2];

            txt = "QN" + read_size(name1) + name1;
            my_socket = servidores[ hashToDB(name1) ];
            write(my_socket,txt.c_str(),txt.length());
        }

        else if (words[0] == "Q" && words[1] == "R")
        {

            name1 = words[2];
            depth = words[3];

            txt = "QR" + read_size(name1) + name1 + read_size(depth) + depth ;
            my_socket = servidores[ hashToDB(name1) ];
            write(my_socket,txt.c_str(),txt.length());
        }
    
		else if (comando == "help")
        {
            showComandos();
        }
        else if (comando == "CS" )
        {
            int pueto;
            cout<<"Ingrese Puerto del Servidor: ";
            cin>>pueto;
            int temFD = createClient("127.0.0.1", pueto);
            servidores.push_back(temFD);
        }
        

        else
        {
            cout<<"comando no reconocido";
        }
        
        		
	//}while( true );
}




//Function for threads that reads The reply of the socket
void readC(int my_socket)
{
	int size;
	char buffer[1000];
	string type;
	int n;


	do{
		n = read(my_socket, buffer ,2); //read just the first byte to check the message's type
        if(n==0)
		{
			printf("Servidor [%d] say bye\n",my_socket);
            break;
		}
        printf("---------- new comando ----------- \n");
        printf("server Comando: [%s]\n",buffer);
		
        if ( buffer[0]=='R' && buffer[1]=='V')
        {
            bzero(buffer,2); 
			
            read(my_socket,buffer,3); 
			printf(" server: [%s]\n",buffer);
            size = atoi(buffer);
			bzero(buffer,3); 
			
            read(my_socket,buffer,size); //read the right size of from		
			printf(" server: [%s]\n",buffer);
            //cout<<"Mensaje de "<< buffer << ": ";
			bzero(buffer,size); //cleaning the buffer of the header-command
			
        }

        else if ( buffer[0]=='R' && buffer[1]=='R')
        {
            bzero(buffer,2); 
			///tamaño de nuemro de vecinos
            read(my_socket,buffer,3); 
			printf(" server: [%s]\n",buffer);
            size = atoi(buffer);
			bzero(buffer,3); 

			///numero de vecinos
            read(my_socket,buffer,size); 
			printf(" Rserver: [%s]\n",buffer);
			
            int numVecinos = atoi(buffer);
            bzero(buffer,size);

            
            int tem_tam;
            string tem_dato;


            for(int i = 0 ; i < numVecinos ; i++){
                
                
                ///tamaño de nuemro de vecinos
                read(my_socket,buffer,3); 
                printf(" server: [%s]\n",buffer);
                tem_tam = atoi(buffer);
                bzero(buffer,3); 

                ///numero de vecinos
                read(my_socket,buffer,tem_tam); 
                printf(" Rserver: [%s]\n",buffer);
                bzero(buffer,tem_tam);
            } 
			
        }

        else if ( buffer[0]=='R' && buffer[1]=='D')
        {
            bzero(buffer,2); 
			///tamaño de nuemro de vecinos
            read(my_socket,buffer,3); 
			printf(" server: [%s]\n",buffer);
            size = atoi(buffer);
			bzero(buffer,3); 

			///numero de vecinos
            read(my_socket,buffer,size); 
			printf(" Rserver: [%s]\n",buffer);
			
            int numVecinos = atoi(buffer);
            bzero(buffer,size);

            
            int tem_tam;
            string tem_dato;


            for(int i = 0 ; i < numVecinos ; i++){
                
                
                ///tamaño de nuemro de vecinos
                read(my_socket,buffer,3); 
                printf(" server: [%s]\n",buffer);
                tem_tam = atoi(buffer);
                bzero(buffer,3); 

                ///numero de vecinos
                read(my_socket,buffer,tem_tam); 
                printf(" Rserver: [%s]\n",buffer);
                bzero(buffer,tem_tam);

                para_borrar.push_back(buffer);
            } 
			
        }
        

        else if (buffer[0]=='A' && buffer[1]=='A') //acknowledgments
		{
			bzero(buffer,2); 
			read(my_socket,buffer,2);
			if(strncmp(buffer,"20", 2)==0)
			{
				cout<<"Operacion echa con exito"<<endl;
			}
			bzero(buffer,2); 
		}


		
		else if (buffer[0]=='E',buffer[1]=='E') //errors
		{
			bzero(buffer,2); //cleaning the buffer of the header-command
			read(my_socket,buffer,2);
			if(strncmp(buffer,"10", 2)==0)
			{
				cout<<"ERROR dato ya insertado"<<endl;
			}
            if(strncmp(buffer,"20", 2)==0)
			{
				cout<<"Mesage ERROR"<<endl;
			}
            if(strncmp(buffer,"30", 2)==0)
			{
				cout<<"ERROR update"<<endl;
			}
            if(strncmp(buffer,"40", 2)==0)
			{
				cout<<"No tiene vecinos"<<endl;
			}
			bzero(buffer,2); //cleaning the buffer of the type of error

		}else
        {
            n = read(my_socket ,buffer,255);

            if (n < 0) perror("ERROR reading from socket");
            printf("Client error de comando : [%s]\n",buffer);
            bzero(buffer,255);
        }
	}while( strncmp(buffer,"A30", 3) != 0 );	
}

void gestor(  ){
    
    char buffer[1000];
    string txt;
    int ports[4] = {8081,8082,8083,8084};

    for(int i = 0 ; i < 4 ; i++){
        int sock_temp = createClient("127.0.0.1",ports[i]);
        servidores.push_back(sock_temp);
        //This is what I receive/read
	    
    }
    thread esclavo1 (readC, servidores[0]);
    thread esclavo2 (readC, servidores[1]);
    thread esclavo3 (readC, servidores[2]);
    thread esclavo4 (readC, servidores[3]);		
    
    esclavo1.detach();
    esclavo2.detach();
    esclavo3.detach();
    esclavo4.detach();

    do
    {
        thread write(writeC);
        write.join();

    }while (1);
    
    // shutdown(SocketFD, SHUT_RDWR);
    // close(SocketFD);
}

int main(void)
{
    
    cout<<"welcome"<<endl;
    
    gestor();

    return 0;
}