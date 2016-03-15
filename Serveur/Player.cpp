#include "Player.h"

Player::Player(sf::TcpSocket* socket, string pseudo)
{
    m_socket = socket;
    m_pseudo = pseudo;
    m_score = 0;
}

Player::~Player()
{
    delete m_socket;
}
