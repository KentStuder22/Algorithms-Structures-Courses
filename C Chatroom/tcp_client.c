/*Kent Studer - krspcp (12544417), CS 4850 Project V1, API Socket Chatroom
 * For this project I will be using straight C to code essentially an echo chamber chatroom between this client and the server
 * Programmed and Tested using Ubuntu on Windows 10*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 14417
#define MAX_LINE 256

int main(int argc, char * argv[]){
    //The first thing we will do is create our socket
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("Socket Error\n");
        return 1;
    }
    //connecting to the server via our specified IP address
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect our client sock to server
    int connect_err = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if(connect_err < 0){
	    printf("Err at connect\n");
	    return 0;
    }else{
	    printf("My chat room client. Version One.\n");
    }
   
    int command_flag = 0; //Changed to 1 if the user is logged in, stays at 0 if not 

    //Infinite loop to keep us in the server until user specifies logout, which we will then break from
    while(1){
	    char client_message[MAX_LINE];
	    char server_message[MAX_LINE];
	    char copy[MAX_LINE];

	    fgets(client_message, MAX_LINE, stdin); //taking input from user on command line
	    strcpy(copy, client_message);

	    char * command = strtok(client_message, " "); //the first word typed is our command, strtok gives us this

	    if(strcmp(command, "login") == 0){ //if command is login
		    if(command_flag == 1){ //check if we are already logged in, if so send message and receive message back and continue to next while loop iteration
			    send(sock, copy, MAX_LINE, 0);
			    recv(sock, server_message, MAX_LINE, 0);

			    puts(server_message);

			    continue;
		    }else{ //if we are not logged in
			    int word_count = 0;
		    	    for(int i = 0; copy[i] != '\0'; i++){
				    if(copy[i] == ' ' && copy[i+1] != ' '){
					    word_count++;
				    }
			    }
			    if(word_count > 2){ //verifies there are 2 words in the command line (not 3 because the 3 word is followed by a newline
				    continue;
			    }
			    else{ //if command is valid we send to server
				    send(sock, copy, MAX_LINE, 0);
				    recv(sock, server_message, MAX_LINE, 0);
				    
				    printf("%s\n", server_message);
				    
				    char * error = strtok(server_message, ".");
				    if(strcmp(error, "Denied") == 0){
					    continue;
				    }else{ //if our message from server is not denied we are logged in
					    command_flag = 1;
				    }
			    }
			    continue;
		    }
	    }
        else if(strcmp(command, "newuser") == 0){ //if command is newuser
		if(command_flag == 1){ //login check
			send(sock, copy, MAX_LINE, 0);
			recv(sock, server_message, MAX_LINE, 0);

			printf("%s\n", server_message);

                	continue;
            	}else{ //not logged in we verify username and password lengths
            		char username[32], password[8];
            		command = strtok(NULL, " ");
			strcpy(username, command);

			command = strtok(NULL, "\n");
			strcpy(password, command);
		
			//Validating lengths
			if(strlen(username) > 32 || strlen(username) < 1){
				printf("Please enter a valid username\n");
			}
			else if(strlen(password) > 8 || strlen(password) < 4){
				printf("Please enter a valid password\n");
			}
			else{ //if both the username and password lengths are valid we send the whole copy string to the server
                		send(sock, copy, MAX_LINE, 0);
				recv(sock, server_message, MAX_LINE, 0);

				printf("%s\n", server_message);
			}
			continue;
            	}
        }
        else if(strcmp(command, "send") == 0){ //if our command is send
            if(command_flag == 0){ //login check
		    send(sock, copy, MAX_LINE, 0);
		    recv(sock, server_message, MAX_LINE, 0);

		    printf("%s\n", server_message);

		    continue;
            }else{ //if logged in, we can send
		    send(sock, copy, MAX_LINE, 0);
		    recv(sock, server_message, MAX_LINE, 0);

		    printf("%s", server_message);

		    continue;
	    }
        }
        else if(strcmp(command, "logout\n") == 0){ //if our command is logout
		if(command_flag == 1){ //login check
			send(sock, copy, MAX_LINE, 0);
			recv(sock, server_message, MAX_LINE, 0);
			
			command_flag = 0;

			printf("%s\n", server_message);
			recv(sock, server_message, MAX_LINE, 0);

			close(sock);

			break;
		}else{ //if logged in, send message to server, set flag to 1 so we break out of program
			send(sock, copy, MAX_LINE, 0);
			recv(sock, server_message, MAX_LINE, 0);

			printf("%s\n", server_message);
		}
	}else{ //the command typed in does not match so we throw client error and continue
		printf("Please enter a valid command\n");
		continue;
	}

    }
    return 0;
}
