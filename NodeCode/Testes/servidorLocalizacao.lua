srv = net.createServer(net.TCP)

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
      print(code, data)
      t = data
      -- print(sjson.decode(data))
      -- print(sjson.decode(data).location.lat)
      -- print(sjson.decode(data).location.lng)

      -- t = sjson.decode('{"key":"value"}')
      -- for k,v in pairs(t) do print(k,v) end
      
    end
  end);
  return s;
end




function receiver(sck, request)

  local buf = t
  print(t)

  if (t ~= nil) then
    sck:send("Access-Control-Allow-Methods POST,GET,OPTIONS", 
    function ()
      sck:send("Access-Control-Allow-Headers Origin, X-Requested-With, Content-Type, Accept", 
      function ()
        sck:send(buf, 
        function()  -- callback: fecha o socket qdo acabar de enviar resposta
          sck:close() 
        end)
      end)
    end)
  end


end

if srv then
  wifi.sta.getap(montaJson)
  srv:listen(80, function(conn)
      print("CONN")
      conn:on("receive", receiver)
    end)
end