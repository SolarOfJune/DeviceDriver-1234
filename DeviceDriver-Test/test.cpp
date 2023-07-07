#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../DeviceDriver/DeviceDriver.cpp"

using namespace testing;

class MockFlashMemoryDevice : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(TestCaseName, ReadFail) {
	MockFlashMemoryDevice mock;
	EXPECT_CALL(mock, read(_))
		.WillOnce(Return(1))
		.WillRepeatedly(Return(2));

	DeviceDriver driver{&mock};
	EXPECT_THROW(driver.read(0x80008000), exception);
}

TEST(TestCaseName, ReadPass) {
	MockFlashMemoryDevice mock;
	EXPECT_CALL(mock, read(_))
		.Times(5)
		.WillRepeatedly(Return(2));

	DeviceDriver driver{ &mock };
	EXPECT_EQ(2, driver.read(0x80008000));
}