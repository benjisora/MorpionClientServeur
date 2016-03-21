#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include "make_unique.h"
#include <string>
#include <thread>


using namespace std;

void pseudocheck(std::vector<std::unique_ptr<sf::TcpSocket>> clients, unique_ptr<sf::TcpSocket> client, std::vector<string> pseudos, int idClient)
{

    boolean pseudoAdded = false;


    int code;
    string pseudo = "";
    sf::Packet packet;
    sf::Packet data;

    client->setBlocking(true);

    while(!pseudoAdded)
    {

        if (clients[idClient]->receive(packet) != sf::Socket::Done)
        {
            cout << "echec de reception du pseudo" <<endl;
        }
        packet >> code >> pseudo;
        packet.clear();

        if(idClient==0 && code == 0) //si il y a aucun pseudo
        {
            pseudoAdded = true;
            code = 101;
        }
        else if(idClient != 0 && code == 0) //si il y a déjà des pseudos
        {
            boolean pseudoGood = true;
            for (unsigned int i=0; i<pseudos.size(); i++)
            {
                if(pseudos[i]==pseudo)
                {
                    pseudoGood = false;
                    code = 1;
                    data << code;
                    if(clients[idClient]->send(data) != sf::Socket::Done)
                    {
                        cout << "echec d'envoi du code" <<endl;
                    }
                    data.clear();
                    break;
                }
            }
            if(pseudoGood)
            {
                pseudoAdded = true;
                code = 0;
            }
        }
    }

    pseudos.push_back(pseudo);
    data << code;
    if(clients[idClient]->send(data) != sf::Socket::Done)
    {
        cout << "echec d'envoi du code" <<endl;
    }
    data.clear();
}




int main()
{
    sf::Packet data;
    int code = 0;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
    std::vector<string> pseudos;

    int idClient = 0;
    //int tailleGrille = 0, nbAlign = 0;
    //string client2IP = "";

    sf::TcpListener listener;
    listener.listen(80); //création du listener et écoute du port
    sf::SocketSelector selector;
    selector.add(listener); //Création du select, et ajout du listener au select

    while (true)
    {
        // On attend un message d'une des sockets
        if (selector.wait())
        {
            // Test du listener
            if (selector.isReady(listener))  // Il y a une connexion en attente
            {
                unique_ptr<sf::TcpSocket> client = make_unique<sf::TcpSocket>();

                if (listener.accept(*client.get()) == sf::Socket::Done)
                {
                    clients.push_back(std::move(client));

                    selector.add(*clients[clients.size()-1]); // Ajout du client au selecteur de telle sorte a ce qu'on puisse l'écouter

                    cout << "NOUVEAU CLIENT !! Nombre de clients: " << clients.size() <<endl;

                    std::thread pseudoverify (pseudocheck, clients, client, pseudos, idClient); //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                    idClient++;

                }
                else
                {
                    cout<<"connexion non acceptee"<<endl;
                }
            }
            else  // La socket du listener n'est pas prete, on teste tous les autres sockets (clients)
            {
                int limit = clients.size();
                for (int i=0; i < limit; i++)
                {
                    if (selector.isReady(*clients[i].get()))
                    {
                        // Le client a fait quelque chose dans la socket

                        sf::Packet data2;

                        if (clients[i]->receive(data2) != sf::Socket::Disconnected)
                        {

                        }
                        else
                        {
                            cout << pseudos[i] << " est parti" << endl;
                            selector.remove(*clients[i].get());
                            clients.erase(clients.begin()+i);
                            pseudos.erase(pseudos.begin()+i);
                            i--;
                            limit--;
                            idClient--;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
