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
    string ip = "127.0.0.1";
    string port = "";
    int portval = 4390;

    /****************************
    CREATE
    *****************************/

    cout << "Client start" << endl;
    //Demande des infos de connexion
    cout << "Entrez IP : ";
    getline(cin, ip);
    cout << "Entrez port : ";
    getline(cin, port);

    stringstream buffer(port);
    buffer >> portval;

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
