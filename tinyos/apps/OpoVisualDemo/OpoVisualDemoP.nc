#include "OpoVisualDemo.h"

#ifdef OPO_DEBUG
#include "printf.h"
#endif

module OpoVisualDemoP {
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
        interface Timer<TMilli> as RTCTimer;
        interface PacketAcknowledgements as PacketAcks;
        interface AMSend as BaseSend;
        interface SplitControl as RfControl;
        interface HplRV4162;
        interface CC2420Config;
    }
}

implementation {
    message_t packet;
    message_t base_packet;
    ovis_base_msg_t *bp;
    ovis_msg_t *opo_data;
    ovis_msg_t *p;
    uint32_t tx_interval_min = 0;
    uint8_t m_id[6];
    uint32_t guard;
    uint32_t dt;
    uint32_t t0;
    uint32_t tn;
    uint32_t rt;
    uint8_t i;
    uint32_t RX_DELAY = 70;
    uint16_t rx_fails = 0;
    uint16_t tx_fails = 0;
    uint16_t seq = 0;

    enum {ENABLE_RX, BASE_SEND} ovis_rx_state = ENABLE_RX;

    void setGuardTime();
    uint16_t getInteractions(message_t* msg);

    event void Boot.booted() {
        uint32_t sum;

        call Opo.setup_pins();

        call PacketAcks.noAck(&packet);
        call PacketAcks.noAck(&base_packet);
        setGuardTime();

        for(;;) {
            call IdReader.read(&m_id[0]);
            for(i = 0; i < 6; i++) {
                sum += m_id[i];
            }
            if(sum != 0)
                break;
        }

        p = (ovis_msg_t*) call Packet.getPayload(&packet,
                                                 sizeof(ovis_msg_t));
        bp = (ovis_base_msg_t*) call Packet.getPayload(&base_packet,
                                                       sizeof(ovis_base_msg_t));
        for(i = 0; i < 6; i++) {
            p -> tx_id[i] = m_id[i];
            bp -> rx_id[i] = m_id[i];
        }


        call HplRV4162.writeSTBit();

        #ifdef OPO_DEBUG
        printf("Booted\n");
        printfflush();
        #endif
    }

    event void RxTimer.fired() {
        #ifdef OPO_DEBUG
        printf("RXT\n");
        printfflush();
        #endif

        if(ovis_rx_state == ENABLE_RX) {
            call Opo.enable_receive();
        }
        else if (ovis_rx_state == BASE_SEND) {
            call HplRV4162.readFullTime();
        }
    }

    event void TxTimer.fired() {
        #ifdef OPO_DEBUG
        printf("TXT\n");
        printfflush();
        #endif

        p = (ovis_msg_t*) call Packet.getPayload(&packet,
                                                 sizeof(ovis_msg_t));
        p->seq = seq;

        call Opo.transmit(&packet, sizeof(ovis_msg_t));
    }

    event void Opo.transmit_done() {
        #ifdef OPO_DEBUG
        printf("TxD\n");
        printf("G: %lu\n", guard);
        printfflush();
        #endif

        call RxTimer.stop();
        setGuardTime();
        seq += 1;
        call TxTimer.startOneShot(1000 + guard);
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void Opo.transmit_failed() {
        #ifdef OPO_DEBUG
        printf("TxF\n");
        printfflush();
        #endif
        tx_fails += 1;
        call TxTimer.startOneShot(guard + 75);
    }

    event void Opo.receive(uint16_t t_rf,
                           uint16_t t_ultrasonic_wake,
                           uint16_t t_ultrasonic_wake_falling,
                           uint16_t t_ultrasonic,
                           uint16_t t_ultrasonic_falling,
                           message_t* msg) {
        #ifdef OPO_DEBUG
        printf("Range: %lu\n", range);
        printfflush();
        #endif

        uint16_t ultrasonic_wake_dt = 0;
        uint16_t ultrasonic_dt = 0;
        uint16_t ultrasonic_rf_dt = 0;

        call TxTimer.stop();
        tn = call TxTimer.getNow();
        t0 = call TxTimer.gett0();
        dt = call TxTimer.getdt();
        if (tn > t0) {
            if (dt > (tn - t0)) {
                rt = dt - (tn - t0);
            }
            else {
                rt = dt;
            }
        }
        else {
            rt = dt;
        }

        opo_data = call Packet.getPayload(msg, sizeof(ovis_msg_t));

        if (t_ultrasonic_wake_falling > t_ultrasonic_wake) {
            ultrasonic_wake_dt = t_ultrasonic_wake_falling - t_ultrasonic_wake;
        }
        if (t_ultrasonic_falling > t_ultrasonic) {
            ultrasonic_dt = t_ultrasonic_falling - t_ultrasonic;
        }
        if (t_ultrasonic > t_rf) {
            ultrasonic_rf_dt = t_ultrasonic - t_rf;
        }

        bp->ultrasonic_rf_dt = ultrasonic_rf_dt;
        bp->ultrasonic_wake_dt = ultrasonic_wake_dt;
        bp->ultrasonic_dt = ultrasonic_dt;
        bp->seq = opo_data->seq;
        bp->rx_fails = rx_fails;
        bp->tx_fails = tx_fails;

        for(i=0; i<6; i++) {
            bp->tx_id[i] = opo_data->tx_id[i];
        }

        ovis_rx_state = BASE_SEND;
        call CC2420Config.setChannel(BASE_CHANNEL);
        call RxTimer.startOneShot(5 + (guard % 40) );
    }

    event void Opo.receive_failed() {
        #ifdef OPO_DEBUG
        printf("RxF\n");
        printfflush();
        #endif
        rx_fails += 1;
        call RxTimer.startOneShot(RX_DELAY);
    }

    event void Opo.enable_receive_failed() {
        #ifdef OPO_DEBUG
        printf("RxEF\n");
        printfflush();
        #endif

        //call RxTimer.startOneShot(RX_DELAY);
    }

    event void BaseSend.sendDone(message_t *msg, error_t err) {
        #ifdef OPO_DEBUG
        printf("BSD\n");
        printfflush();
        #endif
        call RfControl.stop();
    }

    event void RfControl.startDone(error_t err) {
        if(ovis_rx_state == BASE_SEND) {
            call BaseSend.send(AM_BROADCAST_ADDR,
                               &base_packet,
                               sizeof(ovis_base_msg_t));
        }
    }

    event void RfControl.stopDone(error_t err) {
        if(ovis_rx_state == BASE_SEND) {
            ovis_rx_state = ENABLE_RX;
            call CC2420Config.setChannel(OPO_CHANNEL);
            call RxTimer.startOneShot(RX_DELAY);
            call TxTimer.startOneShot(rt);
        }
    }

    event void RTCTimer.fired() {
        call HplRV4162.resetTime();
    }

    event void HplRV4162.readFullTimeDone(error_t err, uint8_t *fullTime) {
        uint16_t time = fullTime[1] + (fullTime[2] * 60) + (fullTime[3] * 60 * 60);
        bp->full_time = fullTime[1];
        call RfControl.start();
    }

    event void HplRV4162.writeSTBitDone(error_t err) {
        call RTCTimer.startOneShot(20);
    }

    event void HplRV4162.setTimeDone(error_t err) {}

    event void HplRV4162.resetTimeDone(error_t err) {
        call RxTimer.startOneShot(RX_DELAY);
        call TxTimer.startOneShot(2000 + guard);
    }

    event void FlashPower.startDone(error_t err) {}
    event void FlashPower.stopDone(error_t err) {}

    event void CC2420Config.syncDone(error_t error) {}

    inline void setGuardTime() {
        guard = call Random.rand32();
        guard = guard % 2000;
    }

}