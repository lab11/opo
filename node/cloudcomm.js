var noble = require('noble')
var async = require('async')
var sleep = require('sleep')
var request = require('request')
var fs = require('fs')

var cc_service_uuid = '2000'
var cc_data_uuid = '2b00'
var cc_meta_uuid = '2b01'

var cc_service_uuid = "2000"
var cc_data_uuid = "2b00"
var cc_meta_uuid = "2b01"
var cc_req_uuid = "2b02"
var cc_inc_uuid = "2b03"
var cc_ready_uuid = "2b04"
var cc_ack_string = "2b05"

var cc_data_char = 0 // Characteristic the
var cc_meta_char = 0
var cc_req_char = 0
var cc_inc_char = 0
var cc_ready_char = 0
var cc_ack_char = 0

var urlSet = false
var urlNode = ""

var dataBuilder = null

noble.on('stateChange', function(state) {
  if (state === 'poweredOn') {
    noble.startScanning();
  } else {
    noble.stopScanning();
  }
})

noble.on('discover', on_discovery)

function on_discovery(peripheral) {
	var advertisement = peripheral.advertisement;

	var localName = advertisement.localName;
	var txPowerLevel = advertisement.txPowerLevel;
	var manufacturerData = advertisement.manufacturerData;
	var serviceData = advertisement.serviceData;
	var serviceUuids = advertisement.serviceUuids;

	if (localName) {
	  console.log('Found peripheral with localName ' + localName)
	  if(localName == "Cloudcomm") {
	  	noble.stopScanning()
	  	explore(peripheral)
	  }
	}
	else {
		console.log('Found peripheral with no localname')
	}
}

function explore(peripheral) {
  console.log('services and characteristics:');

  peripheral.on('disconnect', function() {
      urlSet = false
      dataBuilder = null
      urlNode = ""
      console.log("disconnect")
      noble.startScanning();
  });

  peripheral.connect(function(error) { peripheral.discoverServices([], on_discover_services); });
}

function on_discover_services(error, services) {
      for(i = 0; i < services.length; i++) {
      	var service = services[i]
      	var characteristics
      	if (service.uuid == cc_service_uuid) {
      		console.log("Found cloudcomm service")
      		service.discoverCharacteristics([], on_discover_characteristics)
      		break
      	}
      }
}

function on_discover_characteristics(error, characteristics) {
	console.log("Discover characteristics")
	for(i = 0; i < characteristics.length; i++) {
		var characteristic = characteristics[i]
		if (characteristic.uuid == cc_data_uuid) {
			cc_data_char = characteristic
			cc_data_char.notify(true, function(err) { console.log("Enable cc data char notify") });
			cc_data_char.on('data', on_cc_data_data)
		}
		else if (characteristic.uuid == cc_meta_uuid) {
			cc_meta_char = characteristic
			cc_meta_char.notify(true, function(err) { console.log("Enable cc meta char notify") });
			cc_meta_char.on('data', on_cc_meta_data)
		}
		else if (characteristic.uuid == cc_req_uuid) {
			cc_req_char = characteristic
			cc_req_char.notify(true, function(err) { console.log("Enable cc req char notify")});
            cc_req_char.discoverDescriptors(function (error, descriptors) {
                console.log("Descriptors " + descriptors.length)
                for(i=0;i<descriptors.length;i++) {
                    var d = descriptors[i]
                    console.log(d.uuid)
                    if (d.uuid == "2900") {
                        console.log("foudn the right desc")
                        d.readValue(function(data) {console.log(data)})
                        break
                    }
                }
            })
			cc_req_char.on('data', on_cc_req_data)
		}
		else if(characteristic.uuid == cc_inc_uuid) {
			console.log("set cc_inc char")
			cc_inc_char = characteristic.on('data', on_cc_inc_data)
		}
		else if(characteristic.uuid == cc_ready_uuid) {
			console.log("set cc_ready char")
			cc_ready_char = characteristic
		}
		else if(characteristic.uuid == cc_ack_string) {
			console.log("set cc_ack char")
			cc_ack_char = characteristic
		}
	}
	console.log("Time to write phone ready")
	var buffer = new Buffer([0])
	//sleep.sleep(2)
	//console.log("Write sleep done")
	cc_ready_char.write(buffer, false, function() {console.log("Write phone ready")})
}

