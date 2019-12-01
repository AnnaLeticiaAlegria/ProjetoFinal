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

  mqttClient:subscribe("dados-sensores-1410427/features/#", 0, newSubscribe)

  mqttClient:on("message", function(client, topic, data)
    -- print(topic .. " :" )
    if data ~= nil then
      -- print("data recebida: " .. data)
      
      local idxTopic = string.find(topic, "/" , string.len("dados-sensores-1410427/features/") + 1)
      if (idxTopic) then
        local subtopic = string.sub(topic, idxTopic + 1, string.len(topic) - 1)
        print("*".. subtopic .. "==" .. data .. "//\n")
      end
    else
      print("data nil")
    end
  end)

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
      if (idxKey and idxValue) then key = string.sub(data, idxValue + 2, idxKey - 1) end
      idxValue = string.find(data, "//", idxKey)
      if (idxValue and idxKey) then value = string.sub(data, idxKey + 2, idxValue - 1) end
      if (key) then 
        -- print(key)
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
        mqttClient:publish("dados-sensores-1410427/data/"..clientId..'/'..key, value ,0,0, sendMqtt)
        -- print("mandou "..value.." para o topico "..key)
        index = index + 1
      end
    end
  end

  if (#dataArray > 0) then 
    for key, value in pairs(dataArray[index]) do 
      mqttClient:publish("dados-sensores-1410427/data/"..clientId..'/'..key, value ,0,0, sendMqtt)
      -- print("mandou "..value.." para o topico "..key)
      index = index + 1
    end 
  end
end