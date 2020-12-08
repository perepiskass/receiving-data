#include "callback.h"
#define UNUSED(expr) (void)(expr)

void Callback::BinaryPacket(const char* Data,std::size_t size)
{
    UNUSED(Data);
    UNUSED(size);
}
void Callback::TextPacket(const char* Data, std::size_t size)
{
    UNUSED(Data);
    UNUSED(size);
}