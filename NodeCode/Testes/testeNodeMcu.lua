led1 = 3
led2 = 6
sw1 = 1
sw2 = 2
-- pinos de leds são de saída
gpio.mode(led1, gpio.OUTPUT)
gpio.mode(led2, gpio.OUTPUT)
-- apaga os leds
gpio.write(led1, gpio.LOW);
gpio.write(led2, gpio.LOW);
-- pino de botão 1 por interrupção
-- pullup garante sinal qdo não há algum
gpio.mode(sw1,gpio.INT,gpio.PULLUP)
ledstate = false
-- callback da interrupção
local function pincb(level,
timestamp)
ledstate = not ledstate
if ledstate then
gpio.write(led1, gpio.HIGH);
else
gpio.write(led1, gpio.LOW);
end
end
-- registrar tratamento para botão
gpio.trig(sw1, "down", pincb)
