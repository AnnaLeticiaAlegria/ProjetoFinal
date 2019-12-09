local mqttClient
local mqttObject
local clientId = "u001"
local dataArray = {}

local wificonf = {
  ssid = "iPhone de Anna Leticia",
  pwd = "nihon2310",
  got_ip_cb = function (iptable) 
      print ("ip: ".. iptable.IP) 
      mqttObject = mqtt.Client("clientid_1410427_tcc", 120)
      mqttObject:on("connect", function(client) print ("connected") end)

      mqttObject:connect("test.mosquitto.org", 1883, 0, whenConnected, function(client, reason) print("failed reason: "..reason) end)
    end,
  dhcp_timeout_cb = function() print ("dhcp timeout") end,
  connected_cb = function (info) print ("connected ") end,
  auto = true,
  save = false
}

wifi.setmode(wifi.STATION)
wifi.sta.config(wificonf)

function whenConnected (client)
  mqttClient = client

  if not tmr.create():alarm(10000, tmr.ALARM_AUTO, function()
    wifi.sta.getap(sendSerial)
   end)
   then
     print("Serial tmr not created")
   end

  mqttClient:subscribe("dados-sensores-1410427", 0, newSubscribe)
end 

uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)

uart.on("data", '\r',
  function(data)
    print("recebendo: " .. data)
    dataArray = {}
    local idxValue = -1
    local idxKey = 0
    local key, value
    while(idxValue ~= nil and idxValue + 2 < string.len(data)) do
      idxKey = string.find(data, "==", idxValue + 2)
      if (idxKey and idxValue) then print("idxKey: "..idxKey); key = string.sub(data, idxValue + 2, idxKey - 1) end
      idxValue = string.find(data, "//", idxKey)
      if (idxValue and idxKey) then print("idxValue: "..idxValue);  value = string.sub(data, idxKey + 2, idxValue - 1) end
      if (key) then 
        print(key)
        print(value)
        local dataKeyArray = {}
        dataKeyArray[key] = value
        table.insert(dataArray, dataKeyArray)
      end
    end
end, 0)

function sendSerial () 
  local index = 1
  local state = 0
  print("Entrou na sendSerial", #dataArray)

  local function sendMqtt () 
    if (index <= #dataArray) then
      for key, value in pairs(dataArray[index]) do 
        if (state == 1) then state = 0 else state = 1 end
        mqttClient:publish("dados-sensores-1410427/"..clientId..'/'..key, value ,0,0, sendMqtt)
        print("mandou "..value.." para o topico "..key)
        index = index + 1
      end
    end
  end

  if (#dataArray > 0) then 
    for key, value in pairs(dataArray[index]) do 
      print("primeiro: "..key.." "..value)
      mqttClient:publish("dados-sensores-1410427/"..clientId..'/'..key, value ,0,0, sendMqtt)
      print("mandou "..value.." para o topico "..key)
      index = index + 1
    end 
  end
end