sk=net.createConnection(net.TCP, 0)
sk:on("receive", function(sck, c) print(c) end )
sk:connect(56539, "0.0.0.0")
sk:send("HELLO")
sk:close()
print("sent to server")
