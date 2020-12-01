#include "callback.h"

void Callback::BinaryPacket(const char* Data,std::size_t size)
{
    if(Data!= nullptr)
    {
        size = 0;
    }
    size++;
}
void Callback::TextPacket(const char* Data,std::size_t size)
{
    if(Data!= nullptr)
    {
        size = 0;
    }
    size++;
}