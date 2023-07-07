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
	Application app{ &driver };
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
		.WillRepeatedly(Return(EMPTY_VAL));
	driver.write(0x00, 0x2);
	EXPECT_CALL(mock, read(0x00))
		.WillRepeatedly(Return(0x2));
	EXPECT_EQ(0x2, driver.read(0x00));
}

TEST_F(FixtureDeviceDriver, AppReadAndPrint)
{
	EXPECT_CALL(mock, read(0x200))
		.WillRepeatedly(Return(0x5));
	EXPECT_CALL(mock, read(0x201))
		.WillRepeatedly(Return(0x6));
	EXPECT_CALL(mock, read(0x202))
		.WillRepeatedly(Return(0x7));
	EXPECT_CALL(mock, read(0x203))
		.WillRepeatedly(Return(0x8));

	app.ReadAndPrint(0x200, 0x204);
}

TEST_F(FixtureDeviceDriver, AppWriteAll)
{
	EXPECT_CALL(mock, read(0x0))
		.WillRepeatedly(Return(EMPTY_VAL));
	EXPECT_CALL(mock, read(0x1))
		.WillRepeatedly(Return(EMPTY_VAL));
	EXPECT_CALL(mock, read(0x2))
		.WillRepeatedly(Return(EMPTY_VAL));
	EXPECT_CALL(mock, read(0x3))
		.WillRepeatedly(Return(EMPTY_VAL));
	app.WriteAll(0xA);
}