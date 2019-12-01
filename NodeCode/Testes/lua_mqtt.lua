m = mqtt.Client("clientid_1410427_tcc", 120)
m:on("connect", function(client) print ("connected") end)

function publica(c)
  c:publish("dados-sensores-1410427","alo de 1410427",0,0, 
            function(client) print("mandou!") end)
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
  publica(client)
  client:subscribe("dados-sensores-1410427", 0, novaInscricao)
end 

m:connect("test.mosquitto.org", 1883, 0, 
             funcaoConectado,
             function(client, reason) print("failed reason: "..reason) end)