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
    int idClient = 0;
    int tailleGrille = 0, nbAlign = 0;
    string client2IP = "";

    sf::TcpListener listener;
    listener.listen(80); //création et écoute du port

    std::vector<std::unique_ptr<sf::TcpSocket>> clients;

    sf::SocketSelector selector;
    selector.add(listener); //création


    while (true){
        // On attend un message d'une des sockets
        if (selector.wait()){
            // Test du listener
            if (selector.isReady(listener)){ // Il y a une connexion en attente

                unique_ptr<sf::TcpSocket> client = make_unique<sf::TcpSocket>();

                if (listener.accept(*client.get()) == sf::Socket::Done){

                    clients.push_back(std::move(client));

                    selector.add(*clients[clients.size()-1]); // Ajout du client au selecteur de telle sorte a ce qu'on puisse l'écouter

                    cout << "NOUVEAU CLIENT !! nombre de clients: " << clients.size() <<endl;

                    data << 101 + idClient;
                    cout << 101 + idClient << endl;
                    if(clients[idClient]->send(data) != sf::Socket::Done)
                        cout<< "pas envoyé ): " << endl;
                    data.clear();

                    idClient++;
                }
                else{
                    cout<<"connexion non acceptée"<<endl;
                }
            }
            else{ // La socket du listener n'est pas prete, on teste tous les autres sockets (clients)

                for (int i=0; i < clients.size();i++){

                    if (selector.isReady(*clients[i].get())){
                        // Le client a fait quelque chose, on reçoit

                        sf::Packet packet;
                        if (clients[i]->receive(packet) != sf::Socket::Done)
                            cout <<"on a un truc!!!!"<<endl;

                        packet >> tailleGrille >> client2IP >> nbAlign;
                        packet.clear();
                        cout << tailleGrille <<  " " << client2IP << " " << nbAlign << endl;
                    }
                }
            }






        }
    }




    return 0;
}
