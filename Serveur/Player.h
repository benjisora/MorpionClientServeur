#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <string>
#include "make_unique.h"

using namespace std;

class Player
{
    public:
        Player(sf::TcpSocket* socket, string pseudo);
        virtual ~Player();

        sf::TcpSocket* getSocket(){
            return m_socket;
        }

        string getPseudo(){
            return m_pseudo;
        }

        void setSocket(sf::TcpSocket* socket){
            m_socket = socket;
        }

        void setSocket(string pseudo){
            m_pseudo = pseudo;
        }

        int getScore(){
            return m_score;
        }

        void setScore(int score){
            m_score = score;
        }

    protected:

    private:
        sf::TcpSocket* m_socket;
        string m_pseudo;
        int m_score;
};

#endif // PLAYER_H
