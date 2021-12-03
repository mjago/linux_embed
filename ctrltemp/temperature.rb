require "i2c"
require "i2c/driver/i2c-dev"
device = I2CDevice.new(address: 0x60, driver: I2CDevice::Driver::I2CDev.new("/dev/i2c-1"))

count = 1
loop do
  puts `clear`
  puts count
#  device.i2cget(0x01, 0x30)
  puts `i2cdetect -y 1`
  sleep 0.2
  count += 1
end
