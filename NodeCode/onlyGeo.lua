local mqttClient
local mqttObject
local clientId = "u001"

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
   wifi.sta.getap(sendGeo)
  end)
  then
    print("Geo tmr not created")
  end
  
  mqttClient:subscribe("dados-sensores-1410427", 0, newSubscribe)
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
  http.post('https://www.googleapis.com/geolocation/v1/geolocate?key=AIzaSyCfX0_CrDjekaD2QgKpdH44jnjr1Xvb43U',
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
  end);
  return s;
end