uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)

-- when '\r' is received.
uart.on("data", "\r",
  function(data)
    sendData = data
    print("recebendo: " .. sendData)
    if data=="quit\r" then
      uart.on("data") -- unregister callback function
    end
end, 0)

local cl;

function montaJson(wifiAccessPoints)
  primeirovalor = true;
  s = '  {\n "considerIp":"false",\n "wifiAccessPoints": [';
  for key, value in pairs(wifiAccessPoints) do
    print(key .. " " .. value);
    --vírgula
    if primeirovalor then
      primeirovalor = false;
    else
      s = s .. ','
    end
  
    --linha
    local authmode, rssi, bssid, channel = string.match(value, "([^,]+),([^,]+),([^,]+),([^,]+)");
    s = s .. '\n      { "macAddress": "' .. bssid .. '" }';
    
  end
  
  s = s .. '\n    ]\n  }';
  print(s);
  http.post('https://www.googleapis.com/geolocation/v1/geolocate?key=AIzaSyCfX0_CrDjekaD2QgKpdH44jnjr1Xvb43U',
  'Content-Type: application/json\r\n', s, 
  function(code, data)
    if (code < 0) then
      print("HTTP request failed :", code)
    else
      publica(cl, "u001", "geo", data)
      publica(cl, "u001", "fall", "true")
      publica(cl, "u001", "gas", "false")
    end
  end);
  return s;
end


m = mqtt.Client("clientid_1410427_tcc", 120)
m:on("connect", function(client) print ("connected") end)

--Função chamada quando se deseja publicar em um tópico do mqtt
function publica(c, idCliente, topico, dados)
  c:publish("dados-sensores-1410427/"..idCliente..'/'..topico, dados ,0,0, 
            function(client) print("mandou "..dados.." para o topico "..topico) end)
end

function novaInscricao (c)
  local msgsrec = 0
  function novamsg (c, t, m)
    print ("mensagem ".. msgsrec .. ", topico: ".. t .. ", dados: " .. m)
    msgsrec = msgsrec + 1
  end
  c:on("message", novamsg)
end

function funcaoConectado (client)
  cl = client
  wifi.sta.getap(montaJson)

  
  client:subscribe("dados-sensores-1410427", 0, novaInscricao)
end 

m:connect("test.mosquitto.org", 1883, 0, 
             funcaoConectado,
             function(client, reason) print("failed reason: "..reason) end)