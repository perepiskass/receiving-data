#pragma once
#include <cstdlib>


/**
 * @brief Интерфейс для реализации обработки пакетов.
 */
class ICallback
{   
    public:
        ICallback() = default;
        virtual ~ICallback() = default;
        virtual void BinaryPacket(const char* Data,std::size_t size) = 0;
        virtual void TextPacket(const char* Data,std::size_t size) = 0;
};


/**
 * @brief Реализация интерфейса обработки пакета.
 */
class Callback: public ICallback
{   
    public:
        Callback()
        {}
        void BinaryPacket(const char* Data,std::size_t size) override;
        void TextPacket(const char* Data,std::size_t size) override;
};
