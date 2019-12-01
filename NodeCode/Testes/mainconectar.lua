print("iniciou programa")
cl=net.createConnection(net.TCP, 0)

cl:on("receive", function(sck, strReceived)
  print("Mensagem: " .. strReceived)
  sck:send(strReceived)
end)

cl:on("disconnection", function(sck, err) print("desconectou")  end)
cl:on("connection", function(sck) print("Conectou") end)

-- cl:on("sent", function (sck) sck:send("alo alo") end)

cl:connect(52823, "172.20.10.6")

local port, ip = cl:getpeer()
print("getpeer: ip: " .. ip .. " porta: " .. port)
