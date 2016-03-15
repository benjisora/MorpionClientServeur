#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include "make_unique.h"
#include <string>


using namespace std;

int main()
{
    sf::Packet data;
    int code = 0;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
    std::vector<string> pseudos;

    int idClient = 0;
    int tailleGrille = 0, nbAlign = 0;
    string client2IP = "";

    sf::TcpListener listener;
    listener.listen(80); //création du listener et écoute du port
    sf::SocketSelector selector;
    selector.add(listener); //Création du select, et ajout du listener au select

    while (true){
        // On attend un message d'une des sockets
        if (selector.wait()){
            // Test du listener
            if (selector.isReady(listener)){ // Il y a une connexion en attente

                unique_ptr<sf::TcpSocket> client = make_unique<sf::TcpSocket>();

                if (listener.accept(*client.get()) == sf::Socket::Done){

                    clients.push_back(std::move(client));

                    selector.add(*clients[clients.size()-1]); // Ajout du client au selecteur de telle sorte a ce qu'on puisse l'écouter

                    cout << "NOUVEAU CLIENT !! Nombre de clients: " << clients.size() <<endl;

                    sf::Packet packet;
                    if (clients[idClient]->receive(packet) != sf::Socket::Done){
                        cout << "echec de reception du pseudo" <<endl;
                    }
                    string pseudo;
                    packet >> code >> pseudo;

                    if(code == 0){
                        packet.clear();
                        pseudos.push_back(pseudo);
                        cout << pseudos[idClient]<<endl;
                    }
                    code = 0;
                    data << code;
                    if(clients[idClient]->send(data) != sf::Socket::Done){
                        cout << "echec d'envoi du code" <<endl;
                    }
                    data.clear();

                    idClient++;
                }
                else{
                    cout<<"connexion non acceptee"<<endl;
                }
            }
            else{ // La socket du listener n'est pas prete, on teste tous les autres sockets (clients)

                for (int i=0; i < clients.size();i++){
                        cout << "boucle for"<<endl;
                    if (selector.isReady(*clients[i].get())){
                        // Le client a fait quelque chose dans la socket
                        cout << "selectoready"<<endl;
                        sf::Packet packet;
                        if (clients[i]->receive(packet) != sf::Socket::Done){
                            cout <<"Reception d'un paquet du client"<<endl;
                        }
                        packet >> code >> tailleGrille >> client2IP >> nbAlign;
                        packet.clear();

                        if(code == 11){
                            //delete clients[i].get();

                            clients.erase(clients.begin()+i);
                            pseudos.erase(pseudos.begin()+i);
                            cout << "Client " << i << " est parti..." <<endl;
                            tailleGrille = 0;
                            client2IP = "";
                            nbAlign = 0;
                            //system("pause");
                        }

                    }
                }
            }
        }
    }




    return 0;
}
