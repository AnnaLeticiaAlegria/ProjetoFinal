node.egc.setmode(node.egc.ALWAYS)

local mqttClient
local mqttObject
local clientId = "u001"
local dataArray = {}
local geo = 1

local wificonf = {
  ssid = "iPhone de Anna Leticia",
  pwd = "nihon2310",
  got_ip_cb = function (iptable) 
      print ("ip: ".. iptable.IP) 
      mqttObject = mqtt.Client("clientid_1410427_tcc", 120)
      mqttObject:on("connect", function(client) print ("connected") end)
      print(node.egc.meminfo())
      mqttObject:connect("test.mosquitto.org", 1883, 0, whenConnected, function(client, reason) print("Mqtt failed reason: "..reason) end)
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

  uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)

  uart.on("data", '\r',
    function(data)
      print("recebendo: " .. data)
      dataArray = {}
      local idxValue = -1
      local idxKey = 0
      local key, value
      while(idxValue ~= nil and idxValue + 2 < string.len(data)) do
        idxKey = string.find(data, "==", idxValue)
        if (idxKey and idxValue) then key = string.sub(data, idxValue + 2, idxKey - 1) end
        idxValue = string.find(data, "//", idxKey)
        if (idxValue and idxKey) then value = string.sub(data, idxKey + 2, idxValue - 1) end
        if (key) then 
          print(key)
          print(value)
          local dataKeyArray = {}
          dataKeyArray[key] = value
          table.insert(dataArray, dataKeyArray)
        end
      end
  end, 0)
  
  if not tmr.create():alarm(15000, tmr.ALARM_AUTO, function()
    sendData()
   end)
   then
     print("Tmr not created")
   end
  
  mqttClient:subscribe("dados-sensores-1410427", 0, newSubscribe)
end 

function sendData () 
  print("geo: "..geo)
  if (geo == 1) then
    wifi.sta.getap(sendGeo)
    geo = 0
  else 
    sendSerial()
    geo = 1
  end
end

function sendSerial () 
  local index = 1

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
      mqttClient:publish("dados-sensores-1410427/"..clientId..'/'..key, value ,0,0, sendMqtt)
      print("mandou "..value.." para o topico "..key)
      index = index + 1
    end 
  end
end


function sendGeo(wifiAccessPoints)
  firstValue = true;
  local s = '  {\n "considerIp":"false",\n "wifiAccessPoints": [';
  for key, value in pairs(wifiAccessPoints) do
    -- print(key .. " " .. value);
    if firstValue then
      firstValue = false;
    else
      s = s .. ','
    end
    local authmode, rssi, bssid, channel = string.match(value, "([^,]+),([^,]+),([^,]+),([^,]+)");
    s = s .. '\n      { "macAddress": "' .. bssid .. '" }';
  end
  
  s = s .. '\n    ]\n  }';
  -- print(s);
  print(node.egc.meminfo())
  http.post('https://www.googleapis.com/geolocation/v1/geolocate?key=',
  'Content-Type: application/json\r\n', s, 
  function(code, data)
    if (code < 0) then
      print("HTTP request failed :", code)
    else
      mqttClient:publish("dados-sensores-1410427/"..clientId..'/geo', data ,0,0, 
            function(client) 
              print("Mandou localizacao")
            end)
    end
    print(node.egc.meminfo())
  end);
  return s;
end
