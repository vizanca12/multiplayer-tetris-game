#include "player.hpp"

Player::Player(int socket)
{
    this->socket = socket;
    set_alive(true);
}

int Player::get_socket()
{
    return socket;
}

int Player::get_watching()
{
    return i_watching;
}

int Player::get_watched_by()
{
    return i_watched_by;
}

void Player::set_watched_by(int i)
{
    i_watched_by = i;
}

void Player::set_watching(int i)
{
    i_watching = i;
}

void Player::set_alive(bool a)
{
    alive = a;
}

bool Player::is_alive()
{
    return alive;
}

void Player::set_id(int id)
{
    this->id = id;
}

int Player::get_id()
{
    return id;
}
