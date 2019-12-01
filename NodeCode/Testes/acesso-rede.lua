wificonf = {
  ssid = "iPhone de Anna Leticia",
  pwd = "nihon2310",
  -- ssid = "AndroidAP",
  -- pwd = "svks4163",
  got_ip_cb = function (iptable) print ("ip: ".. iptable.IP) end,
  dhcp_timeout_cb = function() print ("dhcp timeout") end,
  connected_cb = function (info) print ("connected ") end,
  auto = true,
  save = false
}

wifi.setmode(wifi.STATION)
wifi.sta.config(wificonf)
