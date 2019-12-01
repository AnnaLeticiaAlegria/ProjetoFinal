print("iniciou programa")

sck = tls.createConnection()
sck:connect(443,"www.howsmyssl.com") -- porta 443: https : handshake funcionou, desconectou : Reason:[-0x7880]
--sck:connect(443,"api.pushbullet.com") -- falhou o handskahe : Reason:[-0x7780]
--sck:connect(80,"www.httpvshttps.com") --porta 80: http --> falhou o handshake : Reason:[-0x7200]

sck:on("connection", function(sk) print("Conectou") end)
sck:on("receive", function(sk, str)
  print("Mensagem: "..str)
  sk:send(str)
end)
sck:on("disconnection", function(sk,err)
  print("Desconectou")
  if (err ~=nil) then
    print(err)
  end
end)

local port, ip = sck:getpeer()
print("getpeer: ip: " .. ip .. " porta: " .. port)
