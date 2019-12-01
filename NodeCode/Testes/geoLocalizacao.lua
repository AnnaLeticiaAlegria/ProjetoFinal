-- Recebe uma lista com as informações dos pontos de acesso e retorna o json a ser enviado
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
      print(sjson.decode(data))
      print(sjson.decode(data).location.lat)
      print(sjson.decode(data).location.lng)

      t = sjson.decode('{"key":"value"}')
      for k,v in pairs(t) do print(k,v) end

      --mandar data
      http.post('http://2e11f872.ngrok.io/GeoLocation', '{"location":' .. data .. '}',
        function(code,data)
          if (code < 0) then
            print("HTTP request failed :", code)
          else
            print(code, data)
          end
        end)
    end
  end);
  return s;
end


wifi.sta.getap(montaJson)

