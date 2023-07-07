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

TEST(TestCaseName, WriteFail) {
	MockFlashMemoryDevice mock;
	EXPECT_CALL(mock, read(0x00))
		.WillRepeatedly(Return(0x1));

	DeviceDriver driver{ &mock };
	EXPECT_THROW(driver.write(0x00, 0x2), exception);
}

TEST(TestCaseName, WritePass) {
	MockFlashMemoryDevice mock;
	EXPECT_CALL(mock, read(0x00))
		.WillRepeatedly(Return(0xFF));

	DeviceDriver driver{ &mock };
	driver.write(0x00, 0x2);
	EXPECT_CALL(mock, read(0x00))
		.WillRepeatedly(Return(0x2));

	EXPECT_EQ(0x2, driver.read(0x00));
}