// Only use for non-commercial purposes
NTPtime NTPch("ch.pool.ntp.org");   // Server NTP

bool getTimeWatering()
{
  dateTime = NTPch.getNTPtime(7.0, 0); // Timezone
  
  // check dateTime.valid before getting time
  if(dateTime.valid)
  {
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;      // Gio
    byte actualMinute = dateTime.minute;  // Phut
    byte actualSecond = dateTime.second;  // Giay

    if((actualHour >= startWaterTime && actualHour <= endWaterTime) && ((actualHour % timeBTW == 0) && (actualMinute == 0) && (actualSecond <= 25)))
      return true;
  }// Acess water garden between [startWaterTime..endWaterTime] hour.
  return false;
}
