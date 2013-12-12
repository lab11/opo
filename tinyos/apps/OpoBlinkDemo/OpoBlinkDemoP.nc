#include "OpoBlinkDemo.h"

#ifdef OPO_DEBUG
#include "printf.h"
#endif

module OpoBlinkDemoP {
    uses {
        interface Boot;
        interface Leds;
        interface Random;
        interface ReadId48 as IdReader;
        interface SplitControl as FlashPower;
        interface Packet;
        interface Opo;
        interface Timer<TMilli> as TxTimer;
        interface Timer<TMilli> as RxTimer;
        interface Timer<TMilli> as LedsTimer;
    }
}

implementation {
    message_t packet;
    message_t base_packet;
    uint32_t tx_interval_min = 0;
    uint8_t m_id[6];
    uint8_t o_ids[MAX_NODES][6];
    uint8_t node_index;
    uint16_t interactions[MAX_NODES];
    uint32_t guard;

    void setGuardTime();
    uint16_t getInteractions(message_t* msg);

    event void Boot.booted() {
        uint8_t i;
        oblink_msg_t *p;
        call IdReader.read(&m_id[0]);

        p = (oblink_msg_t*) call Packet.getPayload(&packet,
                                                   sizeof(oblink_msg_t));
        for(i = 0; i < 6; i++) {
            p -> m_id[i] = m_id[i];
        }

        call Opo.setup_pins();
        setGuardTime();
        call RxTimer.startOneShot(70);
        call TxTimer.startOneShot(2000 + guard);

        #ifdef OPO_DEBUG
        printf("Booted\n");
        printfflush();
        #endif
    }

    event void RxTimer.fired() {
        call Opo.enable_receive();
    }

    event void TxTimer.fired() {
        call Opo.transmit(&packet, sizeof(oblink_msg_t));
    }

    event void LedsTimer.fired() {
        call Leds.led0Off();
        call Leds.led1Off();
    }

    event void Opo.transmit_done() {
        #ifdef OPO_DEBUG
        printf("TxD\n");
        printf("G: %lu\n", guard);
        printfflush();
        #endif
        call RxTimer.stop();
        setGuardTime();
        call TxTimer.startOneShot(1500 + (guard*40));
        call RxTimer.startOneShot(200);
    }

    event void Opo.transmit_failed() {
        #ifdef OPO_DEBUG
        printf("TxF\n");
        printfflush();
        #endif
        call TxTimer.startOneShot(500 + (guard*2));
    }

    event void Opo.receive(uint32_t range, message_t* msg) {
        #ifdef OPO_DEBUG
        printf("Range: %lu\n", range);
        printfflush();
        #endif

        //call TxTimer.stop();

        if(range <= 1000000) {
            call Leds.led0On();
            if(getInteractions(msg) > 168) {
                call Leds.led1On();
            }
        }
        call LedsTimer.startOneShot(100);

        call RxTimer.startOneShot(200);
        //call TxTimer.startOneShot(1000 + (guard * 2));
    }

    event void Opo.receive_failed() {
        #ifdef OPO_DEBUG
        printf("RxF\n");
        printfflush();
        #endif
        call RxTimer.startOneShot(200);
    }

    event void Opo.enable_receive_failed() {
        #ifdef OPO_DEBUG
        printf("RxEF\n");
        printfflush();
        #endif
        call RxTimer.startOneShot(200);
    }

    event void FlashPower.startDone(error_t err) {}
    event void FlashPower.stopDone(error_t err) {}

    inline void setGuardTime() {
        guard = call Random.rand32();
        guard = (guard % 51);
    }

    inline uint16_t getInteractions(message_t *msg) {
        uint8_t i, j;
        bool found;
        oblink_msg_t* m = (oblink_msg_t*) call Packet.getPayload(msg,
                                                                 sizeof(oblink_msg_t));

        for(i=0; i < node_index; i++) {
            found = TRUE;
            for(j=0; j < 6; j++) {
                if(m->m_id[j] != o_ids[i][j]) {
                    found = FALSE;
                }
            }
            if(found == TRUE) {
                return ++interactions[i];
            }
        }

        for(i=0; i<6; i++) {
            o_ids[node_index][i] = m->m_id[i];
        }
        interactions[node_index] += 1;

        node_index++;
        return interactions[node_index-1];
    }
}