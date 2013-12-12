
#include "PlatformIeeeEui64.h"
#include "DallasId48.h"
#warning "Using my DallasToIEEE"

module DallasId48ToIeeeEui64C {
  provides interface LocalIeeeEui64;
  
  uses {
    interface OneWireStream as OneWire;
  }
}

implementation {

  ieee_eui64_t eui = {{ IEEE_EUI64_COMPANY_ID_0,
                        IEEE_EUI64_COMPANY_ID_1,
                        IEEE_EUI64_COMPANY_ID_2,
                        IEEE_EUI64_SERIAL_ID_0,
                        IEEE_EUI64_SERIAL_ID_1,
                        0, 0, 0 }};
  bool have_id = FALSE;
                       
  command ieee_eui64_t LocalIeeeEui64.getId () {
  //  uint8_t id[6];
    
    dallasid48_serial_t ds2411id;
    error_t e;
    
    if (!have_id) {
      e = call OneWire.read(DALLASID48_CMD_READ, ds2411id.data, DALLASID48_DATA_LENGTH);
      if (e == SUCCESS && dallasid48checkCrc(&ds2411id)) {
        have_id = TRUE;
        eui.data[5] = ds2411id.serial[2];
        eui.data[6] = ds2411id.serial[1];
        eui.data[7] = ds2411id.serial[0];
      }
    }
    
    return eui;
  }

}
