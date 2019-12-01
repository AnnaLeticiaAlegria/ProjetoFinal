data = "heart//80\\gas//true\\fall//false\\"
local dataArray = {}
local idxValue = 0
local idxKey = 0
local key, value
while(idxValue + 2 < string.len(data)) do
  idxKey = string.find(data, "//", idxValue)
  if (idxKey) then key = string.sub(data, idxValue + 1, idxKey - 1) end
  idxValue = string.find(data, "\\", idxKey)
  if (idxValue) then value = string.sub(data, idxKey + 2, idxValue - 1) end
  local dataKeyArray = {}
  dataKeyArray[key] = value
  table.insert(dataArray, dataKeyArray)
end

for idx, item in pairs(dataArray) do
  for key, value in pairs(item) do 
    print(key, value)
  end
end


function sendSerial (dataArray) 
  local index = 1

  local function sendMqtt () 

  end
end


-- for key, value in pairs(dataArray) do
--   print(key)
--   print(value)
-- end

-- function sendSerial (dataArray) 
--   local index = 1

--   for key, value in pairs(dataArray) do
    
--   end