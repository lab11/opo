#include "cc2538-rf-debug.h"
#include "contiki.h"
#include "cc2538-rf.h"
#include <stdbool.h>
#include <stdint.h>
#include "packetqueue.h"
#include "netstack.h"
#include <string.h>

static bool debug_clear = true;

static void rf_debug_callback() { debug_clear = true; }

void send_rf_debug_msg(char msg[]) {
    #if CC2538_RF_PRINT
    if(debug_clear) {
        debug_clear = false;
        char sender[110] = {0};
        uint8_t s = strlen(msg);
        sender[0] = 0xaa;
        sender[1] = 0xbb;
        sender[2] = 0xcc;
        sender[3] = 0xdd;
        sender[4] = RF_DEBUG_ID;
        sender[5] = s;
        uint8_t i;
        if(s < 100) { for(i=0;i<s;i++){sender[6+i] = msg[i];} }
        RF_TXDONE_HANDLER.set_callback(rf_debug_callback);
        packetbuf_clear();
        packetbuf_copyfrom((void *) sender, s + 10);
        cc2538_on_and_transmit(0);
        NETSTACK_MAC.off(0);
    } else {}
    #endif
}
