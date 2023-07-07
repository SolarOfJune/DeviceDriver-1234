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

class FixtureDeviceDriver : public testing::Test
{
public:
	MockFlashMemoryDevice mock;
	DeviceDriver driver{ &mock };
};

TEST_F(FixtureDeviceDriver, ReadFail) {
	EXPECT_CALL(mock, read(_))
		.WillOnce(Return(1))
		.WillRepeatedly(Return(2));
	EXPECT_THROW(driver.read(0x80008000), ReadFailException);
}

TEST_F(FixtureDeviceDriver, ReadPass) {
	EXPECT_CALL(mock, read(_))
		.Times(5)
		.WillRepeatedly(Return(2));
	EXPECT_EQ(2, driver.read(0x80008000));
}

TEST_F(FixtureDeviceDriver, WriteFail) {
	EXPECT_CALL(mock, read(0x00))
		.WillRepeatedly(Return(0x1));
	EXPECT_THROW(driver.write(0x00, 0x2), WriteFailException);
}

TEST_F(FixtureDeviceDriver, WritePass) {
	EXPECT_CALL(mock, read(0x00))
		.WillRepeatedly(Return(0xFF));
	driver.write(0x00, 0x2);
	EXPECT_CALL(mock, read(0x00))
		.WillRepeatedly(Return(0x2));
	EXPECT_EQ(0x2, driver.read(0x00));
}