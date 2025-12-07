#include "room.hpp"

Room::Room(int size)
{
    m_size = size;
}

bool Room::is_filled()
{
    return players.size() == m_size;
}

bool Room::is_empty()
{
    return players.size() == 0;
}

void Room::add(Player *player)
{
    players.push_back(player);
}

void Room::remove(Player *rm_player)
{
    for (auto it = players.begin(); it != players.end(); it++)
    {
        Player *player = *it;
        if (player->get_socket() == rm_player->get_socket())
        {
            players.erase(it);
            break;
        }
    }
}

int Room::current_size()
{
    return players.size();
}

int Room::max_size()
{
    return m_size;
}

void Room::get_frac_filled(char *frac)
{
    frac[0] = players.size() + '0';
    frac[1] = '/';
    frac[2] = m_size + '0';
}

Player *Room::get_player(int i)
{
    i = i % players.size();
    if (i < 0)
        i = i + players.size();
    return players.at(i);
}