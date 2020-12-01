#include "receiver.h"
#include <algorithm>
#include <stdexcept>
#include <inttypes.h>

void Receiver::Receive(const char* data, std::size_t size)
{
    if(size && data != nullptr)
    {
        m_data.insert(m_data.end(), data, data + size);
        if(CheckHandler(m_data.front()))
            TextReceive(size);
        else
            BinReceive();                
    }
    else
        throw std::domain_error( "data is empty!\n" );    
}

bool Receiver::CheckHandler(char c) const
{
    if(c == 0x24)
        return false;      
    return true;
}

void Receiver::TextReceive(size_t size)
{
    static const std::vector<char> finish_bytes{'\r','\n','\r','\n'};

    auto it = std::search(m_data.begin() + (m_data.size() - size > 4 ? m_data.size() - size - 3  : 1 ),
     m_data.end(), finish_bytes.begin(),finish_bytes.end());

    if(it != m_data.end())
    {
        std::size_t lenght = it - std::begin(m_data);
        m_callback->TextPacket(m_data.data() + 1, lenght - 1);
        it += 4;
        if(it != m_data.end())
        {
            m_data.erase(std::copy(it, std::end(m_data), std::begin(m_data)), std::end(m_data));    //лишнее копирование, но если не урезать то может быть вектор огромен
            if(CheckHandler(m_data.front()))
                TextReceive(m_data.size());
            else
                BinReceive();
        }
        else
            m_data.clear();
    }
}

uint32_t Receiver::ToBigEndian(const std::vector<char>& vec) const
{
    uint32_t res = 0;
    for(size_t i = 0; i < 4; ++i)
    {
        uint32_t value = static_cast<unsigned char>(vec[i+1]);
        res = res | value << i*8;
    }
    return res;
}

// uint32_t Receiver::ToBig(const std::vector<char>& vec) const
// {

// }


void Receiver::BinReceive()
{
    if(m_data.size() > 4)
    {
        auto size = ToBigEndian(m_data);
        if(m_data.size() - 5 >= size)
        {
            m_callback->BinaryPacket(m_data.data() + 5, size);
            if(m_data.size() - 5 == size)
                m_data.clear();
            else
            {
                m_data.erase(std::copy(m_data.begin() + 5 + size, m_data.end(), m_data.begin()), std::end(m_data));    //лишнее копирование, но если не урезать то может быть вектор огромен
                if(CheckHandler(m_data.front()))
                    TextReceive(m_data.size());
                else
                    BinReceive();
            }
        }
    }
}
