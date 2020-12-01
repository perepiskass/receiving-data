// загрузка проекта
git clone [url] [path]
cd [path]
git submodule update --init --recursive

// сборка проекта
mkdir build
cd build
cmake ..
cmake --build .

// запуск
if(MSVC)
    .\test\Debug\test_list.exe
else
    ./test/test_list

// задача
Реализовать интерфейс IReceiver.
 На вход объекту класса через этот интерфейс блоками различного размера будет последовательно поступать непрерывный поток
данных.
Поток может содержать пакеты двух типов:
Если пакет начинается с байта 0x24 (1 байт), то это "бинарный" пакет. За 0x24 следует размер данных пакета в байтах
(четырёхбайтовое целое без знака, порядок байт – little endian), а затем – сами данные.
Иначе – пакет "текстовый". "Текстовый" пакет заканчивается на последовательность байт "\r\n\r\n".
Пакеты различных типов могут следовать в произвольном порядке. Данные одного пакета идут строго последовательно, данные
различных пакетов между собой не перемешиваются. При этом поток данных может быть разбит на блоки произвольным образом
независимо от границ пакетов (т. е. могут быть как несколько пакетов в одном блоке, так и один пакет в нескольких блоках).
Как только обнаружен конец пакета, необходимо его данные пакета передать потребителю через интерфейс ICallback.
Реализация ICallback не требуется.
Класс должен быть как можно эффективнее при следующих условиях: средний поток, подаваемый на вход – 500 Мбит/с, средняя
длина блока входных данных – 1024 байт.