This just does LPL 

## General Config: 
MAX_ID in the c file sets up what's the maximum ID we accept. I think I did this so that these could run on the same chanel 
as the ultrasonic ones. 

## RX side: 
We turn on our radio to receive every rx_base_time (3.5 seconds)
We keep our radio on for rx_on_time (100 ms)

First timer used is **rf_rx_vtimer_callback**, which triggers **rssi_rf_rx_proccess** every *rx_base_time*.

**rssi_rf_rx_process** then turns on the radio and sets a finished timer **rf_rx_on_vtimer** for *rx_on_time*.
It also checks to see that we're not plugged in and not currently transmitting.

If we get a radio packet, it triggers **rf_rx_received_callback**, which triggers **rssi_rf_rx_received_proccess**. 
We get the RSSI of the packet by calling 
'''
uint8_t rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
'''
For some reason this has to be done before reading hte actual packet using '''packetbuf_copyto'''

At the end of *rx_on_time*, **rf_rx_on_vtimer** triggers **rf_rx_on_vtimer_callback** which triggers **rssi_rf_rx_done_proccess**. 
**rssi_rf_rx_done_process** stores all the data, turns the radio off, and triggers any pending transmissions. 

Potential problems with the receive side: 
During receive time we don't send any data. There's also no randomization on the receive side right now. So if two nodes happen to be synced, 
they never get out of it. It should be totally possible to transmit while in the RX mode. 

With sending during receiving time, I actually have no idea what happens if we try and transmit while there are packets in the receive buffer. 
IIRC even though we read/write the packetbuf to one register, it actually maps to different locations in memory. This came up for the timing 
stuff where we write directly to the actual TX buffer location. 

## Data storage 

So we're using the opo storage format, but we're multiplexing in our shit, since it's just one byte long. 

## TODO RF side: 
Add in ability to transmit during RX time? Or maybe just a somewhat randomized RX timer. 

## TX side
We turn on to transmit every *tx_base_time* - *(tx_base_time + generate_tx_buffer())* ms (10 - 50 ms) using **rf_tx_vtimer**
**rf_tx_vtimer** triggers **rf_tx_timer_callback** which in turn triggers **rssi_rf_tx_proccess**

It looks like I'm setting the transmit power to be lower than normal for this part? 
I think this was both to save power and also to try and mimic what other people do by trying to get the transmit power lower enough such that 
the packets get absorbed by the human body. 



