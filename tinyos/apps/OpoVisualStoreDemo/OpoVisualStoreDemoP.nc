#include "OpoVisualStoreDemo.h"

#ifdef OPO_DEBUG
#include "printf.h"
#endif

module OpoVisualStoreDemoP {
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
        interface BlockWrite;
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
    uint32_t tx_count = 0;
    uint8_t m_id[6];
    uint32_t guard;
    uint32_t dt;
    uint32_t t0;
    uint32_t tn;
    uint32_t rt;
    uint8_t i;

    enum {ENABLE_RX, BASE_SEND} ovis_rx_state = ENABLE_RX;

    void setGuardTime();
    uint16_t getInteractions(message_t* msg);

    event void Boot.booted() {
        ovis_msg_t *p;

        call Opo.setup_pins();
        call IdReader.read(&m_id[0]);
        call PacketAcks.noAck(&packet);
        call PacketAcks.noAck(&base_packet);
        setGuardTime();

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
        call TxTimer.startOneShot(2000 + guard);
        call RxTimer.startOneShot(70);
    }

    event void Opo.transmit_failed() {
        #ifdef OPO_DEBUG
        printf("TxF\n");
        printfflush();
        #endif

        call TxTimer.startOneShot(500 + (guard*2));
    }

    event void Opo.receive(uint16_t t_rf, uint16_t t_ultrasonic, message_t* msg) {
        #ifdef OPO_DEBUG
        printf("Range: %lu\n", range);
        printfflush();
        #endif

        call TxTimer.stop();
        tn = call TxTimer.getNow();
        t0 = call TxTimer.gett0();
        dt = call TxTimer.getdt();
        rt = dt - (tn - t0);

        opo_data = call Packet.getPayload(msg, sizeof(ovis_msg_t));

        bp->t_rf = t_rf;
        bp->t_ultrasonic = t_ultrasonic;
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

        call RxTimer.startOneShot(100);
    }

    event void Opo.enable_receive_failed() {
        #ifdef OPO_DEBUG
        printf("RxEF\n");
        printfflush();
        #endif

        call RxTimer.startOneShot(100);
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
            if(buffer_index == )
            call RxTimer.startOneShot(70);
            call TxTimer.startOneShot(rt);
        }
    }

    event void BlockWrite.writeDone(storage_addr_t addr,
                       void* buf, storage_len_t len,
                       error_t error) {
        call BlockWrite.sync();
    }

    event void BlockWrite.eraseDone(error_t err) {
        call RxTimer.startOneShot(70);
        call TxTimer.startOneShot(2000 + guard);
    }

    event void BlockWrite.syncDone(error_t err) {
        call RxTimer.startOneShot(70);
        call TxTimer.startOneShot(rt);
    }

    event void RTCTimer.fired() {
        call HplRV4162.resetTime();
    }

    event void HplRV4162.readFullTimeDone(error_t err, uint8_t *fullTime) {
        bp->sec = fullTime[1];
        bp->min = fullTime[2];
        bp->h = fullTime[3];
        call RfControl.start();
    }

    event void HplRV4162.writeSTBitDone(error_t err) {
        call RTCTimer.startOneShot(20);
    }

    event void HplRV4162.setTimeDone(error_t err) {}

    event void HplRV4162.resetTimeDone(error_t err) {
        call BlockWrite.erase();
    }

    event void FlashPower.startDone(error_t err) {}
    event void FlashPower.stopDone(error_t err) {}

    event void CC2420Config.syncDone(error_t error) {}

    inline void setGuardTime() {
        guard = call Random.rand32();
        guard = guard % 2000;
    }

}