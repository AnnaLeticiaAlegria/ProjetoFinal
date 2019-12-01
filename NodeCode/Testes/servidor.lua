local socket = require("socket")
local server = socket.bind("*", 0)

local ip, port = server:getsockname()
print("IP: " .. ip .. ", Port: " .. port)

-- loop forever waiting for clients
while 1 do
  local client = server:accept()
  print("Alguem se conectou")

  client:settimeout(20)

  client:send("Alo alo\n")
  local line, err = client:receive()

  if not err then
    print("Recebi: " .. line)
    client:send(line .. "\n")
  else
    print("Erro: " .. err);
  end
  client:close()
end

