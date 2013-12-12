#include "OpoSimpleDemo.h"

module OpoSimpleDemoP {
    uses {
        interface Leds;
        interface Opo;
        interface AMSend as PrepSend;
        interface SplitControl as FlashPower;
        interface Random;
        interface Boot;
        interface Timer<TMilli> as TxTimer;
        //interface Timer<TMilli> as RxTimer;
        //interface Timer<TMilli> as LedTimer;
        interface ReadId48 as IdReader;
    }
}



implementation {

    //void setRandGuard();
    //uint8_t add_interaction(uint8_t o_id[6]);

    //uint8_t m_id[6];
    //uint8_t nodes[NUM_NODES][6];
    //uint8_t n_index;
    //uint16_t interactions[NUM_NODES];
    //uint32_t randGuard;
    //uint32_t range_sequence;

    //int i;
    //opo_simple_rf_msg_t *p;
    //message_t packet;
    //size_t msg_size = sizeof(opo_simple_rf_msg_t);


    event void Boot.booted() {
        /*
        call IdReader.read(&m_id[0]);

        p = (opo_simple_rf_msg_t*) call PrepSend.getPayload(&packet,
                                                            msg_size);
        for(i = 0; i < 6; i++) {
            p->m_id[i] = m_id[i];
        }

        call FlashPower.stop();
        call Opo.setup_pins();

        setRandGuard();
        call OpoTxTimer.startOneShot(2000 + randGuard); */
    }

    event void Opo.receive(uint32_t range, message_t* msg) {
/*
        if(range <= 2000000) {
            uint16_t num_inters;
            p = (opo_simple_rf_msg_t*) msg->data;
            num_inters = add_interaction(p->m_id);
            call Leds.led0On();
            if(num_inters > 25) {
                call Leds.led1On();
            }
            call LedTimer.startOneShot(15);
        }
        setRandGuard();
        call OpoRxTimer.startOneShot(60);
        call OpoTxTimer.startOneShot(1500 + randGuard);
        */
    }

    event void Opo.transmit_done() {
        /*
        setRandGuard();
        call OpoRxTimer.startOneShot(60);
        call OpoTxTimer.startOneShot(1500 + randGuard); */
    }

    event void Opo.receive_failed() {
        /* setRandGuard();
        call OpoRxTimer.startOneShot(120);
        call OpoTxTimer.startOneShot(2300 + randGuard); */
    }

   // event void LedTimer.fired() {
        /*
        call Leds.led0Off();
        call Leds.led1Off(); */
    //}

    event void TxTimer.fired() {
        //call Opo.transmit(&packet, msg_size);
    }

    //event void RxTimer.fired() {
        //call Opo.enable_receive();
    //}

    event void PrepSend.sendDone(message_t* bufPtr, error_t error) {}
    event void FlashPower.startDone(error_t err) {}
    event void FlashPower.stopDone(error_t err) {}
/*
    inline void setRandGuard() {
        atomic {
            randGuard = call Random.rand32();
            randGuard = (randGuard % 51) * 40;
        }
    }

    inline uint8_t add_interaction(uint8_t o_id[6]) {
        bool found = TRUE;
        int j;

        for(i=0; i < n_index; i++) {
            for(j=0; j < 6; j++) {
                if(o_id[j] != nodes[i][j]) {
                    found = FALSE;
                    break;
                }
            }
            if(found == TRUE) {
                interactions[i] += 1;
                return interactions[i];
            }
        }

        for(i=0; i < 6; i++) {
            nodes[n_index][i] = o_id[i];
        }
        interactions[n_index] += 1;

        if(n_index < NUM_NODES)
            n_index++;

        return interactions[n_index-1];
    } */
}