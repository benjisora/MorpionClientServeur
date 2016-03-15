#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <random>
#include <thread>
#include <sstream>

using namespace std;

int main()
{
    sf::TcpSocket socket;
    string ip = "172.31.247.41";
    string portString = "";
    int port = 80;
    int code=-1;;
    int idCLient=-1;
    sf::Packet data;
    string sizeGridString = "";
    int sizeGrid = -1;
    string nbPionString = "";
    int nbPion = -1;
    string ipPlayerString = "127.0.0.1";

    /****************************
    CREATE
    *****************************/

    cout << "Client start" << endl;

    //Demande des infos de connexion
    /*
    cout << "Entrez IP : ";
    getline(cin, ip);
    cout << "Entrez port : ";
    getline(cin, portString);
    stringstream buffer(portString);
    buffer >> port;
    cout << "Entrez port : ";
    getline(cin, portString);
    stringstream buffer(portString);
    buffer >> port;
    */

    sf::Socket::Status status = socket.connect(ip, port);
    if (status != sf::Socket::Done)
        cout << "Impossible de se connecter" << endl;
    else
        cout << "connexion etablie avec le serveur distant" << endl << endl;


    if (socket.receive(data) != sf::Socket::Done)
        cout << "Erreur receive" << endl;
    data >> code;
    data.clear();

    if(code==101)
    {
        idCLient = code-100;
        cout << "Vous etes les createurs de la partie." << endl;
        cout << "Veuillez renseigner les options suivantes : " << endl;
        cout << "Entrez taille de la grille : ";
        getline(cin, sizeGridString);
        stringstream buffer1(sizeGridString);
        buffer1 >> sizeGrid;
        cout << "Entrez nombre de pions a aligner : ";
        getline(cin, nbPionString);
        stringstream buffer2(nbPionString);
        buffer2 >> nbPion;

        data << sizeGrid << ipPlayerString << nbPion;
        if (socket.send(data) != sf::Socket::Done)
            std::cout << "Impossible d'envoyer les donnees au serveur" << std::endl;
        data.clear();
    }
    else if(code>101 && code<110)
    {
        idCLient = code-100;
        cout << "...En attente d'adversaire..." << endl;
    }




     /*if (socket.receive(data) != sf::Socket::Done)
        cout << "Erreur receive" << endl;
    data >> code;*/
    data.clear();


    //if(code == )
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        /****************************
        STEP
        *****************************/

        window.clear();

        /****************************
        DRAW
        *****************************/

        window.display();
    }

    return 0;
}
