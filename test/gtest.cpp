#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "receiver.h"


TEST(receiver_test_case, create_receiver)
{
    IReceiver* ptr = nullptr;
    ASSERT_FALSE(ptr);

    ptr = new Receiver();

    ASSERT_TRUE(ptr);
    delete ptr;
}

TEST(receiver_test_case, create_receiver_with_argument)
{
    IReceiver* ptr_receiver = nullptr;
    IReceiver* ptr_receiver_2 = nullptr;
    IReceiver* ptr_receiver_3 = nullptr;
    ASSERT_FALSE(ptr_receiver);
    ASSERT_FALSE(ptr_receiver_2);
    ASSERT_FALSE(ptr_receiver_3);
    
    std::unique_ptr<ICallback> ptr_callback = std::make_unique<Callback>();

    ptr_receiver = new Receiver(std::unique_ptr<ICallback>(new Callback()));
    ptr_receiver_2 = new Receiver(std::move(ptr_callback));
    ptr_receiver_3 = new Receiver(std::make_unique<Callback>());

    ASSERT_TRUE(ptr_receiver);
    ASSERT_TRUE(ptr_receiver_2);
    ASSERT_TRUE(ptr_receiver_3);
    delete ptr_receiver_3;
    delete ptr_receiver_2;
    delete ptr_receiver;
}

TEST(receiver_test_case, Receiver_exeption_nullptr)
{
    IReceiver* ptr = nullptr;
    ptr = new Receiver();

    ASSERT_THROW(ptr->Receive(nullptr,10), std::domain_error);

    delete ptr;
}

TEST(receiver_test_case, Receiver_exeption_zero_size)
{
    IReceiver* ptr = nullptr;
    ptr = new Receiver();
    char data[5] = {'a','b','c','d','e'};

    ASSERT_THROW(ptr->Receive(data,0), std::domain_error);

    delete ptr;
}

// Gmock class -------------------------------------------------------------------
using testing::AtLeast;
// using testing::AtMost;
// using testing::Return;
using testing::_;

class MockCallback : public ICallback {

    public:
        MOCK_METHOD(
            void, 
            BinaryPacket, 
            (const char* Data,std::size_t size), 
            (override)
        );

        MOCK_METHOD(
            void,
            TextPacket,
            (const char* Data,std::size_t size),
            (override)
        );
};


// TestFixture--------------------------------------------------------------------------------
struct receiverFixture : public testing::Test {
    // Per-test-suite set-up.
    MockCallback mock_callback;
    IReceiver* receiver = nullptr;

    static void SetUpTestSuite() {
    }
    static void TearDownTestSuite() {
    }

    void SetUp() override {
        delete receiver;
        receiver = new Receiver(std::unique_ptr<ICallback>(&mock_callback));
     }
    void TearDown() override {
        // delete receiver;
    }
};
//--------------------------------------------------------------------------------------------

TEST_F(receiverFixture, call_text_callback)
{
    char header = 'T';
    char data[10] = {header,'h', 'e', 'l', 'l', 'o', '\r', '\n', '\r', '\n'};

    EXPECT_CALL(mock_callback, TextPacket(_, _)).Times(AtLeast(1));

    receiver->Receive(data, 10);
}

TEST_F(receiverFixture, call_bin_callback)
{
    char header = 0x24;
    char data[10] = {header, 0x05, 0x00, 0x00, 0x00, 'o', '\r', '\n', '\r', '\n'};

    EXPECT_CALL(mock_callback, BinaryPacket(_, _)).Times(AtLeast(1));

    receiver->Receive(data, 10);
}

TEST_F(receiverFixture, call_text_bin_callback)
{
    char header_bin = 0x24;
    char header_text = 'T';
    char data[20] = {header_bin, 0x05, 0x00, 0x00, 0x00, 'h', 'e', 'l', 'l', 'o',
                    header_text,'h', 'e', 'l', 'l', 'o', '\r', '\n', '\r', '\n'};

    EXPECT_CALL(mock_callback, BinaryPacket(_, _)).Times(AtLeast(1));
    EXPECT_CALL(mock_callback, TextPacket(_, _)).Times(AtLeast(1));

    receiver->Receive(data, 20);
}

TEST_F(receiverFixture, call_Receive_twice_for_bin)
{
    char header = 0x24;
    char data_1[10] = {header, 0x0b, 0x00, 0x00, 0x00, 'h', 'e', 'l', 'l', 'o'};
    char data_2[6] = {' ','w','o','r','l','d'};

    EXPECT_CALL(mock_callback, BinaryPacket(_, _)).Times(AtLeast(1));

    receiver->Receive(data_1, 10);
    receiver->Receive(data_2, 6);
}

TEST_F(receiverFixture, call_Receive_twice_for_text)
{
    char header = 'T';
    char data_1[6] = {header, 'h', 'e', 'l', 'l', 'o'};
    char data_2[10] = {' ','w','o','r','l','d','\r', '\n', '\r', '\n'};

    EXPECT_CALL(mock_callback, TextPacket(_, _)).Times(AtLeast(1));

    receiver->Receive(data_1, 6);
    receiver->Receive(data_2, 10);
}

TEST_F(receiverFixture, one_block_two_bin_callback)
{
    char header_bin = 0x24;
    char header_text = 'T';
    char data[28] = {header_bin, 0x05, 0x00, 0x00, 0x00, 'b', 'b', 'b', 'b', 'b',
                    header_text,'h', 'e', 'l', 'l', 'o', '\r','\n', '\r', '\n',
                    header_bin,0x03, 0x00, 0x00, 0x00,'y','e','s'};

    EXPECT_CALL(mock_callback, BinaryPacket(_, _)).Times(AtLeast(2));
    EXPECT_CALL(mock_callback, TextPacket(_, _)).Times(AtLeast(1));

    receiver->Receive(data, 28);
}

TEST_F(receiverFixture, one_block_two_text_callback)
{
    char header_bin = 0x24;
    char header_text = 'T';
    char data[30] = {header_text,'m', 'a', 'z', 'd', 'a', '\r', '\n', '\r', '\n',
                    header_bin,0x03, 0x00, 0x00, 0x00,'y','e','s',
                    header_text,'f','e','r','r','a','r','i','\r','\n','\r','\n'};

    EXPECT_CALL(mock_callback, BinaryPacket(_, _)).Times(AtLeast(1));
    EXPECT_CALL(mock_callback, TextPacket(_, _)).Times(AtLeast(2));

    receiver->Receive(data, 30);
}

TEST_F(receiverFixture, text_callback_equal_size)
{
    const size_t size = 11;

    char header = 'T';
    char data_1[13] = {header, 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd','\r',};
    char data_2[3] = {'\n','\r','\n'};

    EXPECT_CALL(mock_callback, TextPacket(_, size)).Times(AtLeast(1));

    receiver->Receive(data_1, 13);
    receiver->Receive(data_2, 3);
}

TEST_F(receiverFixture, bin_callback_equal_size)
{
    char header = 0x24;
    std::vector<char> bin_data(1029, 'd');

    bin_data[0] = header;
    //1024 little endian
    bin_data[1] = 0x00;
    bin_data[2] = 0x04;
    bin_data[3] = 0x00;
    bin_data[4] = 0x00;

    EXPECT_CALL(mock_callback, BinaryPacket(_, 1024)).Times(AtLeast(1));

    receiver->Receive(bin_data.data(), 3);
    receiver->Receive(bin_data.data() + 3, 1026);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}