function on_cc_data_data(data, isNotification) {
	console.log("Received cc data notification")
	console.log("Seq" + data.readUInt8(0) )
    var seqNum = data.readUInt8(0)
    if(!urlSet) {
        urlNode = urlNode.concat(data.toString('ascii', 1, data.length))
    }
    else {
        if (dataBuilder == null) {
            dataBuilder = new Buffer(data.slice(1, data.length))
        }
        else {
            dataBuilder = Buffer.concat([dataBuilder, data.slice(1, data.length)])
        }
    }
    // We are done with our current data payload
	if(seqNum == 0xFF) {
        if(!urlSet) {
            urlSet = true
            console.log("URL: " + urlNode)
        }
        else {
            if(dataBuilder.length == 32 ) {
                console.log("Opo DB: " + dataBuilder.length + " " + dataBuilder.toString('hex'))
                var r = dataBuilder.readUInt32LE(6) / 32768 * 340.29 - .1
                var opo_data = {
                    version: dataBuilder.readUInt16LE(0),
                    rx_id: dataBuilder.readUInt16LE(2),
                    tx_id: dataBuilder.readUInt16LE(4),
                    range_dt: dataBuilder.readUInt32LE(6),
                    range: r,
                    ul_dt: dataBuilder.readUInt32LE(10),
                    m_time_confidence: dataBuilder.readUInt8(14),
                    m_unixtime: dataBuilder.readUInt32LE(15),
                    failed_rx_count: dataBuilder.readUInt32LE(19),
                    tx_unixtime: dataBuilder.readUInt32LE(23),
                    tx_time_confidence: dataBuilder.readUInt8(27),
                    ul_rf_dt: dataBuilder.readUInt32LE(28)
                }
                console.log(opo_data)

                request.post({
                    url: urlNode,
                    json: opo_data,
                    },
                    function(error, response, body) {
                        if(error) {
                            console.log(error)
                        }
                        else {
                            console.log(response.statusCode, body);
                        }
                    })

                nic_data = [opo_data]
                /*request.post({
                    url: "http://192.168.1.98:8080/muestreo-anual/sensors/opo",
                    json: opo_data,
                    },
                    function(error, response, body) {
                        if(error) {
                            console.log(error)
                        }
                        else {
                            console.log(response.statusCode, body);
                        }
                    })*/

                fs.appendFileSync('opo_cc_data.csv', JSON.stringify(opo_data) + "\n")
            }
            else {
                console.log("Other DB: " + dataBuilder.length + " " + dataBuilder.toString('hex'))
            }
            //var version = dataBuilder.readUInt16LE(0)
            //var version_b = dataBuilder.readUInt16BE(0)

            //console.log(version + " " + version_b)
            dataBuilder = null
        }
    }
    var ackBuffer = new Buffer([seqNum])
    cc_ready_char.write(ackBuffer, false, function() {console.log("Write CC ack to peripheral")})
}

function on_cc_meta_data(data, isNotification) {
	console.log("Received cc meta notification")
    var buffer = new Buffer([0])
    cc_ready_char.write(buffer, false, function() {console.log("Write phone ready 2")})
}

function on_cc_req_data(data, isNotification){
	console.log("Received cc req notificatoin")
    var reqnum = data.readUInt8(0)
    if(reqnum == 0) {
        console.log("Node wants the time")
        var cTime = Date.now() / 1000
        cTime = cTime & 0xFFFFFFFF
        var timeBuffer = new Buffer(5)
        timeBuffer.writeUInt8(0, 0)
        timeBuffer.writeUInt32BE(cTime, 1)
        console.log(timeBuffer.toString('hex') + " " + timeBuffer.length)
        cc_inc_char.write(timeBuffer, false, function() {console.log("Wrote time to peripheral")})
    }
}

function on_cc_inc_data(data, isNotification){
	console.log("Received cc inc notificatoin")
}
