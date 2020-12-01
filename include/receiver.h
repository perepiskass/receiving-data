#pragma once
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include "callback.h"


/**
 * @brief Интерфейс для реализации обработки входящего потока.
 */
class IReceiver
{
    public:
        virtual ~IReceiver() = default;
        virtual void Receive(const char* Data, std::size_t size) = 0;
};


/**
 * @brief Реализация интерфейса, для обработки входящего потока.
 */
class Receiver: public IReceiver
{
    public:
        Receiver(std::unique_ptr<ICallback> call = std::make_unique<Callback>()):
        m_callback(std::move(call))
        {
            m_data.reserve(2048);
        }
        void Receive(const char* Data, std::size_t size) override;
    private:
        bool CheckHandler(char c) const;
        void BinReceive();
        void TextReceive(size_t size);
        uint32_t ToBigEndian(const std::vector<char>& vec) const;

        std::vector<char> m_data;

        std::unique_ptr<ICallback> m_callback;
};

