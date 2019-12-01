srv = net.createServer(net.TCP)

function receiver(sck, request)

  -- analisa pedido para encontrar valores enviados
  local _, _, method, path, vars = string.find(request, "([A-Z]+) ([^?]+)%?([^ ]+) HTTP");
  -- se nÃ£o conseguiu casar, tenta sem variÃ¡veis
  if(method == nil)then
    _, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP");
  end

  local buf = [[
  <html>
  <body>
  <h2><i>ESP8266 Web Server</i></h2>
  </body>
  </html>
  ]]

  sck:send(buf, 
           function()  -- callback: fecha o socket qdo acabar de enviar resposta
             sck:close() 
           end)

end

if srv then
  srv:listen(80, function(conn)
      print("CONN")
      conn:on("receive", receiver)
    end)
end