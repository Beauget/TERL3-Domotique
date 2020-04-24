-- Heater Management Script

commandArray = {}

temperatureDevice = 'Température / Humidité'; -- Temperature Sensor
targetTemperatureDevice = 'Température Idéale'; -- Target Temperature Selector
autoSwitch = 'Chauffage Auto.'; -- Switch to enable auto heater
heaterDevice = 'Chauffage'; -- Heater Controler

if devicechanged[temperatureDevice] and otherdevices[autoSwitch] == 'On' then
    if tonumber(otherdevices_temperature[temperatureDevice]) < tonumber(otherdevices[targetTemperatureDevice]) then
        commandArray[heaterDevice] = 'On'
    else
        commandArray[heaterDevice] = 'Off'
    end
end

return commandArray
